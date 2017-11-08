#ifndef _CHESS_H_
#define _CHESS_H_

#include <string>
#include <vector>
#include "game.h"
#include "chat.h"
#include "board.h"
#include "../include/str.h"
#include "../include/WebSocket/frame.h"

using namespace std;
class Chess{
	public:
		Game* game;
		Chat* chat;
		vector<string>* history;
		vector<string>* moves;
		time_t last;
		bool waiting_for_promotion;
		int promotion_row;
		int promotion_col;
		static char cmd;
		static char data_sep;
		Chess(Game* game);
		virtual ~Chess();
		void start();
		void move(int r,int c, int r2, int c2,char side);
		void offer_draw(string user);
		void accept_draw(string user);
		void decline_draw(string user);
		void resign(string user);
		void take_back(string user);
		void store(Board* board);
		void message(string user,string msg);
		void send_board();
		void send_time();
		void send_move(string move);
		void send_moves();
		void send_all();
		void invalid_move();
		void promote(char piece);

};



#endif