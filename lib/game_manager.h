#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_


#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <stdarg.h>
#include "game.h"
#include "types.h"
#include "json.h"
#include "user.h"
#include "../include/client.h"
#include "sql/sql.h"

using namespace std; 
class Game_Manager{
	public:
		/**
		 * The one and only instance of this class in existence
		 * 
		 */
		static Game_Manager* GM;
		vector<Game*>* games;

		Game_Manager();

		static string prepare_message(int args,...);
		static string process(Client* c,string data,int command,Game* out_game = nullptr,int* sd = NULL);
		static void create_game(vector<string>* data,int sd);
		static void join_game(vector<string>* data,int sd);
		static string get_game_state( int64_t gid);
		static int64_t find_game(int64_t id);
		static int64_t find_game(string username);
		static int* processMoveData(vector<string>* data);
		static void disconnectClient(int64_t id);
};


#ifndef COMMAND
#define COMMAND '\005'
#endif

#ifndef DATA_SEP
#define DATA_SEP '\037'
#endif



enum indexes{
	SIDE_INDEX = 0x0,
	USER_INDEX = 0x01,
	ID_INDEX = 0x02,
	FEN_INDEX = 0x03,
	TURN_INDEX = 0x04,
	INC_INDEX = 0x05,
	WTIME_INDEX = 0x06,
	BTIME_INDEX = 0x07
};


/**
 * LIST OF POTENTIAL ACTIONS AND THE ASSOCIATED CONSTANT
 */

enum action{
	LOGIN = 0xF1,
	NEW = 0xF2,
	JOIN = 0xF3,
	MOVE = 0xF4,
	OFFER_DRAW = 0xF5,
	ACCEPT_DRAW = 0xF6,
	DECLINE_DRAW = 0xF7,
	RESIGN = 0xF8,
	TAKE_BACK = 0xF9,
	ACCEPT_TAKE_BACK = 0xFA,
	DECLINE_TAKE_BACK = 0xFB,
	CANCEL_GAME = 0xFC,
	PROMOTE = 0xFD,
	REQUEST_TAKEN = 0xFF,
	TIME = 0x100,
	TURN = 0x101,
	REQUEST_MOVES = 0x102,
	BOARD = 0x103
};


#endif