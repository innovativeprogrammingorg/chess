

#CFLAGS = -Wall -Werror --pedantic
CC= g++
LIB = -L/usr/local/lib/
INCLUDE = -Ilib/
VERSION = c++11
CFLAGS = -g -std=$(VERSION)  -pthread 
FILES = *.cpp 

SQL_FLAGS = -I/usr/local/include -I/usr/local/include/cppconn -Wl,-Bdynamic 

LINK_FLAGS = -lmysqlcppconn -lcrypto 

CFLAGS += $(SQL_FLAGS)

LIB_OBJ = $(foreach x, $(basename $(wildcard lib/*.cpp)), $(x).o)

WS_OBJ = $(foreach x, $(basename $(wildcard lib/server/WebSocket/*.cpp)), $(x).o)

SERVER_OBJ  = $(foreach x, $(basename $(wildcard lib/server/*.cpp)), $(x).o)

SQL_OBJ = $(foreach x, $(basename $(wildcard lib/sql/*.cpp)), $(x).o)

CHESS_OBJ = $(foreach x, $(basename $(wildcard lib/chess/*.cpp)), $(x).o)

USER_OBJ = $(foreach x, $(basename $(wildcard lib/user/*.cpp)), $(x).o)


OBJECTS =  $(LIB_OBJ) $(SERVER_OBJ) $(WS_OBJ) $(SQL_OBJ) $(CHESS_OBJ) $(USER_OBJ)

.PHONY: clean


all:  $(OBJECTS) server

server: 
	$(CC) server_main.cpp -o server $(CFLAGS) $(INCLUDE) $(LIB) $(OBJECTS) -lssl  $(LINK_FLAGS) 
clean: 
	rm -f server 

fclean:
	rm -f server  $(OBJECTS)
	
re: clean all

$(LIB_OBJ): lib/%.o : lib/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ $(LIB) 

$(SERVER_OBJ): lib/server/%.o : lib/server/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ $(LIB) 

$(WS_OBJ): lib/server/WebSocket/%.o : lib/server/WebSocket/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ -lssl -lcrypto

$(SQL_OBJ): lib/sql/%.o : lib/sql/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ $(LIB) $(LINK_FLAGS)

$(CHESS_OBJ): lib/chess/%.o : lib/chess/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ $(LIB) 

$(USER_OBJ): lib/user/%.o : lib/user/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ $(LIB) 
