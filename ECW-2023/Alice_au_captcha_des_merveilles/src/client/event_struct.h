#ifndef EVENT_STRUCT_H
#define EVENT_STRUCT_H

#include <windows.h>
#include <stdint.h>

#define RANDOM_DATA_SIZE 14
#define GLOBAL_TIMER_CODE_SIZE 6
#define LOCAL_TIMER_CODE_SIZE 6
#define FAKE_DATA_SIZE_0 5
#define FAKE_DATA_SIZE_1 8
#define FAKE_DATA_SIZE_2 2
#define EVENT_SIZE 4

typedef enum {
    ALL_EVENT, // 0
    LOST_MOUSE_EVENT, // 1 EVENT_SYSTEM_CAPTUREEND
    RECEIVE_MOUSE_EVENT, // 2 EVENT_SYSTEM_CAPTURESTART
    FOREGROUND_WINDOW_EVENT, // 3 EVENT_SYSTEM_FOREGROUND 
    MOVE_WINDOW_EVENT, // 4 EVENT_SYSTEM_MOVESIZESTART
    OBJECT_CREATE_EVENT, // 5 EVENT_OBJECT_CREATE
    OBJECT_DESTROY_EVENT, // 6 EVENT_OBJECT_DESTROY
    OBJECT_FOCUS_EVENT,// 7 EVENT_OBJECT_FOCUS
    MINIMIZE_WINDOW_START_EVENT, // 8 EVENT_SYSTEM_MINIMIZESTART
    MINIMIZE_WINDOW_END_EVENT,// 9 EVENT_SYSTEM_MINIMIZEEND
    MENU_SELECT_START,// 10 EVENT_SYSTEM_MENUSTART 
    MENU_CONTEXT_START, // 11 EVENT_SYSTEM_MENUPOPUPSTART                         
} event_type_t;

typedef struct {
    uint8_t id; 
    event_type_t event_type;
    uint8_t fake_data_code_0[FAKE_DATA_SIZE_0];
    uint8_t valid_global_timer_code[GLOBAL_TIMER_CODE_SIZE];
    uint8_t invalid_global_timer_code[GLOBAL_TIMER_CODE_SIZE];
    uint8_t valid_local_timer_code[LOCAL_TIMER_CODE_SIZE];
    uint8_t fake_data_code_1[FAKE_DATA_SIZE_1];
    uint8_t invalid_local_timer_code[LOCAL_TIMER_CODE_SIZE];
    uint8_t valid_event[EVENT_SIZE];
    uint8_t invalid_event[EVENT_SIZE];
    uint8_t fake_data_code_2[FAKE_DATA_SIZE_2];
    uint8_t random_data[RANDOM_DATA_SIZE];
} event_t; 

