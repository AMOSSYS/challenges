import secrets 
from pathlib import Path

def pick_random_event_type(i):
    if i == 0: 
        return "LOST_MOUSE_EVENT" 
    if i == 1:
        return "RECEIVE_MOUSE_EVENT"
    if i == 2:
        return "FOREGROUND_WINDOW_EVENT"
    if i == 3:
        return "MENU_CONTEXT_START"
    if i == 4:
        return "MENU_CONTEXT_START"
    if i == 5:
        return "MENU_SELECT_START"
    if i == 6:
        return "OBJECT_CREATE_EVENT"
    if i == 7:
        return "OBJECT_DESTROY_EVENT"
    if i == 8:
        return "OBJECT_FOCUS_EVENT"
    if i == 9:
        return "MINIMIZE_WINDOW_START_EVENT"

    return "ALL_EVENT"

if __name__ == "__main__":
    random_data_size = 14
    local_timer_code_size = 6
    global_timer_code_size = 6
    event_size = 4
    current_dir = Path(__file__).parent.absolute()
    with open(str(current_dir / "renders" / "event_data.txt"), "w") as event_data_file:
        for i in range(100):
            event_data_file.write(pick_random_event_type(i % 10) + '\n') 
            event_data_file.write(secrets.token_hex(global_timer_code_size) + '\n')
            event_data_file.write(secrets.token_hex(global_timer_code_size) + '\n')
            event_data_file.write(secrets.token_hex(local_timer_code_size) + '\n')
            event_data_file.write(secrets.token_hex(local_timer_code_size) + '\n')
            event_data_file.write(secrets.token_hex(event_size) + '\n')
            event_data_file.write(secrets.token_hex(event_size) + '\n')
            event_data_file.write(secrets.token_hex(random_data_size) + '\n')

