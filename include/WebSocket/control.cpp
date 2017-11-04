#include "control.h"

using namespace std;

Frame_Buffer::~Frame_Buffer(){
	delete this->data;
}

Control* Control::control = new Control();

Control::Control(){
	this->data = new map<Client*, Frame_Buffer*>();
}

void Control::flush_frames(Client* c){
	for(auto it = Control::control->data->begin();it!=Control::control->data->end();it++){
		if(it->first->fd == c->fd){
			Control::control->data->erase(it);
			return;
		}
	}
}

Frame_Buffer* Control::get_frames(Client* c){
	for(auto it = Control::control->data->begin();it!=Control::control->data->end();it++){
		if(it->first->fd == c->fd){
			return it->second;
		}
	}
	return nullptr;
}


void Control::push_frame(Client* c,Frame* frame){
	Frame_Buffer* buffer = Control::get_frames(c);
	if(buffer == nullptr){
		buffer = new Frame_Buffer();
		buffer->data->push_back(frame);
		Control::control->data->insert(pair<Client*,Frame_Buffer*>(c,buffer));
	}else{
		buffer->data->push_back(frame);
	}

}

int Control::get_action(Frame* frame,string* data){
	vector<string>* extract = nullptr;
	frame->data->push_back('\0');

	string command = string((char*)frame->data->data());
	
	if(command.find(COMMAND) != string::npos){
		extract = split(COMMAND,command);
		command = extract->at(0);
	}

	if(data != nullptr && extract != nullptr){
		*data = extract->at(1);
	}
	if(extract != nullptr){
		delete extract;
	}
	char first = (char)command[0];

	switch(first){
		case 'A':
		{
			if(command.compare("ACCEPT_DRAW")==0)
				return ACCEPT_DRAW;
			if(command.compare("ACCEPT_TAKE_BACK")==0)
				return ACCEPT_TAKE_BACK;
			break;	
		}
		case 'B':
		{
			if(command.compare("BOARD")==0)
				return BOARD;
			if(command.compare("BUGHOUSE_MESSAGE")==0)
				return BUGHOUSE_MESSAGE;
			break;
		}
		case 'C':
		{
			if(command.compare("CREATE")==0)
				return NEW;
			if(command.compare("CANCEL_GAME")==0)
				return CANCEL_GAME;
			if(command.compare("CHESS_MESSAGE")==0)
				return CHESS_MESSAGE;
			if(command.compare("CREATE_LOBBY_GAME")==0)
				return CREATE_LOBBY_GAME;
			break;
		}
		case 'D':
		{
			if(command.compare("DECLINE_DRAW")==0)
				return DECLINE_DRAW;
			if(command.compare("DECLINE_TAKE_BACK")==0)
				return DECLINE_TAKE_BACK;
			break;
		}
		case 'G':
		{
			if(command.compare("GET_LOBBY_MESSAGES")==0)
				return GET_LOBBY_MESSAGES;
			if(command.compare("GET_CHESS_MESSAGES")==0)
				return GET_CHESS_MESSAGES;
			if(command.compare("GET_BUGHOUSE_MESSAGES")==0)
				return GET_BUGHOUSE_MESSAGES;
			if(command.compare("GET_LOBBY_GAMES")==0)
				return GET_LOBBY_GAMES;
			if(command.compare("GET_LOBBY_USERS")==0)
				return GET_LOBBY_USERS;
			if(command.compare("GET_LOBBY_ALL")==0)
				return GET_LOBBY_ALL;			
			break;
		}
		case 'J':
		{
			if(command.compare("JOIN")==0)
				return JOIN;
			if(command.compare("JOIN_LOBBY_GAME")==0){
				return JOIN_LOBBY_GAME;
			}
			break;
		}
		case 'L':
		{
			if(command.compare("LOGIN")==0)
				return LOGIN;
			if(command.compare("LOBBY_MESSAGE")==0)
	 			return LOBBY_MESSAGE;
			break;
		}
		case 'M':
		{
			if(command.compare("MOVE")==0)
				return MOVE;
			break;
		}
		case 'N':
		{
			if(command.compare("NEW")==0)
				return NEW;
			break;
		}
		case 'O':
		{	
			if(command.compare("OFFER_DRAW")==0)
				return OFFER_DRAW;
			break;
		}
		case 'P':
		{
			if(command.compare("PROMOTE")==0)
				return PROMOTE;
			if(command.compare("PLACE_PIECE")==0)
				return PLACE_PIECE;
			break;
		}
		case 'R':
		{
			if(command.compare("RESIGN")==0)
				return RESIGN;
			if(command.compare("REQUEST_TAKEN")==0)
				return REQUEST_TAKEN;
			if(command.compare("REQUEST_MOVES")==0)
				return REQUEST_MOVES;
			if(command.compare("REMOVE_LOBBY_GAME")==0)
				return REMOVE_LOBBY_GAME;
			break;
		}
		case 'T':
		{
			if(command.compare("TAKE_BACK")==0)
				return TAKE_BACK;
			if(command.compare("TIME")==0)
				return TIME;
			if(command.compare("TURN")==0)
				return TURN;
			break;			
		}	
	}
	return 0;
}

void Control::handle_request(Client* c,char* raw_data){
	Frame* received = new Frame((uint8_t*)raw_data);
	//received->debug();
	cout<<(char*)received->get_cstr()<<endl;
	if(received->opcode == PING){
		received->opcode = PONG;
		received->send(c->fd);
		return;
	}
	if(received->fin != 1){
		Control::push_frame(c,received);
		//expect more packets to arrive
		return;
	}
	Frame_Buffer* buffer = Control::get_frames(c);
	if(nullptr != buffer){
		//merge the fragmented frames
		for(uint32_t i = 0;i<buffer->data->size();i++){
			received->merge(buffer->data->at(i));
		}
		Control::flush_frames(c);
	}
	string data;
	int action = Control::get_action(received,&data);
	Game g;
	int sd = 0;
	string reply = Game_Manager::process(c,data,action,&g,&sd);
	if(reply.compare("") == 0){
		return;
	}
	Frame* response = new Frame();
	response->add((uint8_t*)reply.c_str());
	response->fin = 1;
	response->opcode = TEXT;
	response->mask = 0;
	//response->debug();
	//SEND REPLY
	if(sd == 0){
		response->send(c->fd);
	}else{
		response->send(sd);
	}
	delete response;
}