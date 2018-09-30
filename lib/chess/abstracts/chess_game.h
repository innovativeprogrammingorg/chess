#ifndef _CHESS_ABSTRACT_CHESS_GAME_H_
#define _CHESS_ABSTRACT_CHESS_GAME_H_

#include <string>
#include <cstdint>

using namespace std;


class igame{
	public:
		virtual ~igame() = 0;
		virtual int64_t assign_ids(int64_t id);
		virtual int32_t get_type();

		virtual bool has_game(int64_t id) = 0;
		virtual void start() = 0;
		virtual void move(int row, int col, int row2, int col2,string user) = 0;
		virtual void send_all(int sd) = 0;
		virtual void offer_draw(const string& username) = 0;
		virtual void accept_draw(const string& username) = 0;
		virtual void decline_draw(const string& username) = 0;
		virtual void resign(const string& username) = 0;
		virtual void take_back(const string& username) = 0;
		virtual void message(const string& user,const string& msg) = 0;
		virtual void save() = 0;
		virtual void promote(char piece,const string& username) = 0;
		
};

#endif