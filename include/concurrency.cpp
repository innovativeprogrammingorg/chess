#include "concurrency.h"

WThread new_WThread(Request r){
	WThread out = (WThread)malloc(sizeof(struct thread_wrapper));
	out->req = r;
	out->thread = (pthread_t*)malloc(sizeof(pthread_t));
	return out;
}

void handshake(Client* c, char* data){
	//cout<<"Creating new thread to handle request"<<endl;
	char* dat = (char*)calloc(sizeof(char),strlen(data)+1);
	strcpy(dat,data);
	//cout<<"PASSED TO PARAM:::::\n"<<dat<<endl;
	WThread t = new_WThread(new_request((void*)c,dat));
	pthread_create(t->thread,NULL,handle_handshake,t);
}

void* handle_handshake(void * wt){
	WThread t = (WThread)wt;
	//cout<<"Preparing to respond"<<endl;
	pthread_mutex_lock(((Client*)t->req->client)->lock);
	string* data = new string(t->req->data);
	HTTP_Request* request = new HTTP_Request(data);
	IO::respond(((Client*)t->req->client)->sd,build_response(request));
	((Client*)t->req->client)->handshaked = true;
	pthread_mutex_unlock(((Client*)t->req->client)->lock);
	delete data;
	delete request;
	free(t->req->data);
	free(t->req);
	free(t->thread);
	free(t);
	return NULL;
}

void data_frame(Client* c, char* data,size_t size){
	char* dat = (char*)calloc(sizeof(char),size+1);
	memcpy(dat,data,size);
	WThread t = new_WThread(new_request((void*)c,dat));
	pthread_create(t->thread,NULL,handle_data_frame,t);
}

void* handle_data_frame(void* wt){
	WThread t = (WThread)wt;
	pthread_mutex_lock(((Client*)t->req->client)->lock);
	Control::handle_request((Client*)t->req->client,t->req->data);
	pthread_mutex_unlock(((Client*)t->req->client)->lock);
	free(t->req->data);
	free(t->req);
	free(t->thread);
	free(t);
}