#ifndef _CHESS_HISTORY_H_
#define _CHESS_HISTORY_H_

#include <string>
#include <vector>
#include "server/WebSocket/frame.h"
#include "str.h"

using std::string;
using std::vector;

class history{
	public:
		int turns;
		vector<char> white_taken;
		vector<char> black_taken;
		vector<string> moves;
		vector<string> past;
		history();
		history(const string& past,const string& moves,const string& white_taken,const string& black_taken,int turns);

		string get_white_taken();
		string get_black_taken();
		string get_moves();
		vector<string> get_last_n_moves(int n);
		string get_past();

		void add_white_taken(char p);
		void add_black_taken(char p);
		void add_move(const string& move);
		void add_past(const string& past);

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