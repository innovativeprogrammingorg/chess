#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <vector>
#include <iostream>
#include <cstdint>
#include <pthread.h>

#include "chess/chess.h"
#include "sql/sql.h"
#include "chess/timer.h"
#include "chess/history.h"
#include "chess/bug_house.h"
#include "chess/abstracts/chess_game.h"

using namespace std;

class game_manager{
	private:
		pthread_mutex_t lock;
		uint64_t id;
	public:
		vector<IGame*> games;

		game_manager();
		virtual ~game_manager();
		void add_game(const IGame& game);
		IGame* get_game(int64_t id);
		IGame* load_game(int64_t id);
		void drop_game(int64_t id);
		void save_game(int64_t id);
		void save();
};

#endif