

#CFLAGS += -Wall -Werror --pedantic
CC= g++
LIB = -I /usr/local/lib/
VERSION = c++11
CFLAGS = -std=$(VERSION)  -pthread $(LIB) 
FILES = *.cpp 
LIB_OBJ = $(foreach x, $(basename $(wildcard lib/*.cpp)), $(x).o)
INCLUDE_OBJ = $(foreach x, $(basename $(wildcard include/*.cpp)), $(x).o)
CGI_OBJ = $(foreach x, $(basename $(wildcard include/CGI*.cpp)), $(x).o)
OBJECTS =  $(LIB_OBJ) $(INCLUDE_OBJ) $(CGI_OBJ)

.PHONY: clean


all:  $(LIB_OBJ) $(INCLUDE_OBJ) $(CGI_OBJ) server

server: 
	$(CC) server_main.cpp -o server $(CFLAGS) $(OBJECTS)

clean: 
	rm -f server chess 

fclean:
	rm -f server chess $(OBJECTS)
	
re: clean all

$(LIB_OBJ): lib/%.o : lib/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(LIB) 

$(INCLUDE_OBJ): include/%.o : include/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(LIB) 

$(CGI_OBJ): include/CGI/%.o : include/CGI/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 