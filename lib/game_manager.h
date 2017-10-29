#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_


#include <string>
#include <vector>
#include <cstdint>
#include "game.h"
#include "types.h"
#include "json.h"
#include "user.h"
#include "../include/client.h"

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
		static string process(Client* c,string data,int command,Game* out_game = nullptr);
		static void create_game(vector<string>* data,int sd);
		static void join_game(vector<string>* data,int sd);
		static string get_game_state( int64_t gid);
		static int64_t find_game(int64_t id);
		static int64_t find_game(string username);
		static int* processMoveData(vector<string>* data);
		static void disconnectClient(int64_t id);
};

#ifndef SIDE_INDEX
#define SIDE_INDEX 0x00
#endif

#ifndef USER_INDEX
#define USER_INDEX 0x01
#endif

#ifndef ID_INDEX
#define ID_INDEX 0x02
#endif

#ifndef FEN_INDEX
#define FEN_INDEX 0x03
#endif

#ifndef TURN_INDEX
#define TURN_INDEX 0x04
#endif

#ifndef INC_INDEX
#define INC_INDEX 0x05 
#endif

#ifndef DUR_INDEX
#define DUR_INDEX 0x06
#endif

#ifndef COMMAND
#define COMMAND '\005'
#endif

#ifndef DATA_SEP
#define DATA_SEP '\037'
#endif

/**
 * LIST OF POTENTIAL ACTIONS AND THE ASSOCIATED CONSTANT
 */

enum action{
		LOGIN = 0xF1,
		NEW = 0xF2,
		JOIN = 0xF3,
		MOVE = 0xF4,
		OFFER_DRAW = 0xF5, ACCEPT_DRAW = 0xF6, DECLINE_DRAW = 0xF7,
			 RESIGN = 0xF8, TAKE_BACK = 0xF9, ACCEPT_TAKE_BACK = 0xFA,
			 DECLINE_TAKE_BACK = 0xFB, CANCEL_GAME = 0xFC, PROMOTE = 0xFD,
			 REQUEST_TAKEN = 0xFF};


#endif