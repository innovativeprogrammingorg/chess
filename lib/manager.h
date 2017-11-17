#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <pthread.h>
#include "chess/game.h"
#include "chess/chess_types.h"
#include "json.h"
#include "user/user.h"
#include "server/client.h"
#include "server/WebSocket/frame.h"
#include "sql/sql.h"
#include "chat.h"
#include "lobby.h"
#include "chess/chess.h"
#include "game_manager.h"
#include "user/user_manager.h"

using namespace std; 
class Manager{
	public:
		/**
		 * The one and only instance of this class in existence
		 * 
		 */
		static Manager* GM;
		pthread_mutex_t* lock;
		Game_Manager* games;
		Chat* lobby_chat;
		Lobby* lobby;

		Manager();

		static string process(Client* c,string data,int command,Game* out_game = nullptr,int* sd = NULL);
		static void create_game(Chess* g);
		static Chess* find_game(int64_t id);
		static int* processMoveData(vector<string>* data);
};


/**
 * Obsolete
 */
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
	MOVE = 0x100,
	OFFER_DRAW = 0x101,
	ACCEPT_DRAW = 0x102,
	DECLINE_DRAW = 0x103,
	RESIGN = 0x104,
	TAKE_BACK = 0x105,
	ACCEPT_TAKE_BACK = 0x106,
	DECLINE_TAKE_BACK = 0x107,
	PROMOTE = 0x108,
	REQUEST_TAKEN = 0x109,
	REQUEST_MOVES = 0x10A,
	BOARD = 0x10B,
	PLACE_PIECE = 0x10C,
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
	GET_LOBBY_USERS = 0x303,
	JOIN_LOBBY_GAME = 0x304,
	GET_LOBBY_ALL = 0x305,
	/***GAME ACTIONS***/
	LOGIN = 0x400,
	NEW = 0x401,
	JOIN = 0x402,
	CANCEL_GAME = 0x403,
	TIME = 0x404,
	TURN = 0x405,
	GET_GAME_ALL = 0x406,
};


#endif