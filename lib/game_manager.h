#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_


#include <string>
#include <vector>
#include "game.h"
#include "types.h"
#include "json.h"
#include "user.h"
#include "client.h"

class Game_Manager{
	public:
		vector<Game*>* games;

		Game_Manager();

		void create_game(vector<string>* data,int fd);
		void join_game(vector<string>* data,int fd);
		string get_game_state( int64_t gid);
		int64_t find_game(int64_t id);
		int* processMoveData(vector<string>* data);
		void disconnectClient(int64_t id);
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


#endif