#include "client.h"

Game createGame(User black, User white, uint64_t id, Board b,uint8_t turn,time_t duration,uint64_t inc){
	Game out = (Game)malloc(sizeof(struct game));
	out->b = b;
	out->black = black;
	out->white = white;
	out->turn = turn;
	out->duration = duration;
	out->inc = inc;
	out->white_time = 0;
	out->black_time = 0;
	out->start = 0;
	out->id = id;
	return out;
}

User createUser(char* name){
	User out = (User)malloc(sizeof(struct user));
	out->username = name;
	out->online = TRUE;
	out->response = time(NULL);
	return out;
}

Client createClient(uint32_t addr,in_port_t port,uint32_t sd){
	Client out = (Client)malloc(sizeof(struct client));
	out->addr = addr;
	out->port = port;
	out->sd = sd;
	return out;
}

int64_t find_game(Vector v,uint64_t id){
	Vector t = v;
	int64_t i = 0;
	while(t!=NULL){
		if(((Game)t->data)->id == id){
			return i;
		}
		i++;
		t = t->next;
	}
	return -1;
}

void create_game(Vector data,Vector* games,int fd,int id){
	char side = ((char*)vector_get(data,SIDE_INDEX))[0];
	char* username = vector_get(data,USER_INDEX);
	uint64_t gid = atoi(vector_get(data,ID_INDEX));
	char* FEN = vector_get(data,FEN_INDEX);
	char turn = ((char*)vector_get(data,TURN_INDEX))[0];
	uint64_t inc = atoi(vector_get(data,INC_INDEX));
	char* castle = "bkbqwkwq";
	uint64_t duration = atoi(vector_get(data,DUR_INDEX));
	Board b = createBoard(FEN,NULL,castle);
	User u = createUser(username);
	Game g = createGame(NULL,NULL,gid,b,turn,duration,inc);
	if(side == WHITE){
		g->white = u;
	}else{
		g->black = u;
	}
	vector_push(games,g);
}

void join_game(Vector data,Vector* games,int fd,int id){
	uint64_t gid = atoi(vector_get(data,3));
	int64_t index = find_game(*games,gid);
	if(index == -1){
		create_game(data,games,fd,id);
		return;
	}
	Game g = vector_get(*games,index);
	char side = ((char*)vector_get(data,1))[0];
	if(side == WHITE){
		g->white = vector_get(data,2);
	}else{
		g->black = vector_get(data,2);
	}
}

char* get_game_state(Vector* games, int64_t gid){
	JSON data = createJSON("string");
	int64_t index = find_game(*games,gid);
	if(index == -1){
		return NULL;
	}
	Game g = vector_get(*games,index);
	addss(data,"Black",g->black->username);
	addss(data,"White",g->white->username);
	addss(data,"Board",getBoardData(g->b));
	addss(data,"Turn",(char*)&(g->turn));
	addss(data,"Start",itoa(g->start));
	addss(data,"Duration",itoa(g->duration));
	addss(data,"Inc",itoa(g->inc));
	addss(data,"WTime",itoa(g->white_time));
	addss(data,"BTime",itoa(g->black_time));
	addss(data,"ID",itoa(gid));
	return jsonToString(data);
}

int* processMoveData(Vector data){
	int* out = (int*)malloc(sizeof(int)*4);
	uint8_t offset = 2;
	out[0] = ((char*)vector_get(data,offset))[0];
	out[1] = ((char*)vector_get(data,offset+1))[0];
	out[2] = ((char*)vector_get(data,offset+2))[0];
	out[3] = ((char*)vector_get(data,offset+3))[0];
	return out;
}

void disconnectClient(int64_t id,Vector* games){
	int64_t index = find_game(*games,id);
	Game g = vector_get(*games,index);
	store_game(g);

}

void store_game(Game g){
	
}