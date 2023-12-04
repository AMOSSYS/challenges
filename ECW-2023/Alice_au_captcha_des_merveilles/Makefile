CC = x86_64-w64-mingw32-gcc
C++ = x86_64-w64-mingw32-g++
RESOURCES_COMPILER = x86_64-w64-mingw32-windres
CFLAGS = -Wall -Wextra
LDFLAGS = -s
CCSYNTAX = -std=c11

SRC = src/
BIN = bin/

CLIENT = client
CHALLENGE = alice_au_captcha_des_merveilles
SERVER = server
OBJ = obj
RC4 = rc4
RESOURCES = resources
INCLUDES = includes

CLIENT_DIR = $(CLIENT)/
SERVER_DIR = $(SERVER)/
OBJ_DIR = $(OBJ)/
INCLUDES_DIR = $(INCLUDES)/

CLIENT_BIN = $(BIN)$(CLIENT_DIR)
SERVER_BIN = $(BIN)$(SERVER_DIR)
OBJ_BIN = $(BIN)$(OBJ_DIR)

CLIENT_SRC = $(SRC)$(CLIENT_DIR)
SERVER_SRC = $(SRC)$(SERVER_DIR)
OBJ_SRC = $(SRC)$(OBJ_DIR)

CLIENT_TARGET = $(CLIENT_BIN)$(CHALLENGE).exe
SERVER_TARGET = $(SERVER_BIN)$(SERVER).exe
RC4_TARGET = $(OBJ_BIN)$(RC4).o

CLIENT_OBJS = $(RC4_TARGET)
SERVER_OBJS = $(RC4_TARGET) $(SERVER_RESOURCES_OBJ) $(SRC)$(INCLUDES_DIR)debug.h
SERVER_RESOURCES = $(SERVER_SRC)$(RESOURCES).rc
SERVER_RESOURCES_OBJ = $(OBJ_BIN)$(RESOURCES).o

create_dir: 
	@echo "Start building Challenge"
	@echo -------------------------
	@echo create base directory
	mkdir -p $(CLIENT_BIN) $(SERVER_BIN) $(OBJ_BIN)
	@echo -------------------------

$(RC4_TARGET): $(OBJ_SRC)$(RC4).c $(OBJ_SRC)$(RC4).h 
	@echo generate object file for $< "=>"
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@ 
	@echo -------------------------

start_building_client:
	@echo "Start building client" 
	@echo -------------------------

end_of_building_client:
	@echo "Client builded"
	@echo -------------------------

$(CLIENT_TARGET): $(CLIENT_SRC)$(CLIENT).c $(CLIENT_OBJS) 
	@echo generate target for $< "=>"
	$(C++) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lhttpapi -lwinhttp -loleacc -lole32 -loleaut32 -mwindows -mconsole -lbcrypt
	cp $(CLIENT_TARGET) test/
	@echo -------------------------

client: start_building_client  $(CLIENT_TARGET) end_of_building_client
 
start_building_server:
	@echo "Start building server" 
	@echo -------------------------

end_of_building_server:
	@echo "Server builded"
	@echo -------------------------

$(SERVER_TARGET):  $(SERVER_SRC)$(SERVER).c $(SERVER_OBJS)
	@echo generate target for $< "=>"
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lhttpapi
	cp $(SERVER_TARGET) test/
	@echo -------------------------

 $(SERVER_RESOURCES_OBJ) : $(SERVER_RESOURCES)
	$(RESOURCES_COMPILER) -o $@ $<


server: start_building_server $(SERVER_TARGET) end_of_building_server 


all: create_dir client server  


clean: 
	rm -f $(CLIENT_TARGET) $(SERVER_TARGET) $(OBJ_BIN)*.o
