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
    //REPLACE_ME_0
    //REPLACE_ME_1
    //REPLACE_ME_2
    //REPLACE_ME_3
    //REPLACE_ME_4
    //REPLACE_ME_5
    //REPLACE_ME_6
    //REPLACE_ME_7
    //REPLACE_ME_8
    //REPLACE_ME_9
    //REPLACE_ME_10
    //REPLACE_ME_11
    //REPLACE_ME_12
    //REPLACE_ME_13
    //REPLACE_ME_14
    //REPLACE_ME_15
    //REPLACE_ME_16
    //REPLACE_ME_17
    //REPLACE_ME_18
    //REPLACE_ME_19
    //REPLACE_ME_20
    //REPLACE_ME_21
    //REPLACE_ME_22
    //REPLACE_ME_23
    //REPLACE_ME_24
    //REPLACE_ME_25
    //REPLACE_ME_26
    //REPLACE_ME_27
    //REPLACE_ME_28
    //REPLACE_ME_29
    //REPLACE_ME_30
    //REPLACE_ME_31
    //REPLACE_ME_32
    //REPLACE_ME_33
    //REPLACE_ME_34
    //REPLACE_ME_35
    //REPLACE_ME_36
    //REPLACE_ME_37
    //REPLACE_ME_38
    //REPLACE_ME_39
    //REPLACE_ME_40
    //REPLACE_ME_41
    //REPLACE_ME_42
    //REPLACE_ME_43
    //REPLACE_ME_44
    //REPLACE_ME_45
    //REPLACE_ME_46
    //REPLACE_ME_47
    //REPLACE_ME_48
    //REPLACE_ME_49
    //REPLACE_ME_50
    //REPLACE_ME_51
    //REPLACE_ME_52
    //REPLACE_ME_53
    //REPLACE_ME_54
    //REPLACE_ME_55
    //REPLACE_ME_56
    //REPLACE_ME_57
    //REPLACE_ME_58
    //REPLACE_ME_59
    //REPLACE_ME_60
    //REPLACE_ME_61
    //REPLACE_ME_62
    //REPLACE_ME_63
    //REPLACE_ME_64
    //REPLACE_ME_65
    //REPLACE_ME_66
    //REPLACE_ME_67
    //REPLACE_ME_68
    //REPLACE_ME_69
    //REPLACE_ME_70
    //REPLACE_ME_71
    //REPLACE_ME_72
    //REPLACE_ME_73
    //REPLACE_ME_74
    //REPLACE_ME_75
    //REPLACE_ME_76
    //REPLACE_ME_77
    //REPLACE_ME_78
    //REPLACE_ME_79
    //REPLACE_ME_80
    //REPLACE_ME_81
    //REPLACE_ME_82
    //REPLACE_ME_83
    //REPLACE_ME_84
    //REPLACE_ME_85
    //REPLACE_ME_86
    //REPLACE_ME_87
    //REPLACE_ME_88
    //REPLACE_ME_89
    //REPLACE_ME_90
    //REPLACE_ME_91
    //REPLACE_ME_92
    //REPLACE_ME_93
    //REPLACE_ME_94
    //REPLACE_ME_95
    //REPLACE_ME_96
    //REPLACE_ME_97
    //REPLACE_ME_98
    //REPLACE_ME_99
    return event_list;
}

#endif
