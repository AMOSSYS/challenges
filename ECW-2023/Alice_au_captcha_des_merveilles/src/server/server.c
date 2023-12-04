#include "server.h"
#include <errhandlingapi.h>
#include <heapapi.h>
#include <http.h>
#include <libloaderapi.h>
#include <winerror.h>

uint8_t* read_decrypt_flag(uint8_t* key) {
    uint8_t* flag_decrypted;
    rc4_ctx ctx;

    flag_decrypted = (uint8_t*) ALLOC_MEM(FLAG_SIZE);
    
    rc4_ks(&ctx, key, KEY_SIZE);
    rc4_decrypt(&ctx, (uint8_t*) ENCRYPTED_FLAG, flag_decrypted, FLAG_SIZE);
    return flag_decrypted;
}
uint8_t* read_decrypt_image(uint8_t* key, uint32_t* decrypted_image_size) {
    uint8_t* image_buffer = NULL;
    uint8_t* image_buffer_decrypted = NULL;
    uint8_t resource_id =  100 + key[0];
    HRSRC res_info;
    HGLOBAL res_data;
    rc4_ctx ctx;

    var_dump("resource_id = %d\n", resource_id);

    res_info = FindResourceW(NULL, MAKEINTRESOURCEW(resource_id), RT_RCDATA); 
    if(res_info == NULL) {
        var_dump("FindResourceW failed with error code %d\n", GetLastError());
        goto res_failed;
    }
    res_data = LoadResource(NULL, res_info);
    if(res_data == NULL) {
        var_dump("LoadResource failed with error code %d\n", GetLastError());
        goto res_failed;
    }
    image_buffer = (uint8_t*) LockResource(res_data);
    *decrypted_image_size = SizeofResource(NULL, res_info);

    image_buffer_decrypted = (uint8_t*) ALLOC_MEM(*decrypted_image_size);

    rc4_ks(&ctx, key, KEY_SIZE);
    #if DEBUG
    for(int i = 0; i < KEY_SIZE; i++) 
        printf("%02x", key[i]);
    printf("\n");
    #endif
    rc4_decrypt(&ctx, image_buffer, image_buffer_decrypted, *decrypted_image_size);

    //FreeResource(res_data);
    
    res_failed:
    return image_buffer_decrypted;
}


DWORD SendHttpGetResponse(HANDLE handle_req_queue, PHTTP_REQUEST request, USHORT status_code, uint8_t post) {
    HTTP_RESPONSE response; 
    DWORD result = 0; 
    DWORD bytes_sent;
    uint8_t key[KEY_SIZE];
    ULONG bytes_read;
    HTTP_DATA_CHUNK data_chunk;
    uint8_t* decrypted_image = NULL;
    uint32_t decrypted_image_size = 0;
    uint8_t* decrypted_flag = NULL; 
    
    bytes_read = 0;

    INITIALIZE_HTTP_RESPONSE(&response, status_code, "OK");

    if(request->Flags & HTTP_REQUEST_FLAG_MORE_ENTITY_BODY_EXISTS) {
        bytes_read = 0; 
        bytes_sent = 0;
        result = HttpReceiveRequestEntityBody(
                    handle_req_queue,
                    request->RequestId,
                    0,
                    key,
                    KEY_SIZE,
                    &bytes_read,
                    NULL 
                );
        if(result != NO_ERROR) {
            var_dump("HttpReceiveRequestEntityBody failed with error code %d\n", result);
            goto http_receive_failed;
        }
        result = HttpSendHttpResponse(
                   handle_req_queue,           // ReqQueueHandle
                   request->RequestId, // Request ID
                   HTTP_SEND_RESPONSE_FLAG_MORE_DATA,
                   &response,       // HTTP response
                   NULL,            // pReserved1
                   &bytes_sent,      // bytes sent-optional
                   NULL,            // pReserved2
                   0,               // Reserved3
                   NULL,            // LPOVERLAPPED
                   NULL             // pReserved4
                );
        if(result != NO_ERROR) {
            var_dump("HttpSendHttpResponse failed with error code %d\n", result);
            goto http_send_http_failed;
        }

        data_chunk.DataChunkType = HttpDataChunkFromMemory;
        if(post) {
            decrypted_flag = read_decrypt_flag(key);
            data_chunk.FromMemory.BufferLength = FLAG_SIZE;
            data_chunk.FromMemory.pBuffer = decrypted_flag;
        } else {
            decrypted_image = read_decrypt_image(key, &decrypted_image_size);
            var_dump("decrypted_image_size %d\n", decrypted_image_size); 
            data_chunk.FromMemory.BufferLength = decrypted_image_size;
            data_chunk.FromMemory.pBuffer = decrypted_image;
        }
        result = HttpSendResponseEntityBody(
                    handle_req_queue,
                    request->RequestId,
                    0,           // This is the last send.
                    1,           // Entity Chunk Count.
                    &data_chunk,
                    NULL,
                    NULL,
                    0,
                    NULL,
                    NULL
                    );

        if(result != NO_ERROR)
            var_dump("HttpSendResponseEntityBody failed with error code %d\n", result);


        if(decrypted_image)
            DEALLOC_MEM(decrypted_image);
        if(decrypted_flag)
            DEALLOC_MEM(decrypted_flag);
    }
    http_send_http_failed:

    http_receive_failed:
    return result;
}

