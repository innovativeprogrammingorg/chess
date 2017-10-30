

#CFLAGS = -Wall -Werror --pedantic
CC= g++
LIB = -I /usr/local/lib/
VERSION = c++11
CFLAGS = -std=$(VERSION)  -pthread 
FILES = *.cpp 
LIB_OBJ = $(foreach x, $(basename $(wildcard lib/*.cpp)), $(x).o)

WS_OBJ = $(foreach x, $(basename $(wildcard include/WebSocket/*.cpp)), $(x).o)

INCLUDE_OBJ  = $(foreach x, $(basename $(wildcard include/*.cpp)), $(x).o)

SQL_OBJ = $(foreach x, $(basename $(wildcard lib/sql/*.cpp)), $(x).o)

OBJECTS =  $(LIB_OBJ) $(INCLUDE_OBJ) $(WS_OBJ) $(SQL_OBJ)

.PHONY: clean


all:  $(LIB_OBJ) $(INCLUDE_OBJ) $(WS_OBJ) $(SQL_OBJ) server

server: 
	$(CC) server_main.cpp -o server $(CFLAGS) $(OBJECTS) -lssl -lcrypto

clean: 
	rm -f server chess 

fclean:
	rm -f server chess $(OBJECTS)
	
re: clean all

$(LIB_OBJ): lib/%.o : lib/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(LIB) 

$(INCLUDE_OBJ): include/%.o : include/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(LIB) 

$(WS_OBJ): include/WebSocket/%.o : include/WebSocket/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -lssl -lcrypto

$(SQL_OBJ): lib/sql/%.o : lib/sql/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(LIB) 
