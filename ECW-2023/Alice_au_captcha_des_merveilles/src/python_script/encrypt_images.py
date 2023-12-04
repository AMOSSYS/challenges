from pathlib import Path
from binascii import hexlify
from enum import Enum

class EventType(Enum):
    ALL_EVENT = 0
    LOST_MOUSE_EVENT = 1
    RECEIVE_MOUSE_EVENT = 2
    FOREGROUND_WINDOW_EVENT = 3
    MOVE_WINDOW_EVENT = 4
    OBJECT_CREATE_EVENT = 5
    OBJECT_DESTROY_EVENT = 6
    OBJECT_FOCUS_EVENT = 7
    MINIMIZE_WINDOW_START_EVENT = 8
    MINIMIZE_WINDOW_END_EVENT = 9
    MENU_SELECT_START = 10 
    MENU_CONTEXT_START = 11

class RC4:
    """
    This class implements the RC4 streaming cipher.
    """

    def __init__(self, key, streaming=True):
        assert(isinstance(key, (bytes, bytearray)))

        # key scheduling
        S = list(range(0x100))
        j = 0
        for i in range(0x100):
            j = (S[i] + key[i % len(key)] + j) & 0xff
            S[i], S[j] = S[j], S[i]
        self.S = S

        # in streaming mode, we retain the keystream state between crypt()
        # invocations
        if streaming:
            self.keystream = self._keystream_generator()
        else:
            self.keystream = None

    def crypt(self, data):
        """
        Encrypts/decrypts data (It's the same thing!)
        """
        assert(isinstance(data, (bytes, bytearray)))
        keystream = self.keystream or self._keystream_generator()
        return bytes([a ^ b for a, b in zip(data, keystream)])

    def _keystream_generator(self):
        """
        Generator that returns the bytes of keystream
        """
        S = self.S.copy()
        x = y = 0
        while True:
            x = (x + 1) & 0xff
            y = (S[x] + y) & 0xff
            S[x], S[y] = S[y], S[x]
            i = (S[x] + S[y]) & 0xff
            yield S[i]

def get_event_type_from_str(event_type_str):
    if "ALL_EVENT" in event_type_str:
        return EventType.ALL_EVENT
    elif "LOST_MOUSE_EVENT" in event_type_str:
        return EventType.LOST_MOUSE_EVENT
    elif "RECEIVE_MOUSE_EVENT" in event_type_str:
        return EventType.RECEIVE_MOUSE_EVENT
    elif "FOREGROUND_WINDOW_EVENT" in event_type_str:
        return EventType.FOREGROUND_WINDOW_EVENT
    elif "MOVE_WINDOW_EVENT" in event_type_str:
        return EventType.MOVE_WINDOW_EVENT
    elif "OBJECT_CREATE_EVENT" in event_type_str:
        return EventType.OBJECT_CREATE_EVENT
    elif "OBJECT_DESTROY_EVENT" in event_type_str:
        return EventType.OBJECT_DESTROY_EVENT
    elif "OBJECT_FOCUS_EVENT" in event_type_str:
        return EventType.OBJECT_FOCUS_EVENT
    elif "MINIMIZE_WINDOW_START_EVENT" in event_type_str:
        return EventType.MINIMIZE_WINDOW_START_EVENT
    elif "MINIMIZE_WINDOW_END_EVENT" in event_type_str:
        return EventType.MINIMIZE_WINDOW_END_EVENT
    elif "MENU_SELECT_START" in event_type_str:
        return EventType.MENU_SELECT_START
    else :
        return EventType.MENU_CONTEXT_START 


def build_key(idx, event_data_file):
    key = b''
 
    if idx < 16:
        id = bytes.fromhex(hex(idx)[::2])
    else: 
        id = bytes.fromhex(hex(idx)[2:])

    #print(idx, " ", id, " ")
    event_type_dec = get_event_type_from_str(event_data_file.readline().strip())
    event_type = bytes.fromhex(hex(event_type_dec.value)[::2])

    val_global_timer_data = bytes.fromhex(event_data_file.readline().strip())
    # not used here 
    inval_global_timer_data = bytes.fromhex(event_data_file.readline().strip())
    val_local_timer_data = bytes.fromhex(event_data_file.readline().strip())
    # not used here 
    inval_local_timer_data = bytes.fromhex(event_data_file.readline().strip())
    val_event = bytes.fromhex(event_data_file.readline().strip())
    # not used here 
    inval_event = bytes.fromhex(event_data_file.readline().strip())
    random_data = bytes.fromhex(event_data_file.readline().strip())

    key += id + event_type + val_global_timer_data + val_local_timer_data + val_event + random_data
    

    return key

if __name__ == "__main__":
    current_dir = Path(__file__).parent.absolute()
    renders_dir = current_dir / "renders"
    server_dir = Path(__file__).parent.parent.absolute() / "server"
    images_dir = server_dir / "images" 
    encrypted_images_dir = server_dir / "encrypted_images"
    with open(str(renders_dir / "event_data.txt"), "r") as event_data_file:
        for idx in range(100):
            with open(str(images_dir / f"img_{idx}.jpeg"), "rb") as current_image,\
                open(str(encrypted_images_dir / f"encrypted_img_{idx}.enc"), "wb") as current_encrypted_images:
                key = build_key(idx, event_data_file)
                current_image_content = current_image.read()
                cipher = RC4(key, streaming=True)
                print(f"key = {hexlify(key)}, id = {idx}")
                encrypted_images_content = cipher.crypt(current_image_content)
                current_encrypted_images.write(encrypted_images_content)





