DWORD receive_requests(HANDLE handle_req_queue) {
    uint32_t result; 
    HTTP_REQUEST_ID request_id;
    DWORD bytes_read;
    PHTTP_REQUEST request;
    PCHAR request_buffer;
    uint32_t request_buffer_length;

    request_buffer_length = sizeof(HTTP_REQUEST) + 2048;
    request_buffer = (PCHAR) ALLOC_MEM(request_buffer_length);

    if(request_buffer == NULL) 
        return ERROR_NOT_ENOUGH_MEMORY;

    request = (PHTTP_REQUEST) request_buffer;
    
    HTTP_SET_NULL_ID(&request_id);

    while(1) {
        RtlZeroMemory(request, request_buffer_length);

        result = HttpReceiveHttpRequest(handle_req_queue, request_id, 0, request, request_buffer_length, &bytes_read, NULL);
        if (result == NO_ERROR) {
            switch (request->Verb) {
                case HttpVerbGET: 
                    var_dump("Got a GET request for %ls\n", request->CookedUrl.pFullUrl);
                    SendHttpGetResponse(handle_req_queue, request, 200, 0);
                    break;
                case HttpVerbPOST:
                    var_dump("Got a POST request for %ls\n", request->CookedUrl.pFullUrl);               
                    SendHttpGetResponse(handle_req_queue, request, 200, 1);
                    break;
                default:
                    var_dump("Got an unmanaged request method %d\n", request->Verb);
            }
        }
    }
    DEALLOC_MEM(request_buffer);
    
    return 0;
}

int main()
{
    uint32_t ret_code; 
    HANDLE handle_req_queue = NULL;
    PCWSTR localhost_url = L"http://127.0.0.1:8888/steps/"; 
    HTTPAPI_VERSION http_api_version = HTTPAPI_VERSION_1;

    ret_code = HttpInitialize(http_api_version, HTTP_INITIALIZE_SERVER, NULL);

    if(ret_code != NO_ERROR) {
        var_dump("HttpInitialize failed with %lu\n", ret_code);
        return ret_code;
    }

    ret_code = HttpCreateHttpHandle(&handle_req_queue, 0);
    if(ret_code != NO_ERROR) {
        var_dump("HttpCreateHttpHandle failed with %lu\n", ret_code);
        goto http_create_handle_failed;
    }

    ret_code = HttpAddUrl(handle_req_queue, localhost_url, NULL);
    if(ret_code != NO_ERROR) {
        var_dump("HttpAddUrl failed with %lu\n", ret_code);
        goto http_add_url_failed;
    }

    receive_requests(handle_req_queue);

    http_add_url_failed:
    HttpRemoveUrl(handle_req_queue, localhost_url);
    
    http_create_handle_failed:
    if(handle_req_queue)
        CloseHandle(handle_req_queue);

    HttpTerminate(HTTP_INITIALIZE_SERVER, NULL);

    return ret_code;
}
