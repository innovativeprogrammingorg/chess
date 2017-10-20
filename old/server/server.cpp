#include "server.h"


void process_request(char* request, Vector* games, Map* users,int fd,int id){
	Vector data = explode("#",request);
	if(data == NULL){
		server_handshake(request,fd);
		return;
	}else{
		printf("Data is not null\n");
	}
	printf("Got the data on the request, number of elements is %lu\n",vector_length(data));
	int cmd = ((char*)vector_get(data,0))[0] - 48;
	int64_t gid = map_lookup(*users,id);
	switch(cmd){
		case START_GAME:
			map_update(users,id,atoi(vector_get(data,ID_INDEX)));
			start_request(data,games,fd,id);
			break;
		case MOVE:
			move_request(data,games,fd,gid);
			break;
		case STATUS:
			status_request(games,fd,gid);
			break;
		case UNDO: 
			undo_request(data,games,fd,gid);
			break;
		case PLACE:
			place_request(data,games,fd,gid);
			break;
		case UPDATE_GAME:
			update_request(data,games,fd,gid);
			break;
		default:
			return;
	}
}

void start_request(Vector data, Vector* games, int fd, int id){
	uint8_t new_game = ((char*)vector_get(data,NEW_INDEX))[0] - 48;
	if(new_game){
		create_game(data,games,fd,id);
	}else{
		join_game(data,games,fd,id);
	}
}



void move_request(Vector data, Vector* games, int fd, int id){
	printf("A move has been requested!\n");
	int64_t index = find_game(*games,id);
	Game g = (Game)vector_get(*games,index);
	Board b = g->b;
	char side = ((char*)vector_get(data,1))[0];
	char* dat;
	if(side!=g->turn){
		write(fd,INVALID_TURN,strlen(INVALID_TURN));
		return;
	}
	int* m = processMoveData(data);
	uint8_t success = handle_move(m,g);
	if(!success){
		write(fd,INVALID_MOVE,strlen(INVALID_MOVE));
	}else{
		dat = getBoardData(b);
		write(fd,dat,strlen(dat));
	}
}


void status_request(Vector* games, int fd, int id){
	char* data = get_game_state(games,id);
	write(fd,data,strlen(data));//sends the game state to the user
	return;
}
void undo_request(Vector data, Vector* games, int fd, int id){
	printf("An undo move has been requested\n");
}
void place_request(Vector data, Vector* games, int fd, int id){
	
}
void update_request(Vector data, Vector* games, int fd, int id){
	//IMPLEMENT LATER
}

char* formatTime(uint64_t t){
	uint64_t seconds = t%60;
	uint64_t minutes = (t-seconds)/60;
	char* out = (char*)malloc(sizeof(char)*10);
	size_t length;
	if(seconds<10){
		length = sprintf(out,"%lu:0%lu",minutes,seconds);
		out[length] = '\0';
	}else{
		length = sprintf(out,"%lu:%lu",minutes,seconds);
		out[length] = '\0';
	}
	return out;
}



