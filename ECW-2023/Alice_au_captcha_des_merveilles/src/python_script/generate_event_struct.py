from pathlib import Path
from fileinput import FileInput
from secrets import token_hex
from random import randint
import secrets

def convert_hex_str(data_hex: str):
    res = ""
    separator = "\\x"
    for i in range(0, len(data_hex), 2):
        res += separator + data_hex[i:i+2]

    res = f'"{res}"'
    return res

if __name__ == "__main__":
    print("Building event struct")
    fake_data_code_0_size = 5
    fake_data_code_1_size = 8
    fake_data_code_2_size = 4
    n = 0
    event_create_template_0 = """ 
    event_list[ID].id = ID; 
    event_list[ID].event_type = EVENT_TYPE;"""
    event_create_template_1 = """
    memcpy(event_list[ID].fake_data_code_0, FAKE_DATA_0, FAKE_DATA_SIZE_0);"""
    event_create_template_2 = """
    memcpy(event_list[ID].valid_global_timer_code, VAL_GLOBAL_TIMER_DATA, GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[ID].invalid_global_timer_code, INVALI_GLOBAL_TIMER_DATA, GLOBAL_TIMER_CODE_SIZE);
    memcpy(event_list[ID].valid_local_timer_code, VAL_LOCAL_TIMER_DATA, LOCAL_TIMER_CODE_SIZE);"""
    event_create_template_3 = """
    memcpy(event_list[ID].fake_data_code_1, FAKE_DATA_1, FAKE_DATA_SIZE_1);"""
    event_create_template_4 = """
    memcpy(event_list[ID].invalid_local_timer_code, INVALI_LOCAL_TIMER_DATA, LOCAL_TIMER_CODE_SIZE);
    memcpy(event_list[ID].valid_event, VAL_EVENT, EVENT_SIZE);
    memcpy(event_list[ID].invalid_event, INVALI_EVENT, EVENT_SIZE);"""
    event_create_template_5 = """
    memcpy(event_list[ID].fake_data_code_2, FAKE_DATA_2, FAKE_DATA_SIZE_2);"""
    event_create_template_6 = """
    memcpy(event_list[ID].random_data, RANDOM_DATA_G, RANDOM_DATA_SIZE);"""

    current_dir = Path(__file__).parent.absolute()
    renders_dir = current_dir / "renders"
    client_dir = Path(__file__).parent.parent.absolute() / "client"
    with open(str(renders_dir / "event_data.txt"), "r") as event_data_file,\
        open(str(current_dir / "event_struct_header_template.h"), "r") as event_struct_header_template,\
        open((str(client_dir / "event_struct.h")), "w") as event_struct_h:
        for line in event_struct_header_template:
            to_replace = f"//REPLACE_ME_{n}"
            if(to_replace in line):
                event_create_template = """"""
                event_type = event_data_file.readline().strip() 
                global_val_timer_data = convert_hex_str(event_data_file.readline().strip()) 
                global_inval_timer_data = convert_hex_str(event_data_file.readline().strip()) 
                local_val_timer_data = convert_hex_str(event_data_file.readline().strip()) 
                local_inval_timer_data = convert_hex_str(event_data_file.readline().strip()) 
                valid_event_data =  convert_hex_str(event_data_file.readline().strip()) 
                invalid_event_data =  convert_hex_str(event_data_file.readline().strip()) 
                random_data = convert_hex_str(event_data_file.readline().strip()) 
                event_create_template += event_create_template_0
                if(randint(0, 2) == 0): 
                    event_create_template += event_create_template_1
                event_create_template += event_create_template_2
                if(randint(0, 2) == 0): 
                    event_create_template += event_create_template_3
                event_create_template += event_create_template_4
                if(randint(0, 2) == 0):
                    event_create_template += event_create_template_5
                event_create_template += event_create_template_6
                new_line = line.replace(to_replace ,event_create_template\
                                      .replace("ID", str(n))\
                                      .replace("EVENT_TYPE", event_type)
                                      .replace("VAL_GLOBAL_TIMER_DATA", global_val_timer_data)
                                      .replace("INVALI_GLOBAL_TIMER_DATA", global_inval_timer_data)
                                      .replace("VAL_LOCAL_TIMER_DATA", local_val_timer_data)
                                      .replace("INVALI_LOCAL_TIMER_DATA", local_inval_timer_data)
                                      .replace("VAL_EVENT", valid_event_data)
                                      .replace("INVALI_EVENT", invalid_event_data)
                                      .replace("RANDOM_DATA_G", random_data)
                                      .replace("FAKE_DATA_0", convert_hex_str(token_hex(fake_data_code_0_size)))
                                      .replace("FAKE_DATA_1", convert_hex_str(token_hex(fake_data_code_1_size)))
                                      .replace("FAKE_DATA_2", convert_hex_str((token_hex(fake_data_code_2_size)))))

                line = new_line
                n += 1
            event_struct_h.write(line)


            
    
