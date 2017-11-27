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


using namespace std;
class Chess{
	public:
		Game* game;
		Chat* chat;
		time_t last;
		bool waiting_for_promotion;
		int promotion_row;
		int promotion_col;
		int8_t winner;
		Chess(Game* game);
		virtual ~Chess();
	private:
		void init();
	public:
		void start();
		void notify_sides();
		void notify_turn();
		void notify_game_over();
		void move(int r,int c, int r2, int c2,char side);
		void next(string mv = "");
		void offer_draw(string user);
		void accept_draw(string user);
		void decline_draw(string user);
		void resign(string user);
		void take_back(string user);
		void message(string user,string msg);
		void send_board();
		void send_time();
		void send_move(string move);
		void send_taken();
		void send_taken(uint8_t side,char piece);
		void send_moves();
		void send_promotion();
		void send_all(int sd);
		void broadcast(Frame* frame);
		void broadcast(string msg);
		void invalid_move();
		void promote(char piece);
		void save();
		char get_side_of(string username);

		

};



#endif