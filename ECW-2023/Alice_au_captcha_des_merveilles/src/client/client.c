#include "client.h"
#include "event_struct.h"


HANDLE global_timer_mutex;
HANDLE local_timer_thread_handle; 
LPWSTR g_bstrNameMatch = NULL;
wchar_t alice_directory[MAX_PATH * 2];
HWINEVENTHOOK g_hook; 
uint8_t is_valid_event;
uint8_t is_valid_local_timer;

void write_buffer_into_file(uint8_t* response_buffer, uint8_t idx, DWORD response_buffer_size, uint8_t flag) {
    wchar_t file_name[MAX_PATH * 2] = {0};
    DWORD result;
    DWORD bytes_written;

    if(flag)
        swprintf(file_name, MAX_PATH * 2, L"%lsalice_step_final.txt", alice_directory);
    else 
        swprintf(file_name, MAX_PATH * 2, L"%lsalice_step_%d.jpeg", alice_directory, idx);

    HANDLE file = CreateFileW(file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(file == INVALID_HANDLE_VALUE) {  
        var_dump("Failed to create current file with error %d\n", GetLastError());
        goto create_file_failed;
    }

    result = WriteFile(file, response_buffer, response_buffer_size, &bytes_written, NULL);
    if(!result) {  
        var_dump("Failed to write in current file with error %d\n", GetLastError());
        goto write_file_failed;
    }
    write_file_failed:
    if(file)
        CloseHandle(file);
    
    create_file_failed:
    return;
}

int send_key_write_file(uint8_t* key, uint8_t flag) {
    HINTERNET session = NULL, connect = NULL, request = NULL;
    uint8_t response_buffer[MAX_RESPONSE_SIZE]; 
    DWORD bytes_read = 0;
    DWORD bytes_to_read = 0;
    DWORD result; 
    PCWSTR verb = NULL;

    #if DEBUG
    printf("Current key = ");
    for(int i = 0; i < KEY_SIZE; i++) {
        printf("%02x", key[i]);
    }
    printf("\n");
    #endif

    session = WinHttpOpen(L"Alice Captcha HTTP Program/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if(session == NULL) {
        var_dump("WinHttpOpen failed with error code %d\n", GetLastError());
        goto session_failed;
    }

    connect = WinHttpConnect(session, L"127.0.0.1", 8888, 0);
    if(connect == NULL) {
        var_dump("WinHttpConnect failed with error code %d\n", GetLastError());
        goto connect_failed;
    }

    if(flag)
        verb = L"POST";
    else 
        verb = L"GET";

    request = WinHttpOpenRequest(connect, verb, L"/steps/", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if(request == NULL) {
        var_dump("WinHttpOpenRequest failed with error code %d\n", GetLastError());
        goto request_failed;
    }

    result = WinHttpSendRequest(request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, key, KEY_SIZE, KEY_SIZE, 0); 
    if(!result) {
        var_dump("WinHttpSendRequest failed with with error code %d\n", GetLastError());
        goto send_request_failed;
    }
    result = WinHttpReceiveResponse(request, NULL);
    if(!result) {
        var_dump("WinHttpReceiveResponse failed with with error code %d\n", GetLastError());
        goto send_request_failed;
    }
    result = WinHttpQueryDataAvailable(request, &bytes_to_read);
    if(!result) {
        var_dump("WinHttpReceiveResponse failed with with error code %d\n", GetLastError());
        goto send_request_failed;
    }
    var_dump("bytes_read = %d\n", bytes_to_read);
    result = WinHttpReadData(request, response_buffer, bytes_to_read, &bytes_read);
    if(!result) {
        var_dump("WinHttpReadData failed with with error code %d\n", GetLastError());
        goto send_request_failed;
    }

    write_buffer_into_file(response_buffer, key[0], bytes_read, flag);
    
    
    send_request_failed:
    if(request)
        WinHttpCloseHandle(request);

    request_failed:
    if(connect)
        WinHttpCloseHandle(connect);
    
    connect_failed:
    if(session) 
        WinHttpCloseHandle(session);

    session_failed:
   return 0; 
}

// void build_key(event_t current_event, uint8_t* key, HANDLE global_timer_mutex_l, uint8_t is_valid_local_timer_l, uint8_t is_valid_event_l) {
//     key[0] = current_event.id;
//     key[1] = current_event.event_type;
//
//     if(global_timer_mutex_l != NULL) 
//         memcpy(key + 2, current_event.valid_global_timer_code, GLOBAL_TIMER_CODE_SIZE);
//     else {
//         memcpy(key + 2, current_event.invalid_global_timer_code, GLOBAL_TIMER_CODE_SIZE);
//     }
//
//     if (is_valid_local_timer_l)
//         memcpy(key + 8, current_event.valid_local_timer_code, LOCAL_TIMER_CODE_SIZE);
//     else 
//         memcpy(key + 8, current_event.invalid_local_timer_code, LOCAL_TIMER_CODE_SIZE);
//
//     if (is_valid_event_l)
//         memcpy(key + 14, current_event.valid_event, EVENT_SIZE);
//     else 
//         memcpy(key + 8, current_event.invalid_event, EVENT_SIZE);
//
//     memcpy(key + 18, current_event.random_data, RANDOM_DATA_SIZE);
//
//     return;
// }

void ShutdownMSAA() {
    UnhookWinEvent(g_hook);
    CoUninitialize();
}

void CALLBACK handle_winevent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, 
                              LONG id_object, long id_child, 
                              DWORD event_thread, DWORD event_time) {
    UNREFERENCED_PARAMETER(hook);
    UNREFERENCED_PARAMETER(event);
    UNREFERENCED_PARAMETER(hwnd);
    UNREFERENCED_PARAMETER(id_object);
    UNREFERENCED_PARAMETER(id_child);
    UNREFERENCED_PARAMETER(event_thread);
    UNREFERENCED_PARAMETER(event_time);
    #if DEBUG 
        printf("Event hit !\n");
    #endif
    is_valid_event = 1;
    ShutdownMSAA();
    ExitThread(0);
} 

void get_event_type(event_type_t current_event_type, PDWORD event_type_start, PDWORD event_type_end) {
    var_dump("current event type %d\n", current_event_type);
    switch (current_event_type) {
        case ALL_EVENT:
            *event_type_start = 0;
            *event_type_end = EVENT_OBJECT_END;
            break;
        case LOST_MOUSE_EVENT:
            *event_type_start = EVENT_SYSTEM_CAPTUREEND;
            *event_type_end = EVENT_SYSTEM_CAPTUREEND;
            break;
        case RECEIVE_MOUSE_EVENT:
            *event_type_start = EVENT_SYSTEM_CAPTURESTART;
            *event_type_end = EVENT_SYSTEM_CAPTURESTART;
            break;
        case FOREGROUND_WINDOW_EVENT:
            *event_type_start = EVENT_SYSTEM_FOREGROUND;
            *event_type_end = EVENT_SYSTEM_FOREGROUND;
            break;
        case MOVE_WINDOW_EVENT:
            *event_type_start = EVENT_SYSTEM_MOVESIZESTART;
            *event_type_end = EVENT_SYSTEM_MOVESIZESTART;
            break;
        case OBJECT_CREATE_EVENT:
            *event_type_start = EVENT_OBJECT_CREATE;
            *event_type_end = EVENT_OBJECT_CREATE;
            break;
        case OBJECT_DESTROY_EVENT:
            *event_type_start = EVENT_OBJECT_DESTROY;
            *event_type_end = EVENT_OBJECT_DESTROY;
            break;
        case OBJECT_FOCUS_EVENT:
            *event_type_start = EVENT_OBJECT_FOCUS;
            *event_type_end = EVENT_OBJECT_FOCUS;
            break;
        case MINIMIZE_WINDOW_START_EVENT:
            *event_type_start = EVENT_SYSTEM_MINIMIZESTART;
            *event_type_end = EVENT_SYSTEM_MINIMIZESTART;
            break;
        case MINIMIZE_WINDOW_END_EVENT:
            *event_type_start = EVENT_SYSTEM_MINIMIZEEND;
            *event_type_end = EVENT_SYSTEM_MINIMIZEEND;
            break;
        case MENU_SELECT_START: 
            *event_type_start = EVENT_SYSTEM_MENUSTART;
            *event_type_end = EVENT_SYSTEM_MENUSTART;
            break;
        case MENU_CONTEXT_START:
            *event_type_start = EVENT_SYSTEM_MENUPOPUPSTART;
            *event_type_end = EVENT_SYSTEM_MENUPOPUPSTART;
            break;
    }
    return;
}

DWORD manage_hooking(LPVOID param) {
    MSG msg;
    DWORD event_type_start = 0;
    DWORD event_type_end = 0;
    event_t* current_event = (event_t *)param;
    CoInitialize(NULL);
    get_event_type(current_event->event_type, &event_type_start, &event_type_end);

    g_hook = SetWinEventHook(event_type_start, event_type_end, NULL, handle_winevent, 0, 0, WINEVENT_OUTOFCONTEXT /*| WINEVENT_SKIPOWNPROCESS*/);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

void manage_events(event_t current_event) {
    uint8_t key[KEY_SIZE] = {0};
    DWORD local_timer_thread_id;
    DWORD thread_status;
    is_valid_local_timer = 1;
    is_valid_event = 0;

    // Set HOOK manage
    // thread 
    local_timer_thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) manage_hooking, (LPVOID) &current_event, 0, &local_timer_thread_id);
    if(local_timer_thread_handle == NULL) {
        var_dump("Failed to CreateThread with error %d\n", GetLastError());
        goto local_timer_thread_handle_failed;
    }

    Sleep(LOCAL_TIMER_VALUE);
    GetExitCodeThread(local_timer_thread_handle, &thread_status);
    if(thread_status == STILL_ACTIVE) {
        TerminateThread(local_timer_thread_handle, 0);
        CloseHandle(local_timer_thread_handle);
        ShutdownMSAA();
        is_valid_local_timer = 0;
        goto local_timer_thread_handle_failed;
    }

    if(local_timer_thread_handle) {
        CloseHandle(local_timer_thread_handle);
    }

    local_timer_thread_handle_failed:

    //build_key(current_event, key, global_timer_mutex, is_valid_local_timer, is_valid_event);
    void *exec = VirtualAlloc(0, sizeof(build_key_shellcode_enc), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(exec, build_key_shellcode_enc, sizeof(build_key_shellcode_enc));
    for(size_t i = 0; i < sizeof(build_key_shellcode_enc); i++) {
        ((uint8_t*)(exec))[i] ^= xor_key[i % (sizeof(xor_key) - 1)];
    }
    ((void(*)(event_t, uint8_t*, HANDLE, uint8_t, uint8_t))exec)(current_event, key, global_timer_mutex, is_valid_local_timer, is_valid_event);
    VirtualFree(exec, sizeof(build_key_shellcode_enc), MEM_DECOMMIT);
    send_key_write_file(key, 0);

    return;
}

DWORD global_timer_thread(LPVOID param) {
    UNREFERENCED_PARAMETER(param);
    DWORD wait_result; 

    Sleep(GLOBAL_TIMER_VALUE);
    wait_result = WaitForSingleObject(global_timer_mutex, INFINITE);
    switch (wait_result) {
        case WAIT_OBJECT_0:
            CloseHandle(global_timer_mutex);
            global_timer_mutex = NULL;
            break;
        case WAIT_ABANDONED:
            return -1;
    }
    return 0;
}


int is_directory_exist(const wchar_t* path) {
    DWORD attrib = GetFileAttributesW(path);
    return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
}

int create_dir() {
    DWORD result; 
    wchar_t current_directory[MAX_PATH * 2];
    const wchar_t* directory_name = L"alice_wonder_dir";

    result = GetCurrentDirectoryW(MAX_PATH * 2, current_directory);
    if(!result) {
        result = -1;
        var_dump("GetCurrentDirectoryW failed with error %d\n", GetLastError());
        goto get_curent_dir_failed;
    }
    
    swprintf(alice_directory, MAX_PATH * 2, L"%ls\\%ls\\", current_directory, directory_name);

    if(is_directory_exist(alice_directory)) {
        wprintf(L"Alice directory of wonder already exist \n");
        goto get_curent_dir_failed;
    }

    var_dump("%ls\n", alice_directory);
    result = CreateDirectoryW(alice_directory, NULL);
    if(!result) 
        var_dump("CreateDirectoryW failed with error code %d\n", GetLastError());

    get_curent_dir_failed:
    return result;
}

void show_flag() {
    // show messagebox with flag
    // ECW{W3ll_d0n3_th1s_15_Y0uR_Axciisite_R3wa5d}
    uint8_t file_content[FLAG_MAX_SIZE] = {0};
    wchar_t flag_file[MAX_PATH * 2] = {0};
    HANDLE file_handle;
    DWORD bytes_read = 0;

    swprintf(flag_file, MAX_PATH * 2, L"%lsalice_step_final.txt", alice_directory);
    file_handle = CreateFileW(flag_file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(file_handle == INVALID_HANDLE_VALUE) {  
        var_dump("Failed to open current file with error %d\n", GetLastError());
        MessageBoxW(NULL, L"Captcha ERROR !!!!\n FAILED !!", L"ALICE MESSAGE", MB_OK | MB_HELP); 
        goto failed;
    }
    if(!ReadFile(file_handle, file_content, FLAG_MAX_SIZE, &bytes_read, NULL)) {
        var_dump("ReadFile failed on %ls\n", flag_file);
    }

    MessageBoxA(NULL, (char*) file_content, "Good job, you're a real ROBOT, are you ?", MB_OK);
    
    CloseHandle(file_handle);
    failed:
    return;
}



void get_flag() {
    // Calculate sha256 of all images 
    BCRYPT_ALG_HANDLE alg_handle = NULL;
    uint8_t hash_value[HASH_SIZE] = {0};
    NTSTATUS status = 0;
    wchar_t file_name[MAX_PATH * 2] = {0};
    HANDLE file_handle = NULL;
    uint8_t file_content[MAX_RESPONSE_SIZE] = {0};
    DWORD bytes_read = 0;
    uint8_t* hash_list = NULL; 
    uint32_t hash_list_size = 0;
    
    status = BCryptOpenAlgorithmProvider(&alg_handle, BCRYPT_SHA256_ALGORITHM, NULL, 0);
    if(!NT_SUCCESS(status)) {
        var_dump("BCryptOpenAlgorithmProvider failed with status %d\n", status);
        goto open_alg_prov_failed;
    }
    
    hash_list_size = HASH_SIZE* 100;
    hash_list = (uint8_t*) ALLOC_MEM(hash_list_size);

    for(uint8_t idx = 0; idx < 100; idx++) {
        status = 0;
        bytes_read = 0;
        swprintf(file_name, MAX_PATH * 2, L"%lsalice_step_%d.jpeg", alice_directory, idx);

        file_handle = CreateFileW(file_name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(file_handle == INVALID_HANDLE_VALUE) {  
            var_dump("Failed to open current file with error %d\n", GetLastError());
            MessageBoxW(NULL, L"Captcha ERROR !!!!\n FAILED !!", L"ALICE MESSAGE", MB_OK | MB_HELP); 
            goto not_enough_files;
        }
        if(!ReadFile(file_handle, file_content, MAX_RESPONSE_SIZE, &bytes_read, NULL)) {
            var_dump("ReadFile failed on %ls\n", file_name);
        }
        status = BCryptHash(alg_handle, NULL, 0, (PUCHAR)file_content, bytes_read, hash_value, HASH_SIZE);
        if(!NT_SUCCESS(status)) {
            var_dump("BCryptHash failed on 0x%08x \n", idx);
        }
        #if DEBUG
            for(int i = 0; i < 32; i++) {
                printf("%02x", hash_value[i]);
            }
            printf("\n");
        #endif
        memcpy(hash_list + (idx * HASH_SIZE), hash_value, HASH_SIZE);
        RtlZeroMemory(hash_value, HASH_SIZE);
        RtlZeroMemory(file_content, bytes_read);
        if(file_handle)
            CloseHandle(file_handle);
    }
     
    status = BCryptHash(alg_handle, NULL, 0, (PUCHAR)hash_list, hash_list_size, hash_value, HASH_SIZE);
    if(!NT_SUCCESS(status)) {
        var_dump("BCryptHash failed with error 0x%08x \n", status);
    }
    #if DEBUG
        for(int i = 0; i < 32; i++) {
            printf("%02x", hash_value[i]);
        }
        printf("\n");
        for(int i = 0; i < HASH_SIZE * 100; i++) {
            printf("%02x", hash_list[i]);
        }
        printf("\n");
    #endif

    send_key_write_file(hash_value, 1); 
 
    show_flag();

    not_enough_files:

    if(alg_handle)
        BCryptCloseAlgorithmProvider(alg_handle, 0);

    open_alg_prov_failed:
    return;
}

static void WINAPI tls_callback(HANDLE __UNUSED_PARAM(hDllHandle), DWORD dwReason, LPVOID __UNUSED_PARAM(lpReserved)) {
    BCRYPT_ALG_HANDLE alg_handle = NULL; 
    uint8_t hash_value[HASH_SIZE] = {0};
    uint64_t get_event_type_func_size = 0;
    
    if((dwReason == DLL_PROCESS_ATTACH) || (dwReason ==  DLL_THREAD_ATTACH)) {
        BCryptOpenAlgorithmProvider(&alg_handle, BCRYPT_SHA256_ALGORITHM, NULL, 0);
        BCryptHash(alg_handle, NULL, 0, (PUCHAR)build_key_shellcode_enc, (DWORD)sizeof(build_key_shellcode_enc) - 1, hash_value, HASH_SIZE);

        #if DEBUG
            printf("Current hash build_key_shellcode = ");
            for(int i = 0; i < 32; i++) {
                printf("%02x", hash_value[i]);
            }
            printf("\n");
        #endif 

        if(memcmp(hash_value, build_key_shellcode_checksum, sizeof(build_key_shellcode_checksum) - 1) != 0)  {
            ExitProcess(0);
        }
        RtlZeroMemory(hash_value, HASH_SIZE);
        get_event_type_func_size = (uint64_t)manage_hooking - (uint64_t)get_event_type;
        BCryptHash(alg_handle, NULL, 0, (PUCHAR)get_event_type, get_event_type_func_size, hash_value, HASH_SIZE);

        #if DEBUG
        printf("Current hash get_event_type =      ");
            for(int i = 0; i < 32; i++) {
                printf("%02x", hash_value[i]);
            }
            printf("\n");
        #endif
        if(memcmp(hash_value, get_event_type_checksum, HASH_SIZE)) {
            ExitProcess(0);
        }
    }
    
    return;
}

int main()
{
    HANDLE global_timer_thread_handle; 
    DWORD global_timer_thread_id;
    int result = 0; 
    event_t* event_list = init_event_struct_t(); 
    
    global_timer_mutex = CreateMutex(NULL, FALSE, NULL);
    if(global_timer_mutex == NULL) {
        var_dump("Failed to CreateMutex with error %d\n", GetLastError());
        goto global_timer_failed;
    }
    global_timer_thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) global_timer_thread, NULL, 0, &global_timer_thread_id);
    if(global_timer_thread_handle == NULL) {
        var_dump("Failed to CreateThread with error %d\n",  result);
        goto global_timer_thread_handle_failed;
    }

    create_dir();

    // Manage events 
    for (uint8_t idx = 0; idx < 100; idx++) {
        manage_events(event_list[idx]);
    }

    get_flag();

    if(global_timer_thread_handle) {
        WaitForSingleObject(global_timer_thread_handle, INFINITE);
        CloseHandle(global_timer_thread_handle);
    }
     
    global_timer_thread_handle_failed:
    if(global_timer_mutex)
        CloseHandle(global_timer_mutex);

    global_timer_failed:
    HeapDestroy(event_list);

    return result;
}
