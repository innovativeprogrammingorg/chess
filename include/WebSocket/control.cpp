#include "control.h"

using namespace std;

Frame_Buffer::~Frame_Buffer(){
	delete this->data;
}
map<string,int>* Control::lookup_table =  new map<string,int>();
Control* Control::control = new Control();

void Control::init_tables(){
	Control::lookup_table->insert(pair<string,int>("ACCEPT_DRAW",ACCEPT_DRAW));
	Control::lookup_table->insert(pair<string,int>("ACCEPT_TAKE_BACK",ACCEPT_TAKE_BACK));

	Control::lookup_table->insert(pair<string,int>("BOARD",BOARD));
	Control::lookup_table->insert(pair<string,int>("BUGHOUSE_MESSAGE",BUGHOUSE_MESSAGE));

	Control::lookup_table->insert(pair<string,int>("CREATE",NEW));
	Control::lookup_table->insert(pair<string,int>("CANCEL_GAME",CANCEL_GAME));
	Control::lookup_table->insert(pair<string,int>("CHESS_MESSAGE",CHESS_MESSAGE));
	Control::lookup_table->insert(pair<string,int>("CREATE_LOBBY_GAME",CREATE_LOBBY_GAME));

	Control::lookup_table->insert(pair<string,int>("DECLINE_DRAW",DECLINE_DRAW));
	Control::lookup_table->insert(pair<string,int>("DECLINE_TAKE_BACK",DECLINE_TAKE_BACK));

	Control::lookup_table->insert(pair<string,int>("GET_LOBBY_MESSAGES",GET_LOBBY_MESSAGES));
	Control::lookup_table->insert(pair<string,int>("GET_CHESS_MESSAGES",GET_CHESS_MESSAGES));
	Control::lookup_table->insert(pair<string,int>("GET_BUGHOUSE_MESSAGES",GET_BUGHOUSE_MESSAGES));
	Control::lookup_table->insert(pair<string,int>("GET_LOBBY_GAMES",GET_LOBBY_GAMES));
	Control::lookup_table->insert(pair<string,int>("GET_LOBBY_USERS",GET_LOBBY_USERS));
	Control::lookup_table->insert(pair<string,int>("GET_LOBBY_ALL",GET_LOBBY_ALL));
	Control::lookup_table->insert(pair<string,int>("GET_GAME_ALL",GET_GAME_ALL));

	Control::lookup_table->insert(pair<string,int>("JOIN",JOIN));
	Control::lookup_table->insert(pair<string,int>("JOIN_LOBBY_GAME",JOIN_LOBBY_GAME));

	Control::lookup_table->insert(pair<string,int>("LOGIN",LOGIN));
	Control::lookup_table->insert(pair<string,int>("LOBBY_MESSAGE",LOBBY_MESSAGE));

	Control::lookup_table->insert(pair<string,int>("MOVE",MOVE));

	Control::lookup_table->insert(pair<string,int>("NEW",NEW));

	Control::lookup_table->insert(pair<string,int>("OFFER_DRAW",OFFER_DRAW));

	Control::lookup_table->insert(pair<string,int>("PROMOTE",PROMOTE));
	Control::lookup_table->insert(pair<string,int>("PLACE_PIECE",PLACE_PIECE));

	Control::lookup_table->insert(pair<string,int>("RESIGN",RESIGN));
	Control::lookup_table->insert(pair<string,int>("REQUEST_TAKEN",REQUEST_TAKEN));
	Control::lookup_table->insert(pair<string,int>("REQUEST_MOVES",REQUEST_MOVES));
	Control::lookup_table->insert(pair<string,int>("REMOVE_LOBBY_GAME",REMOVE_LOBBY_GAME));

	Control::lookup_table->insert(pair<string,int>("TAKE_BACK",TAKE_BACK));
	Control::lookup_table->insert(pair<string,int>("TIME",TIME));
	Control::lookup_table->insert(pair<string,int>("TURN",TURN));

}

Control::Control(){
	this->data = new map<Client*, Frame_Buffer*>();
	Control::init_tables();
}

void Control::flush_frames(Client* c){
	for(auto it = Control::control->data->begin();it!=Control::control->data->end();it++){
		if(it->first->sd == c->sd){
			Control::control->data->erase(it);
			return;
		}
	}
}

Frame_Buffer* Control::get_frames(Client* c){
	for(auto it = Control::control->data->begin();it!=Control::control->data->end();it++){
		if(it->first->sd == c->sd){
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
	try{
		int out = Control::lookup_table->at(command);
		return out;
	}catch(const out_of_range& err){
		return 0;
	}
}

void Control::handle_request(Client* c,char* raw_data){
	Frame* received = new Frame((uint8_t*)raw_data);
	//received->debug();
	char* debug_receive = (char*)received->get_cstr();
	cout<<debug_receive<<endl;
	free(debug_receive);
	if(received->opcode == PING){
		received->opcode = PONG;
		received->send(c->sd);
		delete received;
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
		delete buffer;
	}
	string data;
	int action = Control::get_action(received,&data);
	delete received;
	Game g;
	int sd = 0;
	string reply = Manager::process(c,data,action,&g,&sd);
	if(reply.size() == 0){
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
		response->send(c->sd);
	}else{
		response->send(sd);
	}
	delete response;
}