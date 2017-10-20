#include "server.h"

uint8_t stalemate(Vector moves){
	size_t length = vector_length(moves);
	char* lastsix[12];
	size_t j = 0;
	for(size_t i = 2;i<=13;i++){
		lastsix[j] = vector_get(moves,length-i);
		j++;
		if(lastsix[j]==NULL){
			return FALSE;
		}
	}
	if(lastsix[0]==lastsix[4] && lastsix[0]==lastsix[8]
	&& lastsix[1]==lastsix[5] && lastsix[1]==lastsix[9]
	&& lastsix[3]==lastsix[7] && lastsix[3]==lastsix[11]
	&& lastsix[2]==lastsix[6] && lastsix[2]==lastsix[10]){
		return TRUE; 
	}
	return FALSE;
}

uint8_t handle_move(int* mv,Game g){	
	int row = mv[0];
	int col = mv[1];//The column of the move destination
	int sRow = mv[2];//The row of the move origin
	int sCol = mv[3];//The column of the move origin
	char moves[100];
	size_t length;
	Board b = g->b;//creates an instance of the game engine with the current board data
	uint8_t valid;
	char* FEN;
	uint8_t out;
	char query[4000];
	char pieces[400];
	char** data = fetch_game_data(g->id);
	
	valid = move(b,sRow,sCol,row,col,data[EXIT_SUCCESS][0]);//sees if the move is valid, performs it if true, returns false if not
	out = FALSE; //sets the default output to a failed move
	/************************************ Begin handling the data for the move *****************************************/
	if(valid){
		length = sprintf(moves,"%c%c%d",getTile(b,sRow,sCol)->p->FEN,numToCol(col),row);
		moves[length] = '\0';
		
		/*********If the move involves a promotion, then trigger the promotion event*********/
		if(valid != TRUE){
			FEN = generateFEN(b);
			sprintf(query,"UPDATE chessgame SET Promote=1, Board='%s' WHERE ID=%lu",FEN,g->id);
			query[length] = '\0';
			update_query(query);
			memset(query,0,length);
		}

		length = sprintf(query,"UPDATE chessgame SET Past='%s' WHERE ID='%lu'",generateFEN(b),g->id);
		query[length] = '\0';
		update_query(query);
		memset(query,0,length);
		/***************************Handle the Castling Notation******************************/
		if(isPiece(getTile(b,row,col)->p,KING)){
			if(col - sCol == 2){//king side
				strcpy(moves,"o-o");
				moves[3] = '\0';
			}
			if(col - sCol == -2){//queen side
				strcpy(moves,"o-o-o");
				moves[5] = '\0';
			}
		}
		/************************************If a piece has been taken, store it***********************************/
		if(b->taken){
			length = sprintf(query,"UPDATE chessgame SET Last_Taken = '%c' WHERE ID='%lu'",b->taken,g->id);//store the last piece taken
			query[length] = '\0';
			update_query(query);
			memset(query,0,length);
			if(g->turn == WHITE){
				length = sprintf(pieces,"%s|%c",data[White_Captured],b->taken);
				pieces[length] = '\0';
				length = sprintf(query,"UPDATE chessgame SET White_Captured='%s'WHERE ID='%lu'",pieces,g->id);
				query[length] = '\0';
				update_query(query);
				memset(query,0,length);
			}else{
				length = sprintf(pieces,"%s|%c",data[Black_Captured],b->taken);
				pieces[length] = '\0';
				length = sprintf(query,"UPDATE chessgame SET Black_Captured='%s'WHERE ID='%lu'",pieces,g->id);
				query[length] = '\0';
				update_query(query);
				memset(query,0,length);
			}
		}
	
	/************************************ Begin handling the data for the move *****************************************/
	
		length = sprintf(moves,"%c%c%d",getTile(b,sRow,sCol)->p->FEN,numToCol(col),row);
		moves[length] = '\0';
		vector_push(&(b->history),strcpy(malloc(sizeof(char)*length+1),moves));
		FEN = generateFEN(b);
		g->turn = otherSide(g->turn);
		/*********************Stores the special pawn data, as well as the value of the opponents time necessary for the undo function *********************/
		if(g->turn == WHITE){
			length = sprintf(query,"UPDATE chessgame SET Undo_Time = '%s',SET WEP='%s' WHERE ID='%lu'",formatTime(g->white_time),b->special,g->id);
			query[length] = '\0';
			update_query(query);
			memset(query,0,length);
		}else{
			length = sprintf(query,"UPDATE chessgame SET Undo_Time = '%s',SET BEP='%s' WHERE ID='%lu'",formatTime(g->black_time),b->special,g->id);
			query[length] = '\0';
			update_query(query);
			memset(query,0,length);
		}
		/************************************Handles the Notation for Check************************************/
		if(inCheck(b,g->turn)){
			length = sprintf(moves,"%s+,",moves);
			moves[length] = '\0';
		}else{
			length = sprintf(moves,"%s,",moves);
			moves[length] = '\0';
		}
		/**********************************Checks for then handles Checkmate*****************************************/
		if(isCheckmate(b,g->turn)){
			handle_win(g);
		}
		/******************************Detects and Handles a draw by material or moves********************************/
		if(isDraw(b) || stalemate(b->history) ){
			handle_draw(g->id);
		}
		/**************************Increments the number of turns, telling the game that there has been a change**************************/
		g->turns++;
		/***************************Updates the rest of the game data**************************/
		g->start = time(NULL);
		length = sprintf(query,"UPDATE chessgame SET Time=%lu,moves='%s', Board='%s', Turn='%c',Castle='%s',turns=%lu WHERE ID=%lu",
		g->start,moves,FEN,g->turn,getCastleData(b),g->turns,g->id);
		query[length] = '\0';
		update_query(query);
		memset(query,0,length);
		out = TRUE;
	}
	return out;
}


void handle_win(Game g){
	
}
void handle_draw(uint64_t id){
	
}