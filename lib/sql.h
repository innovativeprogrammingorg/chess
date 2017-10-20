#ifndef SQL_H
#define SQL_H

#include "chess.h"

#define White_Captured 0
#define Black_Captured 1

void update_database(Game g);
char* create_query1(Game g);
void update_query(char* query);
char** fetch_game_data(uint64_t id);
#endif