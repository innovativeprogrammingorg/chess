#ifndef _CHESS_HISTORY_H_
#define _CHESS_HISTORY_H_

#include <string>
#include <vector>
#include "server/WebSocket/frame.h"
#include "str.h"

using namespace std;

class History{
	public:
		vector<char>* white_taken;
		vector<char>* black_taken;
		vector<string>* moves;
		vector<string>* past;
		int turns;

		History();
		History(string past,string moves,string white_taken,string black_taken,int turns);
		virtual ~History();

		string get_white_taken();
		string get_black_taken();
		string get_moves();
		string get_past();

		void add_white_taken(char p);
		void add_black_taken(char p);
		void add_move(string move);
		void add_past(string past);

		void remove_last_white_taken();
		void remove_last_black_taken();
		void remove_last_move();
		void remove_last_past();

		bool has_white_taken();
		bool has_black_taken();
		bool has_moves();
		bool has_past();


};







#endif