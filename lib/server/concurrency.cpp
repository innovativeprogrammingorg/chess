#include "server/concurrency.h"

Request new_request(void* c, char* data){
	Request out = (Request)malloc(sizeof(struct request));
	out->client = c;
	out->data = data;
	return out;
}

WThread new_WThread(Request r){
	WThread out = (WThread)malloc(sizeof(struct thread_wrapper));
	out->req = r;
	out->thread = (pthread_t*)malloc(sizeof(pthread_t));
	return out;
}

void handshake(Client* c, char* data,size_t size){
	//cout<<"Creating new thread to handle request"<<endl;
	char* dat = (char*)calloc(sizeof(char),size+1);
	memcpy(dat,data,size);
	//cout<<"PASSED TO PARAM:::::\n"<<dat<<endl;
	//threads++;
	pthread_t thread;
	Request r = new_request((void*)c,dat);
	pthread_create(&thread,NULL,handle_handshake,r);
}

void* handle_handshake(void * wt){
	Request req = (Request)wt;
	pthread_mutex_lock(((Client*)req->client)->lock);

	string* data = new string(req->data);
	HTTP_Request* request = new HTTP_Request(data);
	Response res = build_response(request);

	IO::respond(((Client*)req->client)->sd,res);
	((Client*)req->client)->handshaked = true;
	pthread_mutex_unlock(((Client*)req->client)->lock);
	delete data;
	delete request;
	free(req->data);
	free(req);
	if(res != nullptr){
		if(res->header != nullptr){
			delete res->header;
		}
		if(res->body != nullptr){
			free(res->body);
		}
		free(res);
	}
	
	//threads--;
	return NULL;
}

void data_frame(Client* c, char* data,size_t size){
	char* dat = (char*)calloc(sizeof(char),size+1);
	memcpy(dat,data,size);
	WThread t = new_WThread(new_request((void*)c,dat));
	//threads++;
	//cout<<"There are "<<threads<<" threads running"<<endl;
	pthread_create(t->thread,NULL,handle_data_frame,t);
}

void* handle_data_frame(void* wt){
	WThread t = (WThread)wt;
	pthread_mutex_lock(((Client*)t->req->client)->lock);
	Control::handle_request((Client*)t->req->client,t->req->data);
	//threads--;
	pthread_mutex_unlock(((Client*)t->req->client)->lock);
	free(t->req->data);
	free(t->req);
	free(t->thread);
	free(t);
}