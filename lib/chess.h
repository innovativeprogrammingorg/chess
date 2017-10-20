#ifndef CHESS_H
#define CHESS_H

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <sys/types.h>

#include <stdint.h>

#include <arpa/inet.h>    
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <mysql/mysql.h>
#include <websock/websock.h>

#include <vector>
#include "types/types.h"
#include "utils.h"
#include "str.h"
#include "tile.h"
#include "move.h"
#include "map.h"
#include "json.h"
#include "server/connection.h"
#include "client.h"
#include "server/server.h"
#include "sql.h"




/**Tile**/


/**Piece**/


/**Game**/


/**Location**/




 //AOST ---> A: Pawn->Bishop O: Pawn->Knight S: Pawn->Rook T: Pawn->Queen 
	//uppercase for white, lowercase for black

#endif