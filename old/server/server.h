#ifndef SERVER_H
#define SERVER_H


#include "../chess.h"
#include "header.h"
#include "connection.h"

//EXAMPLE DATA STREAM START_GAME#NEW_GAME?#SIDE#USERNAME#GAME_ID#FEN#TURN#INC#DURATION#WHITE_TIME#BLACK_TIME


void process_request(char* request, Vector* games, Map* users,int fd,int id);
void move_request(Vector data, Vector* games, int fd, int id);
void start_request(Vector data, Vector* games, int fd, int id);
void status_request(Vector* games, int fd, int id);
void undo_request(Vector data, Vector* games, int fd, int id);
void place_request(Vector data, Vector* games, int fd, int id);
void update_request(Vector data, Vector* games, int fd, int id);


char* formatTime(uint64_t t);
//void start_chess();


struct sockaddr_in init_connection(int master_socket);
void* handleRequest(void* in);
uint8_t stalemate(Vector moves);
uint8_t handle_move(int* mv,Game g);
void handle_win(Game g);
void handle_draw(uint64_t id);






#define START_GAME 1
#define MOVE 2
#define STATUS 3
#define UNDO 4
#define PLACE 5
#define UPDATE_GAME 6

#define NEW_GAME 1
#define OLD_GAME 0

#define NEW_INDEX 1
#define SIDE_INDEX 2
#define USER_INDEX 3
#define ID_INDEX 4
#define FEN_INDEX 5
#define TURN_INDEX 6
#define INC_INDEX 7
#define DUR_INDEX 8
#define WT_INDEX 9
#define BT_INDEX 10
#define SPECIAL_INDEX 11
#define CASTLE_INDEX 12
#define BUFFER 8192
#define MAX_CONNECTIONS 30

#define INVALID_TURN "ERROR:IT IS NOT YOUR TURN!"
#define INVALID_MOVE "ERROR:INVALID MOVE"

#endif