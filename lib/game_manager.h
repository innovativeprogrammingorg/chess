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
#include "chat.h"
#include "lobby.h"

using namespace std; 
class Game_Manager{
	public:
		/**
		 * The one and only instance of this class in existence
		 * 
		 */
		static Game_Manager* GM;
		vector<Game*>* games;
		map<int64_t,Chat*>* chats;
		Chat* lobby_chat;
		Lobby* lobby;

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
	/**CHESS ACTIONS**/
	LOGIN = 0x101,
	NEW = 0x102,
	JOIN = 0x103,
	MOVE = 0x104,
	OFFER_DRAW = 0x105,
	ACCEPT_DRAW = 0x106,
	DECLINE_DRAW = 0x107,
	RESIGN = 0x108,
	TAKE_BACK = 0x109,
	ACCEPT_TAKE_BACK = 0x10A,
	DECLINE_TAKE_BACK = 0x10B,
	CANCEL_GAME = 0x10C,
	PROMOTE = 0x10D,
	REQUEST_TAKEN = 0x10F,
	TIME = 0x110,
	TURN = 0x111,
	REQUEST_MOVES = 0x112,
	BOARD = 0x113,
	PLACE_PIECE = 0x114,
	/***CHAT ACTIONS***/
	LOBBY_MESSAGE = 0x200,
	GET_LOBBY_MESSAGES = 0x201,
	CHESS_MESSAGE = 0x202,
	GET_CHESS_MESSAGES = 0x203,
	BUGHOUSE_MESSAGE = 0x204,
	GET_BUGHOUSE_MESSAGES = 0x205,
	/***LOBBY ACTIONS***/
	CREATE_LOBBY_GAME = 0x300,
	REMOVE_LOBBY_GAME = 0x301,
	GET_LOBBY_GAMES = 0x302,
	GET_LOBBY_USERS = 0x303

};


#endif