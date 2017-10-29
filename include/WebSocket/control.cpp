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
	vector<string>* extract = split(COMMAND,string((char*)frame->data->data()));
	string command = extract->at(0);

	if(command.compare("LOGIN")==0){
		return LOGIN;
	}
	if(command.compare("NEW") == 0){
		return NEW;
	}
	if(command.compare("JOIN")==0){
		return JOIN;
	}
	if(command.compare("MOVE")==0){
		return MOVE;
	}
	if(command.compare("OFFER_DRAW")==0){
		return OFFER_DRAW;
	}
	if(command.compare("ACCEPT_DRAW")==0){
		return ACCEPT_DRAW;
	}
	if(command.compare("DECLINE_DRAW") == 0){
		return DECLINE_DRAW;
	}
	if(command.compare("RESIGN")==0){
		return RESIGN;
	}
	if(command.compare("TAKE_BACK")==0){
		return TAKE_BACK;
	}
	if(command.compare("ACCEPT_TAKE_BACK")==0){
		return ACCEPT_TAKE_BACK;
	}
	if(command.compare("DECLINE_TAKE_BACK")==0){
		return DECLINE_TAKE_BACK;
	}
	if(command.compare("CANCEL_GAME")==0){
		return CANCEL_GAME;
	}
	if(command.compare("PROMOTE") == 0){
		return PROMOTE;
	}
	if(command.compare("REQUEST_TAKEN") == 0){
		return REQUEST_TAKEN;
	}

	if(data != nullptr){
		*data = extract->at(0);
	}
	delete extract;

	return 0;
}

void Control::handle_request(Client* c,char* raw_data){
	Frame* received = new Frame((uint8_t*)raw_data);
	received->debug();
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
	string reply = Game_Manager::process(c,data,action,&g);
	Frame* response = new Frame();
	response->add((uint8_t*)reply.c_str());
	response->fin = 1;
	response->opcode = TEXT;
	//SEND REPLY
	response->send(c->fd);
	delete response;
}