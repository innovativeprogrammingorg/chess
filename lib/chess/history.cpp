#include "chess/history.h"

using namespace std;


History::History(){
	this->white_taken = new vector<char>();
	this->black_taken = new vector<char>();
	this->moves = new vector<string>();
	this->past = new vector<string>();
	this->turns = 0;
}

History::History(string past,string moves,string white_taken,string black_taken,int turns){
	this->past = new vector<string>();
	if(past.size()>0){
		this->past->push_back(past);
	}
	if(moves.size()>0){
		this->moves = c_explode(DATA_SEP,moves);
	}else{
		this->moves = new vector<string>();
	}
	this->white_taken = new vector<char>();
	this->black_taken = new vector<char>();
	for(int i = 0;i<white_taken.size();i++){
		this->white_taken->push_back(white_taken[i]);
	}
	for(int i = 0;i<black_taken.size();i++){
		this->black_taken->push_back(black_taken[i]);
	}
	this->turns = turns;
}

History::~History(){
	delete this->white_taken;
	delete this->black_taken;
	delete this->moves;
	delete this->past;
}


string History::get_white_taken(){
	if(this->white_taken->size() == 0){
		return "";
	}
	string out = "";
	for(auto it = this->white_taken->begin();it != this->white_taken->end();it++){
		if(it != this->white_taken->begin()){
			out += DATA_SEP;
		}
		out += *it;
	}
	return out;
}

string History::get_black_taken(){
	if(this->black_taken->size() == 0){
		return "";
	}
	string out = "";
	for(auto it = this->black_taken->begin();it != this->black_taken->end();it++){
		if(it != this->black_taken->begin()){
			out += DATA_SEP;
		}
		out += *it;
	}
	return out;
}

string History::get_moves(){
	if(this->moves->size() == 0){
		return "";
	}
	string out = "";
	for(auto it = this->moves->begin();it != this->moves->end();it++){
		if(it != this->moves->begin()){
			out += DATA_SEP;
		}
		out += *it;
	}
	return out;
}

string History::get_past(){
	if(this->past->size() == 0){
		return "";
	}
	return this->past->back();
}

void History::add_white_taken(char p){
	this->white_taken->push_back(p);
}

void History::add_black_taken(char p){
	this->white_taken->push_back(p);
}

void History::add_move(string move){
	this->moves->push_back(move);
	this->turns++;
}

void History::add_past(string past){
	this->past->push_back(past);
}

void History::remove_last_white_taken(){
	if(this->white_taken->size()>0){
		this->white_taken->pop_back();
	}
}

void History::remove_last_black_taken(){
	if(this->black_taken->size()>0){
		this->black_taken->pop_back();
	}
}

void History::remove_last_move(){
	if(this->moves->size()>0){
		this->moves->pop_back();
		this->turns--;
	}
}

void History::remove_last_past(){
	if(this->past->size()>0){
		this->past->pop_back();
	}
}

bool History::has_white_taken(){
	return this->white_taken->size() > 0;
}

bool History::has_black_taken(){
	return this->black_taken->size() > 0;
}

bool History::has_moves(){
	return this->moves->size() > 0;
}

bool History::has_past(){
	return this->past->size() > 0;
}