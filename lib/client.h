#ifndef CLIENT_H
#define CLIENT_H

#include "chess.h"

User createUser(char* name);
Game createGame(User black, User white, uint64_t id, Board b,uint8_t turn,time_t duration,uint64_t inc);
Client createClient(uint32_t addr,in_port_t port,uint32_t sd);
void create_game(Vector data,Vector* games,int fd,int id);
void join_game(Vector data,Vector* games,int fd,int id);
char* get_game_state(Vector* games, int64_t gid);
int64_t find_game(Vector v,uint64_t id);
int* processMoveData(Vector data);
void disconnectClient(int64_t id,Vector* games);
void store_game(Game g);

#endif