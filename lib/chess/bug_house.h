#ifndef _BUG_HOUSE_H_
#define _BUG_HOUSE_H_

#include <pthread.h>
#include <string>
#include <vector>
#include <memory>
#include "chess/game.h"
#include "chess/chess.h"
#include "chess/abstracts/chess_game.h"

using namespace std;

class Bug_House : public IGame{
	private:
		pthread_mutex_t* lock;
	public:
		vector<unique_ptr<Chess>>* games;

		Bug_House();
		Bug_House(vector<unique_ptr<Chess>>* games);
		~Bug_House();
		bool has_game(int64_t id);
		void start();
		void move(int row, int col, int row2, int col2,string user);
		void send_all(int sd);
		void offer_draw(string username);
		void accept_draw(string username);
		void decline_draw(string username);
		void resign(string username);
		void take_back(string username);
		void message(string user,string msg);
		void save();
		void promote(char piece,string username);
};

#endif