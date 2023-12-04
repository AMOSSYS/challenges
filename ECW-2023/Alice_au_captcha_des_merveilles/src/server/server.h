#ifndef SERVER_H
#define SERVER_H

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "../obj/rc4.h"
#include "../includes/debug.h"

#include <stdint.h>
#include <http.h>
#include <winsock2.h>
#include <windows.h>
#include <stdint.h>
#include <winerror.h>
#include <stdio.h>
#include <winnt.h>

#include "resources.h"

#define ALLOC_MEM(cb) HeapAlloc(GetProcessHeap(), 0, (cb))
#define DEALLOC_MEM(which) HeapFree(GetProcessHeap(), 0, (which)) 

#define INITIALIZE_HTTP_RESPONSE( resp, status, reason )    \
    do                                                      \
    {                                                       \
        RtlZeroMemory( (resp), sizeof(*(resp)) );           \
        (resp)->StatusCode = (status);                      \
        (resp)->pReason = (reason);                         \
        (resp)->ReasonLength = (USHORT) strlen(reason);     \
    } while (FALSE)

#define ADD_KNOWN_HEADER(Response, HeaderId, RawValue)               \
    do                                                               \
    {                                                                \
        (Response).Headers.KnownHeaders[(HeaderId)].pRawValue =      \
                                                          (RawValue);\
        (Response).Headers.KnownHeaders[(HeaderId)].RawValueLength = \
            (USHORT) strlen(RawValue);                               \
    } while(FALSE)

#define KEY_SIZE 32
#define FLAG_SIZE 29 
#define ENCRYPTED_FLAG "\x2a\x48\x63\x15\x43\x8a\xb1\xe4\xd8\x99\x66\x07\x95\xde\xaa\xd9\x25\xd9\xca\x97\x82\x41\x34\xaf\x6c\x3a\x6e\xa2\xea"
#define FLAG_MAX_SIZE 128

DWORD receive_requests(HANDLE handle_req_queue);



#endif

