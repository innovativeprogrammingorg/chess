#ifndef _CHESS_H_
#define _CHESS_H_

#include <string>
#include <vector>
#include "chess/game.h"
#include "chat.h"
#include "chess/board.h"
#include "str.h"
#include "server/WebSocket/frame.h"
#include "sql/sql.h"
#include "chess/abstracts/chess_game.h"


using namespace std;
class chess: public IGame{
	public:
		game chess_game;
		chat game_chat;
		time_t last;
		bool waiting_for_promotion;
		int promotion_row;
		int promotion_col;
		int8_t winner;
		chess(const game& g);
	private:
		void init();
	public:
		bool has_game(int64_t id);
		void start();
		void notify_sides();
		void notify_turn();
		void notify_game_over();
		void move(int r,int c, int r2, int c2,string user);
		void next(const string& mv = "");
		void offer_draw(const string& user);
		void accept_draw(const string& user);
		void decline_draw(const string& user);
		void resign(const string& user);
		void take_back(const string& user);
		void message(const string& user,string msg);
		void send_board();
		void send_time();
		void send_move(const string& move);
		void send_taken();
		void send_taken(uint8_t side,char piece);
		void send_moves();
		void send_promotion();
		void send_all(int sd);
		void broadcast(const frame& f);
		void broadcast(const string& msg);
		void invalid_move();
		void promote(char piece,const string& username);
		void save();
		char get_side_of(const string& username);

		

};



#endif