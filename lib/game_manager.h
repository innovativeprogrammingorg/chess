#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <vector>
#include <map>
#include <iostream>
#include <cstdint>
#include <pthread.h>

#include "chess/chess.h"
#include "sql/sql.h"
#include "chess/timer.h"

using namespace std;

class Game_Manager{
	private:
		pthread_mutex_t* lock;
		uint64_t id;
	public:
		map<int64_t,Chess*>* games;

		Game_Manager();
		virtual ~Game_Manager();
		void add_game(Chess* game);
		void add_game(Chess* game,int64_t id);
		Chess* get_game(int64_t id);
		Chess* load_game(int64_t id);
		void drop_game(int64_t id);
		void save_game(int64_t id);
		void save();
};

#endif