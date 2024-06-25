CC = gcc
CFLAGS = -Wall -Iinclude
OBJ_DIR = obj
BIN_DIR = bin
 
SERVER_SRC = src/server.c
CLIENT_SRC = src/client.c
 
SERVER_OBJ = $(OBJ_DIR)/server.o
CLIENT_OBJ = $(OBJ_DIR)/client.o
 
SERVER_EXEC = $(BIN_DIR)/server
CLIENT_EXEC = $(BIN_DIR)/client
 
all: $(BIN_DIR) $(SERVER_EXEC) $(CLIENT_EXEC)
 
$(BIN_DIR):
	mkdir -p $(BIN_DIR)
 
$(SERVER_EXEC): $(SERVER_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lpthread
 
$(CLIENT_EXEC): $(CLIENT_OBJ)
	$(CC) $(CFLAGS) $^ -o $@
 
$(SERVER_OBJ): $(SERVER_SRC)
	$(CC) $(CFLAGS) -c $< -o $@
 
$(CLIENT_OBJ): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -c $< -o $@
 
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/server $(BIN_DIR)/client out.txt
