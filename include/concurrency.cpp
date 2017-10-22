#include "concurrency.h"

WThread new_WThread(Request r){
	WThread out = (WThread)malloc(sizeof(struct thread_wrapper));
	out->req = r;
	out->thread = (pthread_t*)malloc(sizeof(pthread_t));
	return out;
}

void create_new_thread(Client c, char* data){
	char* dat = (char*)calloc(sizeof(char),strlen(data)+1);
	strcpy(dat,data);
	WThread t = new_WThread(new_request(c,dat));
	pthread_create(t->thread,NULL,handle_request,t);
}

void * handle_request(void * wt){
	WThread t = (WThread)wt;
	respond(t->req->client->fd,build_response(parse_HTTP_message(t->req->data)));
	free(t->req->data);
	free(t->req);
	free(t->thread);
	free(t);
	return NULL;
}