event_t* init_event_struct_t() {
    event_t* event_list;
    event_list = (event_t*) HeapAlloc(GetProcessHeap(), 0, sizeof(event_t) * 100);
     
    event_list[0].id = 0; 
    event_list[0].event_type = LOST_MOUSE_EVENT;
    memcpy(event_list[0].valid_global_timer_code, "\xdd\xdf\x43\x24\x1e\x6c", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[0].invalid_global_timer_code, "\x2e\xa0\x85\x47\xc7\x49", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[0].valid_local_timer_code, "\xa7\x6b\x4a\x45\xe0\xaf", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[0].fake_data_code_1, "\xf6\x27\x19\xfa\x10\x1c\x42\xf5", FAKE_DATA_SIZE_1);
    memcpy(event_list[0].invalid_local_timer_code, "\x39\x56\xae\x52\xd7\xd8", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[0].valid_event, "\xfe\x11\xf6\x4e", EVENT_SIZE);
    memcpy(event_list[0].invalid_event, "\x12\xc1\x81\x94", EVENT_SIZE);
    memcpy(event_list[0].random_data, "\x48\x04\xb1\xbf\x15\x43\xe4\x0c\x3a\xda\x47\x49\x87\x5b", RANDOM_DATA_SIZE);
     
    event_list[1].id = 1; 
    event_list[1].event_type = RECEIVE_MOUSE_EVENT;
    memcpy(event_list[1].fake_data_code_0, "\x38\x17\xb8\xf0\xc1", FAKE_DATA_SIZE_0);
    memcpy(event_list[1].valid_global_timer_code, "\x01\x8e\x27\xbc\x71\xb9", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[1].invalid_global_timer_code, "\x0a\x41\x71\x7a\xae\x1a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[1].valid_local_timer_code, "\xa7\x01\xe8\xf6\xc5\xa1", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[1].invalid_local_timer_code, "\xe4\xde\x9b\x61\x64\x5b", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[1].valid_event, "\xf0\xec\x6f\x15", EVENT_SIZE);
    memcpy(event_list[1].invalid_event, "\x65\xbc\xc9\xca", EVENT_SIZE);
    memcpy(event_list[1].random_data, "\xa0\x7d\xb7\x83\x3b\x55\xf0\x9b\xac\x3d\x24\x9a\x9a\x47", RANDOM_DATA_SIZE);
     
    event_list[2].id = 2; 
    event_list[2].event_type = FOREGROUND_WINDOW_EVENT;
    memcpy(event_list[2].valid_global_timer_code, "\x03\xad\x6b\xfd\xc6\x4f", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[2].invalid_global_timer_code, "\x73\x99\xb1\xde\xb5\x50", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[2].valid_local_timer_code, "\x97\x4a\x66\x47\xfb\xbc", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[2].invalid_local_timer_code, "\x0b\x3f\x0f\x65\x39\x08", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[2].valid_event, "\x79\x4c\x22\xd1", EVENT_SIZE);
    memcpy(event_list[2].invalid_event, "\xfe\xce\x43\x76", EVENT_SIZE);
    memcpy(event_list[2].random_data, "\xff\xf1\x71\x4b\x36\x69\x87\x87\x1d\x76\xbc\x4a\x5d\x62", RANDOM_DATA_SIZE);
     
    event_list[3].id = 3; 
    event_list[3].event_type = MENU_CONTEXT_START;
    memcpy(event_list[3].valid_global_timer_code, "\x32\x7a\x6d\x9c\xc6\xb3", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[3].invalid_global_timer_code, "\x57\xb7\xab\x39\x1d\xf6", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[3].valid_local_timer_code, "\xf9\xc2\xa4\xad\x13\xcd", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[3].invalid_local_timer_code, "\x6f\x7d\xe2\xbc\xaa\x1e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[3].valid_event, "\x2a\xb7\x60\x85", EVENT_SIZE);
    memcpy(event_list[3].invalid_event, "\x2a\x31\x85\x3a", EVENT_SIZE);
    memcpy(event_list[3].fake_data_code_2, "\x40\x71\x1c\x81", FAKE_DATA_SIZE_2);
    memcpy(event_list[3].random_data, "\x35\xcf\x39\xf6\xf6\x24\x2a\x69\xca\x2b\xcc\x33\xd9\x58", RANDOM_DATA_SIZE);
     
    event_list[4].id = 4; 
    event_list[4].event_type = MENU_CONTEXT_START;
    memcpy(event_list[4].valid_global_timer_code, "\x9a\x82\x9d\x5b\xb7\x9b", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[4].invalid_global_timer_code, "\x45\x6d\x10\xcb\x28\x21", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[4].valid_local_timer_code, "\x05\x41\x01\x29\xc5\x72", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[4].fake_data_code_1, "\x96\xdb\xfb\x9b\xd9\x9e\xa7\x49", FAKE_DATA_SIZE_1);
    memcpy(event_list[4].invalid_local_timer_code, "\x6d\x14\xa2\x56\x12\xb6", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[4].valid_event, "\x2d\xf5\xce\x1f", EVENT_SIZE);
    memcpy(event_list[4].invalid_event, "\x8e\x15\xc2\x83", EVENT_SIZE);
    memcpy(event_list[4].random_data, "\xd8\x13\x24\x0d\x6d\x1b\x62\x6a\x0c\xe7\xb6\xc5\xe4\x90", RANDOM_DATA_SIZE);
     
    event_list[5].id = 5; 
    event_list[5].event_type = MENU_SELECT_START;
    memcpy(event_list[5].fake_data_code_0, "\x3d\xd5\x6d\x93\x38", FAKE_DATA_SIZE_0);
    memcpy(event_list[5].valid_global_timer_code, "\x6d\xb1\xc2\x0e\x75\x62", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[5].invalid_global_timer_code, "\xd0\x9a\xcf\xb7\xd8\x9f", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[5].valid_local_timer_code, "\x10\xbc\x6a\x44\x67\x69", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[5].invalid_local_timer_code, "\x35\xc8\x66\x2e\x34\xe8", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[5].valid_event, "\x51\x39\x91\x54", EVENT_SIZE);
    memcpy(event_list[5].invalid_event, "\xac\x43\xd1\x90", EVENT_SIZE);
    memcpy(event_list[5].random_data, "\xda\xc0\xd6\xea\xa6\x92\xd5\x24\x20\x03\xbe\x83\x61\x67", RANDOM_DATA_SIZE);
     
    event_list[6].id = 6; 
    event_list[6].event_type = OBJECT_CREATE_EVENT;
    memcpy(event_list[6].fake_data_code_0, "\xce\xf7\x9b\x5a\xe9", FAKE_DATA_SIZE_0);
    memcpy(event_list[6].valid_global_timer_code, "\x7f\x8f\x8d\xf9\xe9\x2a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[6].invalid_global_timer_code, "\x44\x99\x33\x09\xf9\xc0", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[6].valid_local_timer_code, "\x2a\xb1\x6e\xcd\xf6\xac", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[6].fake_data_code_1, "\x41\x01\xfc\x8a\x47\x58\x21\x77", FAKE_DATA_SIZE_1);
    memcpy(event_list[6].invalid_local_timer_code, "\xf0\x39\x7d\xc1\x2a\xa3", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[6].valid_event, "\x04\xd8\x03\xe8", EVENT_SIZE);
    memcpy(event_list[6].invalid_event, "\x72\x8f\xde\x3b", EVENT_SIZE);
    memcpy(event_list[6].fake_data_code_2, "\x4f\x22\x30\x90", FAKE_DATA_SIZE_2);
    memcpy(event_list[6].random_data, "\x79\x94\x9e\x29\xe5\x12\xe5\x24\x29\xb5\xf4\x52\x79\x1e", RANDOM_DATA_SIZE);
     
    event_list[7].id = 7; 
    event_list[7].event_type = OBJECT_DESTROY_EVENT;
    memcpy(event_list[7].valid_global_timer_code, "\x81\x4c\x77\x2c\x6c\x1d", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[7].invalid_global_timer_code, "\x23\x14\xdb\x2c\x5f\x25", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[7].valid_local_timer_code, "\x3a\x7d\xfe\xdb\x9e\x67", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[7].fake_data_code_1, "\xbf\xe3\x12\x13\x41\x1f\x11\x02", FAKE_DATA_SIZE_1);
    memcpy(event_list[7].invalid_local_timer_code, "\x99\xd4\x1e\xf9\x80\x5f", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[7].valid_event, "\x2b\x57\x0e\xae", EVENT_SIZE);
    memcpy(event_list[7].invalid_event, "\x26\x0a\x26\xc4", EVENT_SIZE);
    memcpy(event_list[7].random_data, "\x74\xd7\xfd\xe8\x22\x61\x01\x32\xfa\xb6\x71\x23\x24\x8a", RANDOM_DATA_SIZE);
     
    event_list[8].id = 8; 
    event_list[8].event_type = OBJECT_FOCUS_EVENT;
    memcpy(event_list[8].valid_global_timer_code, "\xe1\x76\x68\xa5\xba\xa3", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[8].invalid_global_timer_code, "\x25\xda\x2e\x7b\xc5\x1a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[8].valid_local_timer_code, "\x50\x1f\xe3\xfd\x56\x20", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[8].invalid_local_timer_code, "\x84\x3d\xac\x47\xe3\x9e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[8].valid_event, "\xd7\x13\x35\x61", EVENT_SIZE);
    memcpy(event_list[8].invalid_event, "\x98\x1e\xbe\x24", EVENT_SIZE);
    memcpy(event_list[8].random_data, "\x02\xda\xfd\xc4\x24\x51\xa4\x3a\xcf\x4e\xad\x0e\x62\x6b", RANDOM_DATA_SIZE);
     
    event_list[9].id = 9; 
    event_list[9].event_type = MINIMIZE_WINDOW_START_EVENT;
    memcpy(event_list[9].fake_data_code_0, "\xd8\x6e\xb7\x8a\xb1", FAKE_DATA_SIZE_0);
    memcpy(event_list[9].valid_global_timer_code, "\xb5\xab\x46\xac\x20\x67", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[9].invalid_global_timer_code, "\xfc\x5a\x0f\x9d\xca\x50", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[9].valid_local_timer_code, "\x81\xe1\xc2\x9a\x43\x6b", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[9].invalid_local_timer_code, "\x39\x95\x17\x14\x71\xdb", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[9].valid_event, "\x52\x82\x9b\xb8", EVENT_SIZE);
    memcpy(event_list[9].invalid_event, "\x02\x82\xab\x68", EVENT_SIZE);
    memcpy(event_list[9].fake_data_code_2, "\xaa\x3b\x77\xfd", FAKE_DATA_SIZE_2);
    memcpy(event_list[9].random_data, "\x3a\x8f\x46\x60\x73\x68\xb5\x13\xfd\xb7\x73\xfe\x83\xfc", RANDOM_DATA_SIZE);
     
    event_list[10].id = 10; 
    event_list[10].event_type = LOST_MOUSE_EVENT;
    memcpy(event_list[10].valid_global_timer_code, "\xdc\xd6\x2a\x54\x0a\x37", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[10].invalid_global_timer_code, "\x7d\x67\xb6\xd8\x54\xf0", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[10].valid_local_timer_code, "\x05\x96\x7c\x40\x8b\xaa", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[10].fake_data_code_1, "\x9c\x99\x7b\x37\x85\xa1\xaa\x39", FAKE_DATA_SIZE_1);
    memcpy(event_list[10].invalid_local_timer_code, "\x1c\x27\x3d\xae\x6a\x8e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[10].valid_event, "\xce\xaa\xf5\x6e", EVENT_SIZE);
    memcpy(event_list[10].invalid_event, "\xe0\x0a\xfe\xfc", EVENT_SIZE);
    memcpy(event_list[10].random_data, "\xa6\xc2\x22\xf1\x5e\x5c\x55\xce\x28\xd3\x0c\xf2\x0f\xd8", RANDOM_DATA_SIZE);
     
    event_list[11].id = 11; 
    event_list[11].event_type = RECEIVE_MOUSE_EVENT;
    memcpy(event_list[11].valid_global_timer_code, "\x3a\x40\xbd\xbd\x62\xca", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[11].invalid_global_timer_code, "\xc2\x88\x00\x43\x85\x59", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[11].valid_local_timer_code, "\xd5\xac\x52\x69\x87\xa2", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[11].fake_data_code_1, "\xc9\x3e\xd6\xab\x31\x1e\x2e\xa0", FAKE_DATA_SIZE_1);
    memcpy(event_list[11].invalid_local_timer_code, "\x20\x9c\xcc\x83\x19\xe5", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[11].valid_event, "\x8e\x7b\x31\xb9", EVENT_SIZE);
    memcpy(event_list[11].invalid_event, "\xe9\x58\xf0\xca", EVENT_SIZE);
    memcpy(event_list[11].random_data, "\x25\xfb\x3a\xc5\xe3\x17\x60\x57\x06\xbb\xc6\xff\x41\x01", RANDOM_DATA_SIZE);
     
    event_list[12].id = 12; 
    event_list[12].event_type = FOREGROUND_WINDOW_EVENT;
    memcpy(event_list[12].valid_global_timer_code, "\x4c\xab\x36\x56\x95\x99", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[12].invalid_global_timer_code, "\xe9\x46\xac\x0e\x02\x2e", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[12].valid_local_timer_code, "\x48\x7c\x54\x65\xac\x16", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[12].invalid_local_timer_code, "\xd8\x10\xe2\x38\x37\x02", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[12].valid_event, "\x57\x3a\xf4\x7b", EVENT_SIZE);
    memcpy(event_list[12].invalid_event, "\x40\x3b\xd7\xfe", EVENT_SIZE);
    memcpy(event_list[12].random_data, "\x22\x98\x5c\xa7\xda\x6b\x0c\xf3\xe2\x33\xf2\xec\x50\x29", RANDOM_DATA_SIZE);
     
    event_list[13].id = 13; 
    event_list[13].event_type = MENU_CONTEXT_START;
    memcpy(event_list[13].valid_global_timer_code, "\x7f\xca\x7b\x53\x3b\x6d", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[13].invalid_global_timer_code, "\x90\x53\xd5\xc5\x59\x92", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[13].valid_local_timer_code, "\x42\xfd\xa3\x06\x6d\x93", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[13].fake_data_code_1, "\x05\x53\x79\xd3\x84\x1c\x0a\x4b", FAKE_DATA_SIZE_1);
    memcpy(event_list[13].invalid_local_timer_code, "\xf4\x52\xa7\x77\x7e\x64", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[13].valid_event, "\xca\xd0\x54\xd4", EVENT_SIZE);
    memcpy(event_list[13].invalid_event, "\xc0\x35\x9e\x98", EVENT_SIZE);
    memcpy(event_list[13].random_data, "\x9b\xc5\x9f\x64\x49\x00\x4c\xbb\xf3\xbd\x71\xad\xdc\x18", RANDOM_DATA_SIZE);
     
    event_list[14].id = 14; 
    event_list[14].event_type = MENU_CONTEXT_START;
    memcpy(event_list[14].fake_data_code_0, "\xd9\xdf\x58\x20\x8c", FAKE_DATA_SIZE_0);
    memcpy(event_list[14].valid_global_timer_code, "\x83\x7c\x7e\xe5\x8a\x3e", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[14].invalid_global_timer_code, "\x60\x81\x12\x3a\xda\x67", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[14].valid_local_timer_code, "\x3a\xad\x22\xd8\xdd\xa0", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[14].fake_data_code_1, "\x51\x5d\xe5\x07\xec\xa9\x5c\xee", FAKE_DATA_SIZE_1);
    memcpy(event_list[14].invalid_local_timer_code, "\x56\x6a\x6c\xcb\xdb\xe2", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[14].valid_event, "\xf2\xeb\xc1\xb2", EVENT_SIZE);
    memcpy(event_list[14].invalid_event, "\x4d\x83\xfa\xcd", EVENT_SIZE);
    memcpy(event_list[14].fake_data_code_2, "\xf9\x52\x86\x4b", FAKE_DATA_SIZE_2);
    memcpy(event_list[14].random_data, "\xcd\x68\x38\xb7\xe9\xc8\xa7\x7a\x9a\xeb\xeb\xeb\xc5\xe1", RANDOM_DATA_SIZE);
     
    event_list[15].id = 15; 
    event_list[15].event_type = MENU_SELECT_START;
    memcpy(event_list[15].valid_global_timer_code, "\x7d\x73\xaa\xf8\x71\x47", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[15].invalid_global_timer_code, "\x10\x87\xfd\x4c\x50\xf8", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[15].valid_local_timer_code, "\x36\xbf\x85\x02\x8e\x09", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[15].fake_data_code_1, "\x49\xa5\x2a\x16\x2c\xe3\xd0\x27", FAKE_DATA_SIZE_1);
    memcpy(event_list[15].invalid_local_timer_code, "\x45\x0a\x30\x1c\x87\x96", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[15].valid_event, "\x1d\x69\xd1\x83", EVENT_SIZE);
    memcpy(event_list[15].invalid_event, "\xb5\x3e\x1c\xa2", EVENT_SIZE);
    memcpy(event_list[15].fake_data_code_2, "\xac\x4a\xdb\x99", FAKE_DATA_SIZE_2);
    memcpy(event_list[15].random_data, "\x8c\x23\xfe\xd5\xed\xcc\x3b\x93\x07\x14\x3d\xa9\xbc\xbf", RANDOM_DATA_SIZE);
     
    event_list[16].id = 16; 
    event_list[16].event_type = OBJECT_CREATE_EVENT;
    memcpy(event_list[16].valid_global_timer_code, "\xb2\x36\xc6\x09\x6b\xfd", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[16].invalid_global_timer_code, "\x45\x51\xd5\x25\x2d\x68", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[16].valid_local_timer_code, "\x65\xfe\x86\x28\x1d\xfc", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[16].invalid_local_timer_code, "\xb0\x43\x45\xbb\xcf\xf3", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[16].valid_event, "\xba\x8c\x3a\xf5", EVENT_SIZE);
    memcpy(event_list[16].invalid_event, "\xd9\xfe\xd9\xd8", EVENT_SIZE);
    memcpy(event_list[16].random_data, "\xd5\x06\xce\x77\xad\x0d\x6d\x7c\x91\x1d\x7d\xf1\x19\x9d", RANDOM_DATA_SIZE);
     
    event_list[17].id = 17; 
    event_list[17].event_type = OBJECT_DESTROY_EVENT;
    memcpy(event_list[17].valid_global_timer_code, "\xf9\x42\xea\x7a\x54\x2c", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[17].invalid_global_timer_code, "\xb2\x71\x80\x37\xb9\xa3", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[17].valid_local_timer_code, "\x0f\xeb\x84\x8c\xae\x23", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[17].invalid_local_timer_code, "\x95\xeb\x1a\xd8\xa3\xff", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[17].valid_event, "\x69\xf9\x4e\x86", EVENT_SIZE);
    memcpy(event_list[17].invalid_event, "\xa2\x8a\xf4\xfd", EVENT_SIZE);
    memcpy(event_list[17].random_data, "\xd7\x22\x4b\x10\x40\x28\xfa\xc7\x46\x1f\xc2\x79\x5c\x82", RANDOM_DATA_SIZE);
     
    event_list[18].id = 18; 
    event_list[18].event_type = OBJECT_FOCUS_EVENT;
    memcpy(event_list[18].valid_global_timer_code, "\xca\x76\xc3\x0e\xa2\x25", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[18].invalid_global_timer_code, "\x68\x60\x66\x77\x55\xc2", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[18].valid_local_timer_code, "\x37\xe4\x1f\x84\x4a\x6e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[18].fake_data_code_1, "\xde\x67\xde\x10\x0d\x7c\x0f\xc9", FAKE_DATA_SIZE_1);
    memcpy(event_list[18].invalid_local_timer_code, "\x21\xcc\x83\xbc\xf4\xd2", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[18].valid_event, "\x06\x3f\x65\x85", EVENT_SIZE);
    memcpy(event_list[18].invalid_event, "\x37\x6f\xd6\x08", EVENT_SIZE);
    memcpy(event_list[18].random_data, "\x4f\x29\x06\x26\x49\xe3\x24\x3f\xf7\xa6\x54\x5d\xf2\x55", RANDOM_DATA_SIZE);
     
    event_list[19].id = 19; 
    event_list[19].event_type = MINIMIZE_WINDOW_START_EVENT;
    memcpy(event_list[19].fake_data_code_0, "\xaf\x68\x7b\xcd\xaf", FAKE_DATA_SIZE_0);
    memcpy(event_list[19].valid_global_timer_code, "\xa4\xef\x0f\xed\x4a\xb6", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[19].invalid_global_timer_code, "\x8d\xe9\x81\xfa\xa6\x85", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[19].valid_local_timer_code, "\x4b\x16\xf9\x8d\x12\x6d", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[19].invalid_local_timer_code, "\x63\xc7\xd6\x83\x25\x0f", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[19].valid_event, "\xb0\xdc\x0d\xa9", EVENT_SIZE);
    memcpy(event_list[19].invalid_event, "\x12\x98\x68\x26", EVENT_SIZE);
    memcpy(event_list[19].random_data, "\x07\xd9\x77\x33\x1e\x7b\x72\x80\xf7\x76\xf7\x3b\xf0\xea", RANDOM_DATA_SIZE);
     
    event_list[20].id = 20; 
    event_list[20].event_type = LOST_MOUSE_EVENT;
    memcpy(event_list[20].fake_data_code_0, "\x2f\xa3\x50\x5f\xd1", FAKE_DATA_SIZE_0);
    memcpy(event_list[20].valid_global_timer_code, "\x2c\x2a\xb4\x5a\x72\x8b", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[20].invalid_global_timer_code, "\xcc\x84\x8b\xdd\x0a\xae", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[20].valid_local_timer_code, "\xbe\xb0\x7e\x0c\xef\x63", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[20].fake_data_code_1, "\x34\x71\xac\xd3\x2d\x7f\xd9\x68", FAKE_DATA_SIZE_1);
    memcpy(event_list[20].invalid_local_timer_code, "\x39\xbf\x6f\xd2\x34\xc2", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[20].valid_event, "\x69\x07\x1a\x5d", EVENT_SIZE);
    memcpy(event_list[20].invalid_event, "\x0e\x03\xee\xe5", EVENT_SIZE);
    memcpy(event_list[20].fake_data_code_2, "\xdb\x48\x23\x7a", FAKE_DATA_SIZE_2);
    memcpy(event_list[20].random_data, "\x97\xc5\x70\x70\x59\xd6\xe7\xf4\x65\xa5\x68\x12\x07\xb6", RANDOM_DATA_SIZE);
     
    event_list[21].id = 21; 
    event_list[21].event_type = RECEIVE_MOUSE_EVENT;
    memcpy(event_list[21].fake_data_code_0, "\x33\x19\x55\x16\xaf", FAKE_DATA_SIZE_0);
    memcpy(event_list[21].valid_global_timer_code, "\x66\x5b\xb4\x57\x74\x5f", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[21].invalid_global_timer_code, "\x40\xe1\x59\x03\xc5\x9b", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[21].valid_local_timer_code, "\xd5\x1a\xbf\xc2\x0a\x5a", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[21].fake_data_code_1, "\xf6\x67\x74\xcd\x02\x45\x34\x64", FAKE_DATA_SIZE_1);
    memcpy(event_list[21].invalid_local_timer_code, "\x23\x55\xaa\x72\x0c\xcd", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[21].valid_event, "\x4a\x14\x90\x8b", EVENT_SIZE);
    memcpy(event_list[21].invalid_event, "\x37\x6a\xeb\x54", EVENT_SIZE);
    memcpy(event_list[21].random_data, "\x3d\x49\x1c\x44\xd2\xc2\xb0\x38\xd3\xd7\x55\x25\x9c\x20", RANDOM_DATA_SIZE);
     
    event_list[22].id = 22; 
    event_list[22].event_type = FOREGROUND_WINDOW_EVENT;
    memcpy(event_list[22].valid_global_timer_code, "\xa6\xcf\x00\xbe\x4d\xc3", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[22].invalid_global_timer_code, "\x52\xc2\x7c\x31\x8c\x0e", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[22].valid_local_timer_code, "\x54\x78\xc6\xd8\xa2\x26", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[22].invalid_local_timer_code, "\xdd\x30\x55\x9c\x33\x51", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[22].valid_event, "\x33\x73\xa2\x5b", EVENT_SIZE);
    memcpy(event_list[22].invalid_event, "\x0f\xce\x0f\x3d", EVENT_SIZE);
    memcpy(event_list[22].fake_data_code_2, "\xdf\xef\x0f\x94", FAKE_DATA_SIZE_2);
    memcpy(event_list[22].random_data, "\x9e\x75\xbc\x53\x1e\xae\xdf\xc5\x19\x81\x7f\x00\xfa\x45", RANDOM_DATA_SIZE);
     
    event_list[23].id = 23; 
    event_list[23].event_type = MENU_CONTEXT_START;
    memcpy(event_list[23].valid_global_timer_code, "\xf2\x03\x25\xfa\x74\x5c", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[23].invalid_global_timer_code, "\x0c\xda\x1e\xad\xf6\xf4", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[23].valid_local_timer_code, "\x9c\xda\xad\x01\x43\x53", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[23].fake_data_code_1, "\x4d\xa0\xa2\x4e\x91\xd4\x43\xff", FAKE_DATA_SIZE_1);
    memcpy(event_list[23].invalid_local_timer_code, "\x6b\xa1\x32\x64\xec\x08", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[23].valid_event, "\xb4\x4f\xe3\x45", EVENT_SIZE);
    memcpy(event_list[23].invalid_event, "\x88\x07\xe7\x71", EVENT_SIZE);
    memcpy(event_list[23].random_data, "\xf6\x7a\xce\x43\x6e\x12\xaa\xd2\x59\x6c\x49\x18\xf8\xf3", RANDOM_DATA_SIZE);
     
    event_list[24].id = 24; 
    event_list[24].event_type = MENU_CONTEXT_START;
    memcpy(event_list[24].fake_data_code_0, "\x6a\xc7\x44\x4e\x5f", FAKE_DATA_SIZE_0);
    memcpy(event_list[24].valid_global_timer_code, "\xf1\xdd\x75\x97\x17\x80", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[24].invalid_global_timer_code, "\x1f\x32\xbd\xff\x30\xda", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[24].valid_local_timer_code, "\xf8\x90\xac\x42\x02\x7b", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[24].invalid_local_timer_code, "\x5b\xe4\x79\xec\x83\xed", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[24].valid_event, "\x1d\xe0\x95\x87", EVENT_SIZE);
    memcpy(event_list[24].invalid_event, "\x4e\x0d\xf8\x78", EVENT_SIZE);
    memcpy(event_list[24].random_data, "\xd0\xe9\xda\x1a\x50\x3d\x06\xc0\x58\xc7\xaf\x74\x42\x64", RANDOM_DATA_SIZE);
     
    event_list[25].id = 25; 
    event_list[25].event_type = MENU_SELECT_START;
    memcpy(event_list[25].valid_global_timer_code, "\x08\x42\xe5\xbc\x31\x86", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[25].invalid_global_timer_code, "\x31\x2a\xe5\x02\xaf\x4d", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[25].valid_local_timer_code, "\x76\x21\x5a\xba\x38\x8f", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[25].invalid_local_timer_code, "\x78\x0c\x96\x74\x97\x09", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[25].valid_event, "\x9f\x49\xf6\xce", EVENT_SIZE);
    memcpy(event_list[25].invalid_event, "\x8c\x2e\x40\x63", EVENT_SIZE);
    memcpy(event_list[25].random_data, "\x77\x2b\x78\x03\xb3\x4d\xba\xbc\xb7\x16\x58\xc8\x6d\xab", RANDOM_DATA_SIZE);
     
    event_list[26].id = 26; 
    event_list[26].event_type = OBJECT_CREATE_EVENT;
    memcpy(event_list[26].valid_global_timer_code, "\xbc\x76\xfe\xf9\x37\x33", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[26].invalid_global_timer_code, "\x02\x1c\xd8\xfa\x8e\xaa", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[26].valid_local_timer_code, "\xd4\x07\x32\xae\xe4\x4a", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[26].invalid_local_timer_code, "\x6b\xa6\x9b\x20\x5d\x79", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[26].valid_event, "\x73\x00\x42\x94", EVENT_SIZE);
    memcpy(event_list[26].invalid_event, "\x17\xbd\xc9\xe8", EVENT_SIZE);
    memcpy(event_list[26].random_data, "\xbf\xaa\x34\x27\x33\x17\x39\x43\x33\x48\x38\x4e\x6f\xae", RANDOM_DATA_SIZE);
     
    event_list[27].id = 27; 
    event_list[27].event_type = OBJECT_DESTROY_EVENT;
    memcpy(event_list[27].valid_global_timer_code, "\x78\x74\xdb\xf6\x57\xa9", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[27].invalid_global_timer_code, "\xd8\x72\x63\x92\x64\x1b", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[27].valid_local_timer_code, "\xda\xb0\xf4\x2a\x7a\xc9", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[27].invalid_local_timer_code, "\xc7\x98\x06\x83\x9c\x1c", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[27].valid_event, "\x08\x47\x46\xc1", EVENT_SIZE);
    memcpy(event_list[27].invalid_event, "\xc2\x73\xb9\x15", EVENT_SIZE);
    memcpy(event_list[27].fake_data_code_2, "\x9e\x29\x9f\x00", FAKE_DATA_SIZE_2);
    memcpy(event_list[27].random_data, "\xf4\xc0\x7b\x81\x9a\xb0\x63\x86\x80\x3e\xd6\x2c\x46\xcc", RANDOM_DATA_SIZE);
     
    event_list[28].id = 28; 
    event_list[28].event_type = OBJECT_FOCUS_EVENT;
    memcpy(event_list[28].fake_data_code_0, "\x45\xa3\x2e\x13\xf0", FAKE_DATA_SIZE_0);
    memcpy(event_list[28].valid_global_timer_code, "\x98\x80\xa3\xd4\xf7\x10", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[28].invalid_global_timer_code, "\xac\x60\x8c\xde\xe8\x24", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[28].valid_local_timer_code, "\x2a\xbd\x6f\x3f\x55\xc6", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[28].fake_data_code_1, "\x72\x13\x18\x2e\xbb\x68\x04\xd0", FAKE_DATA_SIZE_1);
    memcpy(event_list[28].invalid_local_timer_code, "\x53\xe3\x18\x41\xaa\xb2", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[28].valid_event, "\x85\x70\x47\xf4", EVENT_SIZE);
    memcpy(event_list[28].invalid_event, "\xf6\xf8\x2f\xed", EVENT_SIZE);
    memcpy(event_list[28].random_data, "\x7a\xd5\x20\x2f\x5e\xc6\x09\x0b\x8d\x9a\x32\x75\xc3\x0c", RANDOM_DATA_SIZE);
     
    event_list[29].id = 29; 
    event_list[29].event_type = MINIMIZE_WINDOW_START_EVENT;
    memcpy(event_list[29].valid_global_timer_code, "\x0c\x4b\x50\xcc\x2a\x9a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[29].invalid_global_timer_code, "\xf7\xa0\xde\x8d\x0a\x1e", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[29].valid_local_timer_code, "\x89\x33\x3a\xdc\xea\x6d", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[29].fake_data_code_1, "\x8f\x26\xa8\xbf\xc6\x0a\x48\x9c", FAKE_DATA_SIZE_1);
    memcpy(event_list[29].invalid_local_timer_code, "\xe1\xcd\x68\x43\x5d\x59", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[29].valid_event, "\x2e\xb2\x0d\x2d", EVENT_SIZE);
    memcpy(event_list[29].invalid_event, "\xf3\x7f\x7d\xf5", EVENT_SIZE);
    memcpy(event_list[29].random_data, "\xfc\x39\x23\x2d\xe7\x9c\x18\xc4\xde\x9a\xe5\x6e\xd7\xb5", RANDOM_DATA_SIZE);
     
    event_list[30].id = 30; 
    event_list[30].event_type = LOST_MOUSE_EVENT;
    memcpy(event_list[30].fake_data_code_0, "\x75\x80\x4e\x3f\x14", FAKE_DATA_SIZE_0);
    memcpy(event_list[30].valid_global_timer_code, "\xda\x33\x0a\xed\xb7\x0d", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[30].invalid_global_timer_code, "\x2a\xf8\x25\x07\x4e\x1a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[30].valid_local_timer_code, "\x00\x11\x68\xd9\x24\xa5", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[30].invalid_local_timer_code, "\xd4\x0b\xe2\xd6\x7b\x6e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[30].valid_event, "\x54\x65\x65\x72", EVENT_SIZE);
    memcpy(event_list[30].invalid_event, "\x03\x61\xa6\xdb", EVENT_SIZE);
    memcpy(event_list[30].random_data, "\x6c\x3e\x84\x92\xb6\x44\x49\xa2\x37\x42\xfe\x47\xb2\x1a", RANDOM_DATA_SIZE);
     
    event_list[31].id = 31; 
    event_list[31].event_type = RECEIVE_MOUSE_EVENT;
    memcpy(event_list[31].fake_data_code_0, "\x89\x06\x91\x23\x88", FAKE_DATA_SIZE_0);
    memcpy(event_list[31].valid_global_timer_code, "\xdc\x94\x3a\x9e\xdf\xc4", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[31].invalid_global_timer_code, "\x25\x29\x19\x8c\x2e\x5a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[31].valid_local_timer_code, "\xfe\xcb\x0d\x5e\xd7\xfd", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[31].fake_data_code_1, "\xde\x27\x6e\x63\x23\xbb\x85\x72", FAKE_DATA_SIZE_1);
    memcpy(event_list[31].invalid_local_timer_code, "\x89\xd9\x35\x2d\xa7\x29", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[31].valid_event, "\xd5\xb1\x28\xd6", EVENT_SIZE);
    memcpy(event_list[31].invalid_event, "\x34\xc9\x95\x29", EVENT_SIZE);
    memcpy(event_list[31].random_data, "\x52\x71\x88\xc9\xdf\x3a\x8c\xd6\x80\xa2\x83\xd5\x6f\x81", RANDOM_DATA_SIZE);
     
    event_list[32].id = 32; 
    event_list[32].event_type = FOREGROUND_WINDOW_EVENT;
    memcpy(event_list[32].fake_data_code_0, "\xe4\xc3\xde\x68\x9b", FAKE_DATA_SIZE_0);
    memcpy(event_list[32].valid_global_timer_code, "\x5d\x20\x39\xb1\xeb\x61", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[32].invalid_global_timer_code, "\x86\x52\xfc\x26\x95\xbb", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[32].valid_local_timer_code, "\x57\xc0\x5f\x16\x9e\xea", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[32].invalid_local_timer_code, "\x82\xde\x00\x2e\x1c\xb6", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[32].valid_event, "\x6f\x08\x9d\x29", EVENT_SIZE);
    memcpy(event_list[32].invalid_event, "\x90\x1b\x09\xbd", EVENT_SIZE);
    memcpy(event_list[32].random_data, "\x9f\x64\x27\xbb\x86\x9c\x16\x96\x82\x9b\xb9\x32\x53\xea", RANDOM_DATA_SIZE);
     
    event_list[33].id = 33; 
    event_list[33].event_type = MENU_CONTEXT_START;
    memcpy(event_list[33].fake_data_code_0, "\xdb\x26\xe3\x6a\xef", FAKE_DATA_SIZE_0);
    memcpy(event_list[33].valid_global_timer_code, "\xa8\xa4\xc1\x27\x3c\xbd", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[33].invalid_global_timer_code, "\xb7\x23\xde\x10\x72\x89", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[33].valid_local_timer_code, "\x64\x8b\xa2\x9a\x45\x12", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[33].fake_data_code_1, "\xbb\x49\xf8\xfc\x74\x2e\x23\xff", FAKE_DATA_SIZE_1);
    memcpy(event_list[33].invalid_local_timer_code, "\x00\x55\x53\x56\x37\x66", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[33].valid_event, "\xa8\x45\x53\xde", EVENT_SIZE);
    memcpy(event_list[33].invalid_event, "\x05\x11\x96\x7f", EVENT_SIZE);
    memcpy(event_list[33].random_data, "\xd2\x29\x4f\x4c\xdb\xa1\xd4\x28\xf5\x74\xd7\xca\x55\xd5", RANDOM_DATA_SIZE);
     
    event_list[34].id = 34; 
    event_list[34].event_type = MENU_CONTEXT_START;
    memcpy(event_list[34].valid_global_timer_code, "\xac\xd2\x47\xca\xc3\x22", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[34].invalid_global_timer_code, "\x01\xb0\xcc\x30\xfc\x24", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[34].valid_local_timer_code, "\xa6\x6e\x96\xb6\xbd\x60", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[34].fake_data_code_1, "\xbe\xad\xba\xce\x6b\x64\x8b\x5e", FAKE_DATA_SIZE_1);
    memcpy(event_list[34].invalid_local_timer_code, "\xf8\x8d\x8d\x14\xb7\x95", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[34].valid_event, "\xe6\x76\xd6\x11", EVENT_SIZE);
    memcpy(event_list[34].invalid_event, "\x41\xe6\x33\x02", EVENT_SIZE);
    memcpy(event_list[34].random_data, "\xba\xd6\x4c\x43\xa9\xa4\x45\x01\x8f\x23\xd4\xe5\x7c\x8c", RANDOM_DATA_SIZE);
     
    event_list[35].id = 35; 
    event_list[35].event_type = MENU_SELECT_START;
    memcpy(event_list[35].valid_global_timer_code, "\x2c\x58\x2c\xed\x7c\xce", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[35].invalid_global_timer_code, "\xb2\xe7\x3a\x72\x3b\x0a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[35].valid_local_timer_code, "\x3d\x39\x0e\x84\xe5\xa2", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[35].invalid_local_timer_code, "\xf8\xf2\x0d\xda\x8f\x63", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[35].valid_event, "\x66\xff\xe9\x48", EVENT_SIZE);
    memcpy(event_list[35].invalid_event, "\x6a\x9d\xf6\xb1", EVENT_SIZE);
    memcpy(event_list[35].random_data, "\xf5\x97\xaf\xd9\x62\x74\xa4\x43\xfc\xa8\xa3\x37\x83\x89", RANDOM_DATA_SIZE);
     
    event_list[36].id = 36; 
    event_list[36].event_type = OBJECT_CREATE_EVENT;
    memcpy(event_list[36].valid_global_timer_code, "\xe1\x4e\x56\xfa\x19\xb6", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[36].invalid_global_timer_code, "\x8d\x9f\x2a\xfd\x65\x5a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[36].valid_local_timer_code, "\xf8\x72\x3e\x6c\x11\x8e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[36].invalid_local_timer_code, "\x3b\xb4\xf5\x22\x84\xcc", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[36].valid_event, "\x72\xe3\x6c\x40", EVENT_SIZE);
    memcpy(event_list[36].invalid_event, "\x90\x03\x66\xc4", EVENT_SIZE);
    memcpy(event_list[36].random_data, "\x39\x99\xdd\x34\x4a\x32\x7e\x19\xff\x52\xf8\xd6\x89\x70", RANDOM_DATA_SIZE);
     
    event_list[37].id = 37; 
    event_list[37].event_type = OBJECT_DESTROY_EVENT;
    memcpy(event_list[37].valid_global_timer_code, "\x8b\xef\x1c\x7e\xf2\xf3", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[37].invalid_global_timer_code, "\x03\x38\x24\xd5\x9f\x31", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[37].valid_local_timer_code, "\x1b\x5d\x04\x69\xa6\x74", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[37].invalid_local_timer_code, "\x0f\xc1\x17\xdc\x36\xb0", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[37].valid_event, "\x7b\xde\x53\xd7", EVENT_SIZE);
    memcpy(event_list[37].invalid_event, "\xb5\xcd\xc8\x44", EVENT_SIZE);
    memcpy(event_list[37].random_data, "\x79\x30\xd2\xf6\x54\xfb\x67\x2f\x16\xd5\x3b\xd1\x71\xe6", RANDOM_DATA_SIZE);
     
    event_list[38].id = 38; 
    event_list[38].event_type = OBJECT_FOCUS_EVENT;
    memcpy(event_list[38].valid_global_timer_code, "\xdb\xf1\x7a\x9c\xc1\xbb", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[38].invalid_global_timer_code, "\x91\xe3\xcb\xdb\x18\xfe", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[38].valid_local_timer_code, "\xe4\xda\x4f\x10\x34\xac", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[38].invalid_local_timer_code, "\xaa\x27\xd5\x74\x9a\x59", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[38].valid_event, "\xf9\xb7\x0b\xa5", EVENT_SIZE);
    memcpy(event_list[38].invalid_event, "\x42\x0f\xcf\x65", EVENT_SIZE);
    memcpy(event_list[38].random_data, "\xde\xf4\xbd\x39\x17\xb1\x9d\x1d\x77\x0f\x57\xdb\x82\xe4", RANDOM_DATA_SIZE);
     
    event_list[39].id = 39; 
    event_list[39].event_type = MINIMIZE_WINDOW_START_EVENT;
    memcpy(event_list[39].fake_data_code_0, "\x75\x53\x67\x33\xe1", FAKE_DATA_SIZE_0);
    memcpy(event_list[39].valid_global_timer_code, "\xc1\x97\x8b\x44\xfb\x9d", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[39].invalid_global_timer_code, "\x20\xc2\x77\x0d\xb9\x20", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[39].valid_local_timer_code, "\xcd\xa6\x87\x4c\x4a\x1e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[39].invalid_local_timer_code, "\x36\x53\x53\xc5\x8e\x92", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[39].valid_event, "\xf7\xf9\x95\xd4", EVENT_SIZE);
    memcpy(event_list[39].invalid_event, "\xe1\x07\x48\x7a", EVENT_SIZE);
    memcpy(event_list[39].random_data, "\x85\x2f\x3c\x9e\x01\x82\x76\x8d\xae\xf6\xe9\x9e\x57\x8a", RANDOM_DATA_SIZE);
     
    event_list[40].id = 40; 
    event_list[40].event_type = LOST_MOUSE_EVENT;
    memcpy(event_list[40].valid_global_timer_code, "\xb2\xf7\x93\x4b\xf5\x46", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[40].invalid_global_timer_code, "\xc7\x56\x4e\xf0\xb6\x19", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[40].valid_local_timer_code, "\x39\x5a\xae\x95\x4a\x81", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[40].fake_data_code_1, "\x06\x47\xc3\xfd\x9a\x70\x37\xc1", FAKE_DATA_SIZE_1);
    memcpy(event_list[40].invalid_local_timer_code, "\x24\xc0\x54\x3a\xa8\xba", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[40].valid_event, "\xcd\x24\xa9\xc5", EVENT_SIZE);
    memcpy(event_list[40].invalid_event, "\x68\x2c\xcc\xe2", EVENT_SIZE);
    memcpy(event_list[40].fake_data_code_2, "\xc6\x6c\x0b\x96", FAKE_DATA_SIZE_2);
    memcpy(event_list[40].random_data, "\xd7\x7a\xd7\x13\xd7\x90\xc0\x88\x89\x59\x82\x51\x3a\x0a", RANDOM_DATA_SIZE);
     
    event_list[41].id = 41; 
    event_list[41].event_type = RECEIVE_MOUSE_EVENT;
    memcpy(event_list[41].fake_data_code_0, "\x13\xee\x0d\x73\xa2", FAKE_DATA_SIZE_0);
    memcpy(event_list[41].valid_global_timer_code, "\x0c\xa6\xb2\xfb\xd3\x86", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[41].invalid_global_timer_code, "\xae\x22\x03\x10\xc1\x4c", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[41].valid_local_timer_code, "\x9d\xdb\x9a\x47\xcf\x89", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[41].invalid_local_timer_code, "\x9a\xb2\xf8\x3c\x52\xf4", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[41].valid_event, "\xf0\x9c\x58\xa3", EVENT_SIZE);
    memcpy(event_list[41].invalid_event, "\xda\x33\x92\xb5", EVENT_SIZE);
    memcpy(event_list[41].random_data, "\xa2\x54\x91\xd2\xcf\xe4\x12\xf8\xf6\x47\xc0\x09\x5d\xb6", RANDOM_DATA_SIZE);
     
    event_list[42].id = 42; 
    event_list[42].event_type = FOREGROUND_WINDOW_EVENT;
    memcpy(event_list[42].fake_data_code_0, "\x1e\x4e\x06\xd6\x6e", FAKE_DATA_SIZE_0);
    memcpy(event_list[42].valid_global_timer_code, "\xdf\x8a\x86\xc1\x08\x76", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[42].invalid_global_timer_code, "\xb6\xba\x49\xea\x18\x9c", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[42].valid_local_timer_code, "\xbd\x4e\x74\xbe\xac\xf1", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[42].fake_data_code_1, "\xd8\x1f\xb0\x46\xa9\x79\xbb\xb5", FAKE_DATA_SIZE_1);
    memcpy(event_list[42].invalid_local_timer_code, "\xf6\xf7\x56\x91\xa4\x5c", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[42].valid_event, "\xd4\xa3\x4b\xab", EVENT_SIZE);
    memcpy(event_list[42].invalid_event, "\x8f\x88\x88\x77", EVENT_SIZE);
    memcpy(event_list[42].random_data, "\x59\x97\x11\xa4\x77\xb6\x00\x2c\x9f\x38\x22\x8d\x08\x6c", RANDOM_DATA_SIZE);
     
    event_list[43].id = 43; 
    event_list[43].event_type = MENU_CONTEXT_START;
    memcpy(event_list[43].fake_data_code_0, "\xb3\x15\xd8\x0b\x18", FAKE_DATA_SIZE_0);
    memcpy(event_list[43].valid_global_timer_code, "\xf0\xf0\x09\x59\x39\x30", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[43].invalid_global_timer_code, "\xcd\x98\x85\x71\x8b\x2b", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[43].valid_local_timer_code, "\x7c\x6b\x43\x0b\xe6\x27", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[43].invalid_local_timer_code, "\x51\xad\x87\x7a\xcf\x6d", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[43].valid_event, "\x17\x69\x8a\x60", EVENT_SIZE);
    memcpy(event_list[43].invalid_event, "\x70\x5f\x4b\x07", EVENT_SIZE);
    memcpy(event_list[43].fake_data_code_2, "\x65\x5d\x71\xbd", FAKE_DATA_SIZE_2);
    memcpy(event_list[43].random_data, "\xbb\xcd\x07\x16\xca\x2c\xec\x3a\x0a\xbf\x20\x96\x00\x9e", RANDOM_DATA_SIZE);
     
    event_list[44].id = 44; 
    event_list[44].event_type = MENU_CONTEXT_START;
    memcpy(event_list[44].fake_data_code_0, "\x38\x3f\x9d\x4e\xe6", FAKE_DATA_SIZE_0);
    memcpy(event_list[44].valid_global_timer_code, "\xb2\x92\xef\x4d\xd9\x62", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[44].invalid_global_timer_code, "\xfd\x23\x61\xa7\x6f\xfd", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[44].valid_local_timer_code, "\x1f\xe9\x45\x3a\x26\xa6", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[44].fake_data_code_1, "\x58\x93\x97\xa1\x78\xd5\x83\x38", FAKE_DATA_SIZE_1);
    memcpy(event_list[44].invalid_local_timer_code, "\xb4\xb8\x98\xa2\xa9\x63", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[44].valid_event, "\x9b\x72\x22\xbf", EVENT_SIZE);
    memcpy(event_list[44].invalid_event, "\xad\xf6\x35\xf2", EVENT_SIZE);
    memcpy(event_list[44].fake_data_code_2, "\xab\xfa\x95\x6a", FAKE_DATA_SIZE_2);
    memcpy(event_list[44].random_data, "\x52\x20\x92\xad\x1b\x9a\xa7\x21\xe0\x05\xcc\x22\xd8\x41", RANDOM_DATA_SIZE);
     
    event_list[45].id = 45; 
    event_list[45].event_type = MENU_SELECT_START;
    memcpy(event_list[45].fake_data_code_0, "\x28\x04\x14\x20\xd7", FAKE_DATA_SIZE_0);
    memcpy(event_list[45].valid_global_timer_code, "\x20\x2c\xd1\xdf\xc4\xcc", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[45].invalid_global_timer_code, "\xe4\x63\xf1\x10\xef\x9a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[45].valid_local_timer_code, "\x39\xd6\xa5\xcd\x94\x22", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[45].fake_data_code_1, "\x41\x61\xe5\x1d\xe7\xc3\xeb\x6a", FAKE_DATA_SIZE_1);
    memcpy(event_list[45].invalid_local_timer_code, "\x24\x39\x59\x6a\x21\x08", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[45].valid_event, "\x18\x92\x32\x11", EVENT_SIZE);
    memcpy(event_list[45].invalid_event, "\x03\xa9\x38\xb7", EVENT_SIZE);
    memcpy(event_list[45].random_data, "\xa1\x59\xde\x1d\x54\xc5\x09\x2d\xb5\x48\xd7\x5c\xeb\xbe", RANDOM_DATA_SIZE);
     
    event_list[46].id = 46; 
    event_list[46].event_type = OBJECT_CREATE_EVENT;
    memcpy(event_list[46].fake_data_code_0, "\x50\xd8\xb2\x09\xf0", FAKE_DATA_SIZE_0);
    memcpy(event_list[46].valid_global_timer_code, "\x94\x09\xfb\xe7\x47\x2d", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[46].invalid_global_timer_code, "\xd4\xce\x03\x57\x60\x6a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[46].valid_local_timer_code, "\xd4\x6d\xdc\x79\xbb\xbc", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[46].invalid_local_timer_code, "\x25\x75\xff\x66\x17\xe1", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[46].valid_event, "\x3d\xb5\xee\xe2", EVENT_SIZE);
    memcpy(event_list[46].invalid_event, "\x74\x9e\xa8\x32", EVENT_SIZE);
    memcpy(event_list[46].fake_data_code_2, "\x68\x94\x0e\xe3", FAKE_DATA_SIZE_2);
    memcpy(event_list[46].random_data, "\x35\x90\x89\x57\x55\x21\x7e\xd9\x5f\x40\xf4\xb3\x4c\x53", RANDOM_DATA_SIZE);
     
    event_list[47].id = 47; 
    event_list[47].event_type = OBJECT_DESTROY_EVENT;
    memcpy(event_list[47].valid_global_timer_code, "\x14\x32\x0d\x7d\x2d\xfa", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[47].invalid_global_timer_code, "\x94\xca\x57\x59\x68\x85", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[47].valid_local_timer_code, "\xf4\xab\x4b\xdd\x1e\x10", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[47].fake_data_code_1, "\x5a\xbf\x6e\x81\x04\xbb\xb1\xf8", FAKE_DATA_SIZE_1);
    memcpy(event_list[47].invalid_local_timer_code, "\x3c\x6e\xcf\x59\x9a\xd7", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[47].valid_event, "\x4a\xf1\x8c\xb4", EVENT_SIZE);
    memcpy(event_list[47].invalid_event, "\x0f\x0b\x29\x0a", EVENT_SIZE);
    memcpy(event_list[47].fake_data_code_2, "\xf4\x2f\xb4\x66", FAKE_DATA_SIZE_2);
    memcpy(event_list[47].random_data, "\xc7\x60\xd7\x50\x11\xa0\x77\x77\x1b\xec\xa6\xec\x58\xeb", RANDOM_DATA_SIZE);
     
    event_list[48].id = 48; 
    event_list[48].event_type = OBJECT_FOCUS_EVENT;
    memcpy(event_list[48].valid_global_timer_code, "\xe1\xdd\xfa\xde\xff\x71", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[48].invalid_global_timer_code, "\x50\x8a\xe8\x19\x51\x0f", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[48].valid_local_timer_code, "\x7f\xa2\x3e\x2d\x7f\x3e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[48].invalid_local_timer_code, "\x4b\x52\xbe\x42\xd3\x54", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[48].valid_event, "\xe3\xf3\x6f\x4f", EVENT_SIZE);
    memcpy(event_list[48].invalid_event, "\xd7\xd0\xcc\xef", EVENT_SIZE);
    memcpy(event_list[48].fake_data_code_2, "\xa1\xdc\x87\xfe", FAKE_DATA_SIZE_2);
    memcpy(event_list[48].random_data, "\xf0\xc7\x88\x1c\xed\xa6\xea\x77\xcb\xd3\x3c\x66\xa7\x71", RANDOM_DATA_SIZE);
     
    event_list[49].id = 49; 
    event_list[49].event_type = MINIMIZE_WINDOW_START_EVENT;
    memcpy(event_list[49].valid_global_timer_code, "\x64\x99\xb9\xf0\x1c\xa6", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[49].invalid_global_timer_code, "\xb9\xcb\xd4\x5d\xe9\x21", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[49].valid_local_timer_code, "\x47\x98\xaf\x06\x15\x40", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[49].invalid_local_timer_code, "\x55\x69\xca\x30\xc8\xa5", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[49].valid_event, "\x00\xa0\x0a\x87", EVENT_SIZE);
    memcpy(event_list[49].invalid_event, "\xed\x08\x52\x04", EVENT_SIZE);
    memcpy(event_list[49].fake_data_code_2, "\xed\x58\xd3\xfa", FAKE_DATA_SIZE_2);
    memcpy(event_list[49].random_data, "\x83\x70\x36\x43\x98\x31\xab\x06\x20\x1f\x9e\x6d\xa4\x22", RANDOM_DATA_SIZE);
     
    event_list[50].id = 50; 
    event_list[50].event_type = LOST_MOUSE_EVENT;
    memcpy(event_list[50].valid_global_timer_code, "\xdd\xb8\x20\x12\xb9\x6a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[50].invalid_global_timer_code, "\x3c\xc1\xf3\xf4\xe6\xd4", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[50].valid_local_timer_code, "\x58\xf1\xfe\x18\xc3\x68", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[50].invalid_local_timer_code, "\x44\x1c\xe7\x54\x49\x03", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[50].valid_event, "\x90\xed\x94\xb8", EVENT_SIZE);
    memcpy(event_list[50].invalid_event, "\x90\x7c\x06\xbe", EVENT_SIZE);
    memcpy(event_list[50].fake_data_code_2, "\xe3\x83\xd7\x8c", FAKE_DATA_SIZE_2);
    memcpy(event_list[50].random_data, "\xf2\x70\xd3\x80\xd7\x46\xf7\x0e\xc1\xf8\xc2\x53\xea\x48", RANDOM_DATA_SIZE);
     
    event_list[51].id = 51; 
    event_list[51].event_type = RECEIVE_MOUSE_EVENT;
    memcpy(event_list[51].fake_data_code_0, "\x0d\xaa\x75\x7d\x0b", FAKE_DATA_SIZE_0);
    memcpy(event_list[51].valid_global_timer_code, "\x16\x31\x4b\x4e\x39\xb5", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[51].invalid_global_timer_code, "\xe0\x4d\xd8\x9c\xa4\x18", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[51].valid_local_timer_code, "\xbc\x91\xd7\x9f\x70\xaf", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[51].fake_data_code_1, "\xd9\xdc\xdb\x25\x74\xbb\xaf\x1c", FAKE_DATA_SIZE_1);
    memcpy(event_list[51].invalid_local_timer_code, "\xb6\x99\x4d\x7d\x70\x58", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[51].valid_event, "\x68\x6f\x92\xe6", EVENT_SIZE);
    memcpy(event_list[51].invalid_event, "\x1b\x6c\xac\x10", EVENT_SIZE);
    memcpy(event_list[51].random_data, "\x14\x95\x01\xe1\xaf\xc7\x1b\xef\x54\x06\x2b\x93\xf0\x2f", RANDOM_DATA_SIZE);
     
    event_list[52].id = 52; 
    event_list[52].event_type = FOREGROUND_WINDOW_EVENT;
    memcpy(event_list[52].valid_global_timer_code, "\xe3\xa5\xee\x08\x4d\xb7", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[52].invalid_global_timer_code, "\x4b\x28\x7f\xde\x5f\xce", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[52].valid_local_timer_code, "\x2d\x33\x45\xbe\x62\x57", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[52].fake_data_code_1, "\x1d\x2e\x64\xca\x1d\x2f\xed\xa4", FAKE_DATA_SIZE_1);
    memcpy(event_list[52].invalid_local_timer_code, "\x87\xf4\x9f\x8e\xcd\xa1", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[52].valid_event, "\x5a\x80\x2b\x5f", EVENT_SIZE);
    memcpy(event_list[52].invalid_event, "\x43\x3c\x55\x71", EVENT_SIZE);
    memcpy(event_list[52].random_data, "\x07\xc8\x49\x71\x97\x2b\x35\x3a\x3c\x6a\x29\x3b\x75\x0b", RANDOM_DATA_SIZE);
     
    event_list[53].id = 53; 
    event_list[53].event_type = MENU_CONTEXT_START;
    memcpy(event_list[53].valid_global_timer_code, "\x8a\x1f\x0b\x52\xc1\xcc", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[53].invalid_global_timer_code, "\x5b\xd5\x39\x8f\x11\xcf", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[53].valid_local_timer_code, "\xd0\xa9\xc6\x3b\x39\x09", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[53].invalid_local_timer_code, "\xb2\xb3\x95\xa9\x73\xa9", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[53].valid_event, "\x49\x7b\xcc\x8f", EVENT_SIZE);
    memcpy(event_list[53].invalid_event, "\x19\x35\x8e\x2a", EVENT_SIZE);
    memcpy(event_list[53].random_data, "\x86\x73\xca\x4e\xed\x1d\x88\x0e\x79\xf5\xef\xb6\xfc\xb1", RANDOM_DATA_SIZE);
     
    event_list[54].id = 54; 
    event_list[54].event_type = MENU_CONTEXT_START;
    memcpy(event_list[54].valid_global_timer_code, "\x3a\xec\x30\xfb\x61\x1f", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[54].invalid_global_timer_code, "\x0b\x7a\x54\x0b\x06\xe2", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[54].valid_local_timer_code, "\x7c\xdd\xe4\x1b\x85\xf3", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[54].invalid_local_timer_code, "\xc6\x63\x25\x86\x84\x1d", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[54].valid_event, "\xe5\x15\x3b\xf3", EVENT_SIZE);
    memcpy(event_list[54].invalid_event, "\x79\x67\xa1\xbc", EVENT_SIZE);
    memcpy(event_list[54].random_data, "\xd7\xb4\x78\xab\x90\xc2\x56\x67\x5e\xc6\x0a\x2b\x0d\xdb", RANDOM_DATA_SIZE);
     
    event_list[55].id = 55; 
    event_list[55].event_type = MENU_SELECT_START;
    memcpy(event_list[55].fake_data_code_0, "\xc2\x51\x52\x26\x69", FAKE_DATA_SIZE_0);
    memcpy(event_list[55].valid_global_timer_code, "\x9c\x3f\x75\x51\x0b\x68", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[55].invalid_global_timer_code, "\xaa\x8f\xaf\x24\xa3\x64", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[55].valid_local_timer_code, "\x35\x71\x1e\x02\x31\x6d", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[55].fake_data_code_1, "\x6a\x2a\xc2\xdd\x53\x5e\x8e\xf4", FAKE_DATA_SIZE_1);
    memcpy(event_list[55].invalid_local_timer_code, "\xde\x03\xa1\x3c\xe2\xc7", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[55].valid_event, "\x6f\x3a\xb3\xed", EVENT_SIZE);
    memcpy(event_list[55].invalid_event, "\x41\x55\x08\x88", EVENT_SIZE);
    memcpy(event_list[55].random_data, "\xb9\xeb\x0f\x17\xdc\x16\x6a\x0a\x03\x9b\x95\x60\x84\x38", RANDOM_DATA_SIZE);
     
    event_list[56].id = 56; 
    event_list[56].event_type = OBJECT_CREATE_EVENT;
    memcpy(event_list[56].valid_global_timer_code, "\xff\xd2\x98\x1a\x07\xe5", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[56].invalid_global_timer_code, "\xcf\x16\x90\x63\xc4\x8a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[56].valid_local_timer_code, "\xf1\xae\x1d\x55\x7b\x94", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[56].invalid_local_timer_code, "\x2e\x68\x46\x66\x75\xc1", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[56].valid_event, "\xb4\xab\x28\xb7", EVENT_SIZE);
    memcpy(event_list[56].invalid_event, "\xe3\xeb\xad\xe8", EVENT_SIZE);
    memcpy(event_list[56].random_data, "\x6a\x4a\xb3\x8a\x36\xfd\x7f\x70\x7d\x79\xdb\xae\x7a\xac", RANDOM_DATA_SIZE);
     
    event_list[57].id = 57; 
    event_list[57].event_type = OBJECT_DESTROY_EVENT;
    memcpy(event_list[57].valid_global_timer_code, "\x71\xd3\xf6\x04\x9e\xcc", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[57].invalid_global_timer_code, "\x16\x6c\x04\xb0\x62\x09", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[57].valid_local_timer_code, "\xdc\x28\x96\xad\x20\x71", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[57].invalid_local_timer_code, "\x88\x95\x8f\xfa\x9e\x3a", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[57].valid_event, "\xc2\x82\xc0\x65", EVENT_SIZE);
    memcpy(event_list[57].invalid_event, "\x8b\x24\xd6\x57", EVENT_SIZE);
    memcpy(event_list[57].random_data, "\x03\xb4\x7d\xc6\x18\x7c\x93\x79\x07\xa4\xcc\x6d\xb5\xeb", RANDOM_DATA_SIZE);
     
    event_list[58].id = 58; 
    event_list[58].event_type = OBJECT_FOCUS_EVENT;
    memcpy(event_list[58].valid_global_timer_code, "\xde\x13\x1b\xaf\xcc\x3d", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[58].invalid_global_timer_code, "\x12\x52\x18\x7a\xc5\x59", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[58].valid_local_timer_code, "\x62\x42\xc0\x31\x4f\x45", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[58].fake_data_code_1, "\xd4\x26\x6a\xf0\x99\x95\x5d\x00", FAKE_DATA_SIZE_1);
    memcpy(event_list[58].invalid_local_timer_code, "\xa9\x59\x5c\x48\x94\xa5", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[58].valid_event, "\x25\x63\xfa\x5f", EVENT_SIZE);
    memcpy(event_list[58].invalid_event, "\x5f\x77\x09\x0d", EVENT_SIZE);
    memcpy(event_list[58].random_data, "\x53\x84\xa8\x11\x45\x19\x25\x29\xda\xbb\xb6\x37\xf8\x6f", RANDOM_DATA_SIZE);
     
    event_list[59].id = 59; 
    event_list[59].event_type = MINIMIZE_WINDOW_START_EVENT;
    memcpy(event_list[59].fake_data_code_0, "\xb7\xbf\x93\x95\xfe", FAKE_DATA_SIZE_0);
    memcpy(event_list[59].valid_global_timer_code, "\x82\x9d\xcb\x56\x5b\x77", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[59].invalid_global_timer_code, "\x16\x4b\x05\xaf\xdc\xd8", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[59].valid_local_timer_code, "\x13\x53\x89\x5b\x71\xef", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[59].fake_data_code_1, "\x26\xb8\x2b\x53\xe2\x76\x3f\x41", FAKE_DATA_SIZE_1);
    memcpy(event_list[59].invalid_local_timer_code, "\x7e\x2f\x36\xc9\x28\x80", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[59].valid_event, "\x68\xfa\x86\xba", EVENT_SIZE);
    memcpy(event_list[59].invalid_event, "\xa4\x03\x8e\x71", EVENT_SIZE);
    memcpy(event_list[59].random_data, "\xbf\x2f\x93\xa6\x17\x7e\x89\xbe\xa2\xc0\x82\x9a\x72\x4e", RANDOM_DATA_SIZE);
     
    event_list[60].id = 60; 
    event_list[60].event_type = LOST_MOUSE_EVENT;
    memcpy(event_list[60].valid_global_timer_code, "\xb3\x29\x5f\x47\x05\x22", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[60].invalid_global_timer_code, "\x1c\x79\x15\x95\x52\x60", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[60].valid_local_timer_code, "\xb4\x9e\x49\xfc\x8d\x56", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[60].fake_data_code_1, "\x43\x1f\xb5\x8f\xf2\x3a\xec\x4f", FAKE_DATA_SIZE_1);
    memcpy(event_list[60].invalid_local_timer_code, "\x37\x28\x14\x1c\xce\x36", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[60].valid_event, "\xf4\x72\x4c\x6c", EVENT_SIZE);
    memcpy(event_list[60].invalid_event, "\x04\x23\xf2\x97", EVENT_SIZE);
    memcpy(event_list[60].random_data, "\x71\x86\x1f\xb1\xaa\x9d\xdd\xa7\x43\x80\x17\xcf\x75\xcb", RANDOM_DATA_SIZE);
     
    event_list[61].id = 61; 
    event_list[61].event_type = RECEIVE_MOUSE_EVENT;
    memcpy(event_list[61].valid_global_timer_code, "\x5f\xc6\x36\x3d\xa2\xaf", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[61].invalid_global_timer_code, "\x09\x7c\x03\xcb\x75\x53", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[61].valid_local_timer_code, "\xd4\x29\xb6\x65\x9f\xf5", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[61].invalid_local_timer_code, "\x8f\x29\xd9\x7e\x05\xeb", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[61].valid_event, "\x81\x7b\xe4\xf9", EVENT_SIZE);
    memcpy(event_list[61].invalid_event, "\x69\x52\x37\x2b", EVENT_SIZE);
    memcpy(event_list[61].random_data, "\x49\x65\x5f\x0f\x07\x49\xff\x92\xe6\x6d\x8b\xf0\x15\x10", RANDOM_DATA_SIZE);
     
    event_list[62].id = 62; 
    event_list[62].event_type = FOREGROUND_WINDOW_EVENT;
    memcpy(event_list[62].valid_global_timer_code, "\x57\x41\xf0\x4e\x30\xcd", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[62].invalid_global_timer_code, "\x16\x0f\xb3\x0b\xbe\xcb", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[62].valid_local_timer_code, "\x43\x32\x91\x7d\xf6\x17", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[62].invalid_local_timer_code, "\xb2\xae\x2a\xf1\x6e\x80", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[62].valid_event, "\x39\x6f\x29\xb8", EVENT_SIZE);
    memcpy(event_list[62].invalid_event, "\x65\x86\x04\x8c", EVENT_SIZE);
    memcpy(event_list[62].fake_data_code_2, "\x9c\xb6\x52\x3f", FAKE_DATA_SIZE_2);
    memcpy(event_list[62].random_data, "\x0e\x8f\x96\x52\x33\x38\x3a\x6e\x1c\xcb\x07\x70\xea\xdd", RANDOM_DATA_SIZE);
     
    event_list[63].id = 63; 
    event_list[63].event_type = MENU_CONTEXT_START;
    memcpy(event_list[63].fake_data_code_0, "\x22\x7a\x63\x47\xfe", FAKE_DATA_SIZE_0);
    memcpy(event_list[63].valid_global_timer_code, "\x49\x12\xac\x69\x23\xc2", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[63].invalid_global_timer_code, "\x28\x5c\x21\x69\xf4\x64", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[63].valid_local_timer_code, "\x51\xde\x01\x70\xf3\xae", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[63].invalid_local_timer_code, "\x4d\xe7\x01\x60\x93\x3e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[63].valid_event, "\xd6\x0b\xc2\x2c", EVENT_SIZE);
    memcpy(event_list[63].invalid_event, "\x6c\x69\x03\x0c", EVENT_SIZE);
    memcpy(event_list[63].fake_data_code_2, "\x69\x44\x64\xd8", FAKE_DATA_SIZE_2);
    memcpy(event_list[63].random_data, "\x87\xd7\x9c\x3e\x54\x2e\x51\x98\xc1\x2f\x1f\xa3\x8b\xd8", RANDOM_DATA_SIZE);
     
    event_list[64].id = 64; 
    event_list[64].event_type = MENU_CONTEXT_START;
    memcpy(event_list[64].fake_data_code_0, "\x1e\x5b\x5b\x22\x6b", FAKE_DATA_SIZE_0);
    memcpy(event_list[64].valid_global_timer_code, "\x8b\xdf\x15\xc4\xd2\x22", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[64].invalid_global_timer_code, "\x3f\xd7\x2c\x0c\x19\xbf", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[64].valid_local_timer_code, "\x8e\x5f\x0c\x63\x15\xf1", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[64].invalid_local_timer_code, "\xf0\x7e\xa2\x4e\x40\xc6", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[64].valid_event, "\xd8\x8f\xb4\x83", EVENT_SIZE);
    memcpy(event_list[64].invalid_event, "\xb2\x60\x54\xa1", EVENT_SIZE);
    memcpy(event_list[64].random_data, "\xa7\x56\xbf\x39\x2b\xa8\x9c\x70\x7b\x67\x28\xf8\x26\x62", RANDOM_DATA_SIZE);
     
    event_list[65].id = 65; 
    event_list[65].event_type = MENU_SELECT_START;
    memcpy(event_list[65].valid_global_timer_code, "\x6e\x0c\x3d\x80\x69\x87", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[65].invalid_global_timer_code, "\x29\xd3\x48\x05\x9a\x64", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[65].valid_local_timer_code, "\x78\xf4\x52\xed\x16\x2f", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[65].fake_data_code_1, "\xb2\x64\x3f\x4f\x02\xc0\xaf\xe9", FAKE_DATA_SIZE_1);
    memcpy(event_list[65].invalid_local_timer_code, "\x3a\xf5\xc9\xea\x25\x7a", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[65].valid_event, "\x00\xf4\xd8\x47", EVENT_SIZE);
    memcpy(event_list[65].invalid_event, "\xf6\xe6\xc9\x90", EVENT_SIZE);
    memcpy(event_list[65].random_data, "\x39\x43\xb3\x99\x1c\xcf\xab\xc5\xff\x34\x8a\xe6\xf7\xed", RANDOM_DATA_SIZE);
     
    event_list[66].id = 66; 
    event_list[66].event_type = OBJECT_CREATE_EVENT;
    memcpy(event_list[66].valid_global_timer_code, "\x30\x8d\x82\x2f\x64\x61", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[66].invalid_global_timer_code, "\xdb\x96\x32\x8d\xba\x0c", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[66].valid_local_timer_code, "\xb0\x2c\xaa\xff\xb1\xc2", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[66].invalid_local_timer_code, "\x7d\x86\xed\x00\xf7\xf0", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[66].valid_event, "\xba\x2f\x82\xe3", EVENT_SIZE);
    memcpy(event_list[66].invalid_event, "\x5d\x41\xcb\x61", EVENT_SIZE);
    memcpy(event_list[66].random_data, "\x36\xfd\xc1\x41\x76\x3a\x9a\x2f\xd7\xc4\xc6\xe9\x16\xa0", RANDOM_DATA_SIZE);
     
    event_list[67].id = 67; 
    event_list[67].event_type = OBJECT_DESTROY_EVENT;
    memcpy(event_list[67].valid_global_timer_code, "\xcb\x42\xb4\x8d\xf5\x89", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[67].invalid_global_timer_code, "\x19\x6b\xfc\x17\x47\xc3", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[67].valid_local_timer_code, "\x77\x57\xf8\x6b\xac\x7b", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[67].invalid_local_timer_code, "\x3d\x8a\xac\x31\x11\xd9", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[67].valid_event, "\x02\x05\x0e\x19", EVENT_SIZE);
    memcpy(event_list[67].invalid_event, "\xce\xf8\x4c\x6e", EVENT_SIZE);
    memcpy(event_list[67].random_data, "\x29\x64\xf0\x72\x50\x16\xad\x66\x6d\xbe\x27\x2d\xea\x93", RANDOM_DATA_SIZE);
     
    event_list[68].id = 68; 
    event_list[68].event_type = OBJECT_FOCUS_EVENT;
    memcpy(event_list[68].valid_global_timer_code, "\x1a\x44\xfc\x24\x2b\x02", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[68].invalid_global_timer_code, "\x46\x19\x98\x73\x8d\x8c", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[68].valid_local_timer_code, "\x67\x13\xc4\x86\x82\x42", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[68].fake_data_code_1, "\x6b\xfe\x24\x53\x5e\x4f\x90\x8a", FAKE_DATA_SIZE_1);
    memcpy(event_list[68].invalid_local_timer_code, "\xb2\x05\x50\xab\xeb\x4c", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[68].valid_event, "\xfd\xe2\x2c\x31", EVENT_SIZE);
    memcpy(event_list[68].invalid_event, "\x20\xee\x28\xb4", EVENT_SIZE);
    memcpy(event_list[68].random_data, "\x51\xe7\x05\x14\xc1\x2d\xc6\xba\x20\x99\xf1\xa1\xac\xa9", RANDOM_DATA_SIZE);
     
    event_list[69].id = 69; 
    event_list[69].event_type = MINIMIZE_WINDOW_START_EVENT;
    memcpy(event_list[69].fake_data_code_0, "\x39\x85\x55\x56\x8d", FAKE_DATA_SIZE_0);
    memcpy(event_list[69].valid_global_timer_code, "\x67\x57\xba\x9a\x73\xea", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[69].invalid_global_timer_code, "\xa7\xf5\x59\x3f\xe1\x53", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[69].valid_local_timer_code, "\x4b\xb5\xfa\x07\x61\x39", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[69].invalid_local_timer_code, "\xf2\x83\xc6\x2a\x88\x8e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[69].valid_event, "\x5f\xab\x93\x76", EVENT_SIZE);
    memcpy(event_list[69].invalid_event, "\x01\x90\x1f\x57", EVENT_SIZE);
    memcpy(event_list[69].fake_data_code_2, "\x43\xea\x90\xce", FAKE_DATA_SIZE_2);
    memcpy(event_list[69].random_data, "\x19\x28\x5c\xbd\x09\x91\x5c\xda\x4b\xf1\x4b\xff\xeb\x0f", RANDOM_DATA_SIZE);
     
    event_list[70].id = 70; 
    event_list[70].event_type = LOST_MOUSE_EVENT;
    memcpy(event_list[70].valid_global_timer_code, "\xb4\x27\x4b\x4a\x47\xe4", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[70].invalid_global_timer_code, "\x57\x68\xbd\xdc\xb4\xd5", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[70].valid_local_timer_code, "\x14\xfc\x2f\x39\xb8\x81", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[70].invalid_local_timer_code, "\x5f\xf5\x83\xdd\x6f\x20", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[70].valid_event, "\x07\xb2\xc8\xa9", EVENT_SIZE);
    memcpy(event_list[70].invalid_event, "\xab\x9f\x87\xe5", EVENT_SIZE);
    memcpy(event_list[70].random_data, "\x64\x15\x8d\x16\x77\x30\x70\xaf\x77\x30\xc9\x23\xc4\x65", RANDOM_DATA_SIZE);
     
    event_list[71].id = 71; 
    event_list[71].event_type = RECEIVE_MOUSE_EVENT;
    memcpy(event_list[71].valid_global_timer_code, "\x5a\x6a\x19\x54\xed\xbc", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[71].invalid_global_timer_code, "\x18\xa8\x0a\x69\x31\xf6", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[71].valid_local_timer_code, "\x35\x0c\x19\xd6\x40\xc5", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[71].invalid_local_timer_code, "\xe1\x7e\x40\xd3\xb1\x02", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[71].valid_event, "\x22\x2a\xad\xa5", EVENT_SIZE);
    memcpy(event_list[71].invalid_event, "\x4e\x41\x9b\x9b", EVENT_SIZE);
    memcpy(event_list[71].fake_data_code_2, "\x70\xe8\x07\x2d", FAKE_DATA_SIZE_2);
    memcpy(event_list[71].random_data, "\x0a\x94\xee\x0a\xc6\x1c\x76\x12\xd4\x12\xc0\xef\x4f\x1a", RANDOM_DATA_SIZE);
     
    event_list[72].id = 72; 
    event_list[72].event_type = FOREGROUND_WINDOW_EVENT;
    memcpy(event_list[72].valid_global_timer_code, "\xa9\x02\x76\xc1\x63\xa2", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[72].invalid_global_timer_code, "\x8c\x3b\x50\x56\xae\x96", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[72].valid_local_timer_code, "\x3d\x6c\xf2\x08\x33\xa3", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[72].invalid_local_timer_code, "\xc0\xea\x3c\xb0\x1c\xfd", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[72].valid_event, "\x27\xa0\x4e\x48", EVENT_SIZE);
    memcpy(event_list[72].invalid_event, "\x12\xde\xa5\xb1", EVENT_SIZE);
    memcpy(event_list[72].random_data, "\xcf\x7f\x8c\xd3\x43\x78\x6b\x9c\xa4\x45\x89\x32\x3d\xc1", RANDOM_DATA_SIZE);
     
    event_list[73].id = 73; 
    event_list[73].event_type = MENU_CONTEXT_START;
    memcpy(event_list[73].fake_data_code_0, "\xa2\xaa\xc4\xf6\x6e", FAKE_DATA_SIZE_0);
    memcpy(event_list[73].valid_global_timer_code, "\x66\xe5\xba\x6d\xe6\xc7", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[73].invalid_global_timer_code, "\x36\x43\x98\x6c\x63\x99", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[73].valid_local_timer_code, "\xf4\x7f\x21\x7f\x05\xad", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[73].fake_data_code_1, "\x1d\xf6\xcf\x86\x72\x73\x09\x3d", FAKE_DATA_SIZE_1);
    memcpy(event_list[73].invalid_local_timer_code, "\x97\xb5\xad\x9a\xb2\x3c", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[73].valid_event, "\x37\x10\xb4\x27", EVENT_SIZE);
    memcpy(event_list[73].invalid_event, "\x5d\xc7\xd7\xbb", EVENT_SIZE);
    memcpy(event_list[73].fake_data_code_2, "\x2f\x2a\x88\x2a", FAKE_DATA_SIZE_2);
    memcpy(event_list[73].random_data, "\x8e\xa9\x95\x99\x4b\x3b\xc6\xe8\x1e\x85\x33\xd0\xbc\x83", RANDOM_DATA_SIZE);
     
    event_list[74].id = 74; 
    event_list[74].event_type = MENU_CONTEXT_START;
    memcpy(event_list[74].valid_global_timer_code, "\x5b\xc9\xb7\x1c\x5b\x89", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[74].invalid_global_timer_code, "\x4c\x03\x56\x39\x0e\xe5", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[74].valid_local_timer_code, "\xad\xed\xa4\x7e\x84\x81", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[74].fake_data_code_1, "\x30\xb0\xef\xc8\x80\x40\xc5\x1d", FAKE_DATA_SIZE_1);
    memcpy(event_list[74].invalid_local_timer_code, "\x6b\x4a\x2c\x5f\xc9\xcd", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[74].valid_event, "\xce\x6c\xda\x84", EVENT_SIZE);
    memcpy(event_list[74].invalid_event, "\x44\x12\xff\x87", EVENT_SIZE);
    memcpy(event_list[74].fake_data_code_2, "\x21\x9d\xae\xa3", FAKE_DATA_SIZE_2);
    memcpy(event_list[74].random_data, "\x98\x49\x3d\xb1\x12\x45\x39\x96\x92\x38\x53\x40\x64\xc5", RANDOM_DATA_SIZE);
     
    event_list[75].id = 75; 
    event_list[75].event_type = MENU_SELECT_START;
    memcpy(event_list[75].valid_global_timer_code, "\x56\xa0\xa8\xce\x47\x2b", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[75].invalid_global_timer_code, "\x94\xa9\xae\x2d\xf5\x57", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[75].valid_local_timer_code, "\x36\x44\x5b\xdb\xd7\xa6", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[75].invalid_local_timer_code, "\xd2\x7f\x46\xa5\x93\x29", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[75].valid_event, "\xdd\x09\x15\x40", EVENT_SIZE);
    memcpy(event_list[75].invalid_event, "\x06\xfd\x2d\x74", EVENT_SIZE);
    memcpy(event_list[75].random_data, "\xad\xcc\x1c\x82\xda\xbf\xaa\x35\x88\x89\x94\xbc\x51\xed", RANDOM_DATA_SIZE);
     
    event_list[76].id = 76; 
    event_list[76].event_type = OBJECT_CREATE_EVENT;
    memcpy(event_list[76].valid_global_timer_code, "\xfe\xce\x87\xb8\xbb\x71", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[76].invalid_global_timer_code, "\x8f\x0a\xfe\x0e\x7e\x7f", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[76].valid_local_timer_code, "\x88\x5a\x68\xfb\xc3\x7c", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[76].fake_data_code_1, "\x85\xc2\xc0\xe1\x47\x18\xf8\xc5", FAKE_DATA_SIZE_1);
    memcpy(event_list[76].invalid_local_timer_code, "\x2a\xb0\x55\xb1\xfe\xde", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[76].valid_event, "\xaf\x44\x40\xbc", EVENT_SIZE);
    memcpy(event_list[76].invalid_event, "\x36\x85\xa3\x0b", EVENT_SIZE);
    memcpy(event_list[76].fake_data_code_2, "\x41\x84\x41\xaa", FAKE_DATA_SIZE_2);
    memcpy(event_list[76].random_data, "\x96\xa4\xb2\x82\x25\xe0\x88\xb2\xb4\xac\xfa\xcf\x5a\x74", RANDOM_DATA_SIZE);
     
    event_list[77].id = 77; 
    event_list[77].event_type = OBJECT_DESTROY_EVENT;
    memcpy(event_list[77].fake_data_code_0, "\x8b\xb5\xc6\xab\x64", FAKE_DATA_SIZE_0);
    memcpy(event_list[77].valid_global_timer_code, "\xfc\x57\x42\x14\xc7\x4a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[77].invalid_global_timer_code, "\xf5\x05\x12\x62\xd8\x66", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[77].valid_local_timer_code, "\x46\x91\x46\x34\xe8\x78", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[77].invalid_local_timer_code, "\x9a\xf6\xec\xc7\x66\x55", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[77].valid_event, "\x6d\x46\x56\x28", EVENT_SIZE);
    memcpy(event_list[77].invalid_event, "\x71\x67\xb6\xab", EVENT_SIZE);
    memcpy(event_list[77].random_data, "\xc7\x70\x92\xa0\x93\x41\xe7\xcd\x2e\x6c\x34\xdb\xcd\x96", RANDOM_DATA_SIZE);
     
    event_list[78].id = 78; 
    event_list[78].event_type = OBJECT_FOCUS_EVENT;
    memcpy(event_list[78].valid_global_timer_code, "\xc4\xe0\xa7\xd0\x5f\x5a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[78].invalid_global_timer_code, "\xb4\xd8\xd6\xc3\x94\x8c", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[78].valid_local_timer_code, "\x54\x65\x1e\x80\x33\xa8", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[78].fake_data_code_1, "\xd2\xb0\xe8\xc2\x29\x99\x2c\x69", FAKE_DATA_SIZE_1);
    memcpy(event_list[78].invalid_local_timer_code, "\xd3\x14\xc8\x34\xc6\xed", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[78].valid_event, "\x3a\x63\x44\xe9", EVENT_SIZE);
    memcpy(event_list[78].invalid_event, "\xca\x55\xe4\x58", EVENT_SIZE);
    memcpy(event_list[78].random_data, "\x06\x28\x4e\x8a\x79\x04\x37\x72\x74\x42\xd1\xf3\xac\xa1", RANDOM_DATA_SIZE);
     
    event_list[79].id = 79; 
    event_list[79].event_type = MINIMIZE_WINDOW_START_EVENT;
    memcpy(event_list[79].valid_global_timer_code, "\xfe\x6b\x02\xe5\x49\x72", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[79].invalid_global_timer_code, "\x29\x08\x14\x93\xbd\x75", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[79].valid_local_timer_code, "\xd8\xdd\x93\xb3\xda\x2a", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[79].fake_data_code_1, "\xa5\xc9\x1d\xe5\x1f\x67\x23\xb8", FAKE_DATA_SIZE_1);
    memcpy(event_list[79].invalid_local_timer_code, "\xb0\xb4\xf3\xae\x2b\xdd", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[79].valid_event, "\x90\xf7\x17\xb2", EVENT_SIZE);
    memcpy(event_list[79].invalid_event, "\x10\x5a\x42\xbe", EVENT_SIZE);
    memcpy(event_list[79].random_data, "\x24\xd2\x5b\x35\xff\x17\xab\x5f\x9c\x82\xf0\x6d\x79\x60", RANDOM_DATA_SIZE);
     
    event_list[80].id = 80; 
    event_list[80].event_type = LOST_MOUSE_EVENT;
    memcpy(event_list[80].fake_data_code_0, "\xb6\xbf\xec\x14\x18", FAKE_DATA_SIZE_0);
    memcpy(event_list[80].valid_global_timer_code, "\x42\x74\xc1\xe9\xa0\x85", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[80].invalid_global_timer_code, "\xb1\x9a\xd6\xac\xa4\x3d", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[80].valid_local_timer_code, "\xd4\x6c\x57\x92\x87\x20", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[80].invalid_local_timer_code, "\x18\x95\x7a\xb9\x4d\x40", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[80].valid_event, "\x70\x41\x0e\xc1", EVENT_SIZE);
    memcpy(event_list[80].invalid_event, "\x7b\xdd\x8c\xd1", EVENT_SIZE);
    memcpy(event_list[80].fake_data_code_2, "\x41\xd0\x8b\x9f", FAKE_DATA_SIZE_2);
    memcpy(event_list[80].random_data, "\xe9\xe2\xcf\x30\xd1\x82\x8b\x9e\xe8\x8f\x80\x8e\x38\x99", RANDOM_DATA_SIZE);
     
    event_list[81].id = 81; 
    event_list[81].event_type = RECEIVE_MOUSE_EVENT;
    memcpy(event_list[81].valid_global_timer_code, "\xdd\xf6\x1f\xb9\x0e\x38", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[81].invalid_global_timer_code, "\x1a\xf9\x79\x4f\x53\xaf", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[81].valid_local_timer_code, "\x33\xc4\xac\x69\x10\x89", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[81].invalid_local_timer_code, "\xe7\x34\x49\x14\x08\x6d", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[81].valid_event, "\x09\x0c\x0e\x09", EVENT_SIZE);
    memcpy(event_list[81].invalid_event, "\x04\x74\x27\x64", EVENT_SIZE);
    memcpy(event_list[81].fake_data_code_2, "\x03\x0c\xfd\x10", FAKE_DATA_SIZE_2);
    memcpy(event_list[81].random_data, "\x82\x2e\x12\xf8\xe6\x5b\x10\x02\x00\xb8\x53\xc2\x87\xa8", RANDOM_DATA_SIZE);
     
    event_list[82].id = 82; 
    event_list[82].event_type = FOREGROUND_WINDOW_EVENT;
    memcpy(event_list[82].valid_global_timer_code, "\xa7\xe7\x9d\xa9\xc6\x05", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[82].invalid_global_timer_code, "\xab\xb5\xd1\x0b\x38\x00", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[82].valid_local_timer_code, "\xbd\x6f\xc1\xbc\xaf\x3f", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[82].invalid_local_timer_code, "\xd7\xc3\x1a\x58\xd2\xb0", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[82].valid_event, "\x62\xfe\xf5\x7c", EVENT_SIZE);
    memcpy(event_list[82].invalid_event, "\xdd\x72\xf1\x7d", EVENT_SIZE);
    memcpy(event_list[82].random_data, "\xee\x16\x0b\x0a\x08\xdb\x83\xd2\xb8\xbf\x87\x96\xd3\xb5", RANDOM_DATA_SIZE);
     
    event_list[83].id = 83; 
    event_list[83].event_type = MENU_CONTEXT_START;
    memcpy(event_list[83].valid_global_timer_code, "\xe0\x70\x3a\x66\x3a\x0e", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[83].invalid_global_timer_code, "\x5f\x3b\x68\x61\xdb\x79", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[83].valid_local_timer_code, "\x28\x57\xcf\x04\x30\xfb", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[83].fake_data_code_1, "\xec\x0c\x66\xbd\xcb\x04\x79\x88", FAKE_DATA_SIZE_1);
    memcpy(event_list[83].invalid_local_timer_code, "\x60\x15\xa5\x4d\x0f\x6c", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[83].valid_event, "\xb3\xcf\x32\x69", EVENT_SIZE);
    memcpy(event_list[83].invalid_event, "\x61\xcb\xd5\xe2", EVENT_SIZE);
    memcpy(event_list[83].random_data, "\x4e\x94\x8a\xe8\x8e\xbd\x56\x4c\x54\xbf\xda\x49\x03\x80", RANDOM_DATA_SIZE);
     
    event_list[84].id = 84; 
    event_list[84].event_type = MENU_CONTEXT_START;
    memcpy(event_list[84].fake_data_code_0, "\x18\x53\x3e\x88\x3d", FAKE_DATA_SIZE_0);
    memcpy(event_list[84].valid_global_timer_code, "\xfc\x10\xe8\xb5\x96\x4a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[84].invalid_global_timer_code, "\xd3\x79\x14\x52\x2f\x6b", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[84].valid_local_timer_code, "\x8e\xb2\x58\xf3\xa5\x98", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[84].invalid_local_timer_code, "\x17\xd6\xd2\x9b\xe3\x4e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[84].valid_event, "\x7a\xe0\xcb\xa1", EVENT_SIZE);
    memcpy(event_list[84].invalid_event, "\xe5\x79\xcb\xea", EVENT_SIZE);
    memcpy(event_list[84].random_data, "\x46\x7c\x0e\x65\x63\x81\xe0\x1f\x78\x5e\x6e\xd0\x8a\x5a", RANDOM_DATA_SIZE);
     
    event_list[85].id = 85; 
    event_list[85].event_type = MENU_SELECT_START;
    memcpy(event_list[85].valid_global_timer_code, "\x78\x03\xf1\xc1\x6c\xec", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[85].invalid_global_timer_code, "\x71\xb9\xe6\xd1\x72\xaa", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[85].valid_local_timer_code, "\x78\xac\xef\x2e\x48\x47", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[85].invalid_local_timer_code, "\x03\xc2\xbe\xdf\x99\x08", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[85].valid_event, "\xe7\x6c\xb5\x5b", EVENT_SIZE);
    memcpy(event_list[85].invalid_event, "\x57\x30\xac\x42", EVENT_SIZE);
    memcpy(event_list[85].random_data, "\x1b\xc4\x6d\xb5\xbf\xb3\x6a\x0f\xbc\x77\xd7\x67\x4f\x07", RANDOM_DATA_SIZE);
     
    event_list[86].id = 86; 
    event_list[86].event_type = OBJECT_CREATE_EVENT;
    memcpy(event_list[86].valid_global_timer_code, "\xe0\x81\xa4\x1f\xcf\x46", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[86].invalid_global_timer_code, "\xe3\x5a\x93\x24\x2f\xd5", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[86].valid_local_timer_code, "\x01\x6a\x4a\xc9\xf8\x3d", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[86].fake_data_code_1, "\xa6\xe1\x92\x83\x1d\x52\xf5\x18", FAKE_DATA_SIZE_1);
    memcpy(event_list[86].invalid_local_timer_code, "\x27\x97\x80\xf6\x63\xff", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[86].valid_event, "\xe7\x31\x00\xa6", EVENT_SIZE);
    memcpy(event_list[86].invalid_event, "\x00\xe3\x9e\x2a", EVENT_SIZE);
    memcpy(event_list[86].random_data, "\x74\x1e\x08\x1f\xf4\xec\x98\xba\x40\xb6\x89\x6f\x85\x35", RANDOM_DATA_SIZE);
     
    event_list[87].id = 87; 
    event_list[87].event_type = OBJECT_DESTROY_EVENT;
    memcpy(event_list[87].valid_global_timer_code, "\xb4\xbf\xdb\xfd\x36\x25", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[87].invalid_global_timer_code, "\x04\x74\x79\x27\x24\xcc", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[87].valid_local_timer_code, "\xc3\x41\x69\x7c\xdf\x1b", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[87].invalid_local_timer_code, "\x11\xf8\x92\xf8\x9d\x7e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[87].valid_event, "\x2f\x6f\x58\x10", EVENT_SIZE);
    memcpy(event_list[87].invalid_event, "\x67\x20\x1d\xa7", EVENT_SIZE);
    memcpy(event_list[87].random_data, "\x31\x22\x04\x1b\xdc\x8e\x56\x2e\x2c\x8d\x34\x95\x4f\xcb", RANDOM_DATA_SIZE);
     
    event_list[88].id = 88; 
    event_list[88].event_type = OBJECT_FOCUS_EVENT;
    memcpy(event_list[88].valid_global_timer_code, "\xe3\x8a\xa4\x5e\x69\xca", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[88].invalid_global_timer_code, "\x5b\x5c\x5f\x0b\x62\xd1", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[88].valid_local_timer_code, "\x5a\x39\xda\x2a\x3d\x40", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[88].invalid_local_timer_code, "\xfa\x6b\xd6\xeb\x0d\xa3", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[88].valid_event, "\x19\x1c\x24\x44", EVENT_SIZE);
    memcpy(event_list[88].invalid_event, "\xc0\x60\xb2\x0b", EVENT_SIZE);
    memcpy(event_list[88].random_data, "\x9e\xf9\xe9\x73\x6a\xa4\x7f\x5f\x67\x60\x2e\xb9\x38\x2e", RANDOM_DATA_SIZE);
     
    event_list[89].id = 89; 
    event_list[89].event_type = MINIMIZE_WINDOW_START_EVENT;
    memcpy(event_list[89].valid_global_timer_code, "\x90\x10\x2f\xab\x22\xf9", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[89].invalid_global_timer_code, "\x44\x7b\x7e\x22\x4a\x31", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[89].valid_local_timer_code, "\xa3\x0d\x68\xf7\x25\x65", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[89].invalid_local_timer_code, "\x2b\x69\x87\x55\x39\x8e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[89].valid_event, "\x3d\xd7\x10\xfc", EVENT_SIZE);
    memcpy(event_list[89].invalid_event, "\x79\xaf\x64\x62", EVENT_SIZE);
    memcpy(event_list[89].fake_data_code_2, "\x09\xb9\xb7\x9d", FAKE_DATA_SIZE_2);
    memcpy(event_list[89].random_data, "\xf0\x7c\x9b\x8f\xfb\x7a\x59\xfd\x33\x53\x6c\xf5\x36\x11", RANDOM_DATA_SIZE);
     
    event_list[90].id = 90; 
    event_list[90].event_type = LOST_MOUSE_EVENT;
    memcpy(event_list[90].fake_data_code_0, "\xfc\x15\x72\x8a\xb3", FAKE_DATA_SIZE_0);
    memcpy(event_list[90].valid_global_timer_code, "\x0d\xd0\xb8\xfc\x91\xe2", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[90].invalid_global_timer_code, "\xda\x97\x1b\x18\xda\x97", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[90].valid_local_timer_code, "\x49\x73\x34\x9b\xf0\x61", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[90].invalid_local_timer_code, "\xb9\x67\x6a\x62\x77\x87", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[90].valid_event, "\x35\x70\xfd\xb0", EVENT_SIZE);
    memcpy(event_list[90].invalid_event, "\x01\xc3\xeb\x79", EVENT_SIZE);
    memcpy(event_list[90].fake_data_code_2, "\x40\xbf\x55\xdb", FAKE_DATA_SIZE_2);
    memcpy(event_list[90].random_data, "\x5e\x01\xfc\x3e\x05\xac\x48\x60\xc1\x48\xcc\x2a\xbc\xdf", RANDOM_DATA_SIZE);
     
    event_list[91].id = 91; 
    event_list[91].event_type = RECEIVE_MOUSE_EVENT;
    memcpy(event_list[91].valid_global_timer_code, "\xad\x5d\x6b\x85\x68\x0f", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[91].invalid_global_timer_code, "\xfc\x0a\x58\x39\xb2\xff", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[91].valid_local_timer_code, "\x7a\xb2\x67\xb7\x06\x9e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[91].invalid_local_timer_code, "\x53\xd2\xb9\x24\x66\x23", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[91].valid_event, "\x85\x82\x66\xb8", EVENT_SIZE);
    memcpy(event_list[91].invalid_event, "\x76\xf2\x0a\xd3", EVENT_SIZE);
    memcpy(event_list[91].random_data, "\x42\xfd\x22\x33\x83\xdb\x3e\x66\x44\xa7\xc4\x2d\x6b\xc6", RANDOM_DATA_SIZE);
     
    event_list[92].id = 92; 
    event_list[92].event_type = FOREGROUND_WINDOW_EVENT;
    memcpy(event_list[92].fake_data_code_0, "\x4e\xb4\x75\x6a\x3c", FAKE_DATA_SIZE_0);
    memcpy(event_list[92].valid_global_timer_code, "\x6e\x52\x21\x60\xc5\xc7", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[92].invalid_global_timer_code, "\xc5\x8a\x24\x6b\xf9\x58", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[92].valid_local_timer_code, "\x6a\x69\xce\x62\xaa\x13", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[92].invalid_local_timer_code, "\xcd\x1a\x2d\x82\xb1\x50", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[92].valid_event, "\x8f\x13\xbe\xfd", EVENT_SIZE);
    memcpy(event_list[92].invalid_event, "\x12\xfa\xc1\x28", EVENT_SIZE);
    memcpy(event_list[92].fake_data_code_2, "\xc3\xca\x19\x7d", FAKE_DATA_SIZE_2);
    memcpy(event_list[92].random_data, "\x7a\xee\x32\x89\x4a\x58\xbe\x28\xd8\x2a\x95\x8f\x80\x69", RANDOM_DATA_SIZE);
     
    event_list[93].id = 93; 
    event_list[93].event_type = MENU_CONTEXT_START;
    memcpy(event_list[93].valid_global_timer_code, "\x57\xe6\x7b\xb8\x8e\xe1", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[93].invalid_global_timer_code, "\xe4\xda\x97\x6d\x8e\xa7", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[93].valid_local_timer_code, "\x4d\xe7\xbb\xeb\x23\xcd", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[93].invalid_local_timer_code, "\xce\xe0\x54\x60\x7d\x2c", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[93].valid_event, "\x49\x09\xa0\x3e", EVENT_SIZE);
    memcpy(event_list[93].invalid_event, "\x37\x49\xa9\x3b", EVENT_SIZE);
    memcpy(event_list[93].random_data, "\x55\x9d\x28\xbf\x46\x7c\x4f\x52\xff\xb5\x6c\x67\xc5\xd9", RANDOM_DATA_SIZE);
     
    event_list[94].id = 94; 
    event_list[94].event_type = MENU_CONTEXT_START;
    memcpy(event_list[94].valid_global_timer_code, "\x18\xb8\x29\x83\xbd\x54", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[94].invalid_global_timer_code, "\xcf\x0c\xf1\x48\x0d\x3c", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[94].valid_local_timer_code, "\x56\x09\xb1\xe9\x3a\xca", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[94].invalid_local_timer_code, "\xcd\x1e\x20\x2f\xab\x5d", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[94].valid_event, "\xb8\xd6\xfb\x74", EVENT_SIZE);
    memcpy(event_list[94].invalid_event, "\x80\xb7\xdb\xfb", EVENT_SIZE);
    memcpy(event_list[94].random_data, "\xcf\x4d\x37\xea\xc9\xc4\xff\xa9\x5a\xcc\xe1\x3d\x8e\xdf", RANDOM_DATA_SIZE);
     
    event_list[95].id = 95; 
    event_list[95].event_type = MENU_SELECT_START;
    memcpy(event_list[95].valid_global_timer_code, "\x3b\xd0\xe6\xdb\x0a\xf6", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[95].invalid_global_timer_code, "\x88\xa9\x55\x4d\x8f\x15", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[95].valid_local_timer_code, "\xc7\x1d\xfc\xd2\x78\x5b", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[95].fake_data_code_1, "\x54\x88\xf9\x0b\x4a\x30\x05\x0c", FAKE_DATA_SIZE_1);
    memcpy(event_list[95].invalid_local_timer_code, "\x6f\x4b\x72\xf5\x04\x1c", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[95].valid_event, "\xaf\x4a\x0a\x98", EVENT_SIZE);
    memcpy(event_list[95].invalid_event, "\xaf\x6c\xe9\xb8", EVENT_SIZE);
    memcpy(event_list[95].random_data, "\x8e\xbb\x87\x89\x5c\xc0\xfb\xa4\xe1\xd6\x26\x9f\xf6\x7f", RANDOM_DATA_SIZE);
     
    event_list[96].id = 96; 
    event_list[96].event_type = OBJECT_CREATE_EVENT;
    memcpy(event_list[96].valid_global_timer_code, "\x18\x5e\x18\xfe\x39\x08", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[96].invalid_global_timer_code, "\x3f\xe4\x64\x79\x44\x7a", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[96].valid_local_timer_code, "\xfe\x72\x17\x0f\x24\x7e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[96].invalid_local_timer_code, "\x8c\xf7\x1f\x25\x13\x41", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[96].valid_event, "\xc7\xef\x6c\xa8", EVENT_SIZE);
    memcpy(event_list[96].invalid_event, "\xa6\x30\xc6\x29", EVENT_SIZE);
    memcpy(event_list[96].random_data, "\xd9\x71\x85\x91\xee\x88\xc3\x93\x4c\xeb\xa3\x1c\xf9\x68", RANDOM_DATA_SIZE);
     
    event_list[97].id = 97; 
    event_list[97].event_type = OBJECT_DESTROY_EVENT;
    memcpy(event_list[97].fake_data_code_0, "\x43\x3e\x07\x13\xcd", FAKE_DATA_SIZE_0);
    memcpy(event_list[97].valid_global_timer_code, "\xfe\xfc\x15\x80\x2f\xa3", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[97].invalid_global_timer_code, "\x1f\x48\xe6\x6d\x59\x4c", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[97].valid_local_timer_code, "\x6b\xfd\x27\x03\xb7\x15", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[97].invalid_local_timer_code, "\xa8\x4a\x91\xcd\x1f\x3e", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[97].valid_event, "\xd3\x02\xc2\x4d", EVENT_SIZE);
    memcpy(event_list[97].invalid_event, "\x16\xa6\xac\x76", EVENT_SIZE);
    memcpy(event_list[97].random_data, "\xaa\x24\xe4\x80\xe1\x26\xdb\xab\x1b\x7e\xf6\x4b\xec\x79", RANDOM_DATA_SIZE);
     
    event_list[98].id = 98; 
    event_list[98].event_type = OBJECT_FOCUS_EVENT;
    memcpy(event_list[98].valid_global_timer_code, "\xc5\xa4\x5e\xac\x6c\xac", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[98].invalid_global_timer_code, "\x94\x64\x51\xca\xb1\x35", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[98].valid_local_timer_code, "\xd1\x5c\x68\x54\x89\x3b", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[98].fake_data_code_1, "\x99\x86\xd1\x4d\xd7\x4d\x85\x5d", FAKE_DATA_SIZE_1);
    memcpy(event_list[98].invalid_local_timer_code, "\x64\x1b\x09\x09\xb9\xdd", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[98].valid_event, "\x08\x56\x0f\xde", EVENT_SIZE);
    memcpy(event_list[98].invalid_event, "\xea\xb1\xf5\x14", EVENT_SIZE);
    memcpy(event_list[98].random_data, "\x27\x86\x13\x06\x75\xd4\xd8\x71\x59\x74\x93\xef\x6d\xc3", RANDOM_DATA_SIZE);
     
    event_list[99].id = 99; 
    event_list[99].event_type = MINIMIZE_WINDOW_START_EVENT;
    memcpy(event_list[99].valid_global_timer_code, "\x6a\xe3\x6c\xe7\x6a\xb7", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[99].invalid_global_timer_code, "\x99\xeb\x38\xb8\x39\x34", GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[99].valid_local_timer_code, "\xb5\x88\x3a\x6c\x7b\x71", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[99].fake_data_code_1, "\x8f\x5d\xd2\x07\x87\x44\x23\x13", FAKE_DATA_SIZE_1);
    memcpy(event_list[99].invalid_local_timer_code, "\x12\x05\x0f\x83\x6b\xa5", LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[99].valid_event, "\x0f\x7c\x83\x94", EVENT_SIZE);
    memcpy(event_list[99].invalid_event, "\xab\x63\x10\x6b", EVENT_SIZE);
    memcpy(event_list[99].fake_data_code_2, "\x50\x86\xa7\x72", FAKE_DATA_SIZE_2);
    memcpy(event_list[99].random_data, "\xc1\x82\xf5\xec\xf4\xd2\xb8\x4c\xdc\x49\x28\xd3\xe0\x57", RANDOM_DATA_SIZE);
    return event_list;
}

#endif
