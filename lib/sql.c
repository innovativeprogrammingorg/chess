#include "sql.h"

static char* server = "localhost";
static char* user = "admin172";
static char* password = "hB1lT^F+,O9T";
static char* database = "chessClub";
void update_query(char* query){
	MYSQL* conn;
    conn = mysql_init(NULL);
    /* Connect to database */
    if(!mysql_real_connect(conn, server,
        user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }

    /* send SQL query */
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
    mysql_close(conn);
}


void update_database(Game g){
	MYSQL* conn;
    char* query = create_query1(g);
    conn = mysql_init(NULL);
    /* Connect to database */
    if(!mysql_real_connect(conn, server,
        user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }

    /* send SQL query */
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }
    free(query);
    mysql_close(conn);
}

char* create_query1(Game g){
	char* out = (char*)malloc(sizeof(char)*8000);
	uint64_t w_sec = g->white_time % 60;
	uint64_t w_min = (g->white_time - w_sec)/60;
	uint64_t b_sec = g->black_time % 60;
	uint64_t b_min = (g->black_time - b_sec)/60;
	char wt[10];
	char bt[10];
	size_t length;
	if(w_sec<10){
		length = sprintf(wt,"%lu:0%lu",w_min,w_sec);
		wt[length] = '\0';
	}else{
		length = sprintf(wt,"%lu:%lu",w_min,w_sec);
		wt[length] = '\0';
	}
	if(b_sec<10){
		length = sprintf(bt,"%lu:0%lu",b_min,b_sec);
		bt[length] = '\0';
	}else{
		length = sprintf(bt,"%lu:%lu",b_min,b_sec);
		bt[length] = '\0';
	}

	sprintf(out,"UPDATE chessgame SET Turn = %c, WTime = '%s', BTime = '%s',Time = %lu,Castle = %s,Board = %s WHERE ID = %lu",
		g->turn,wt,bt,g->start,getCastleData(g->b),generateFEN(g->b),g->id);
	return out;
	
}

char** fetch_game_data(uint64_t id){

	return NULL;
}