#include "chess/history.h"

using std::string;
using std::vector;


history::history(): turns(0){
}

history::history(const string& past,const string& moves,const string& white_taken,const string& black_taken,int turns) : turns(turns)
{
	
	if(!past.empty()){
		this->past.push_back(past);
	}
	if(!moves.empty()){
		this->moves = explode(DATA_SEP,moves);
	}
	for(int i = 0;i<white_taken.size();i++){
		this->white_taken.push_back(white_taken[i]);
	}
	for(int i = 0;i<black_taken.size();i++){
		this->black_taken.push_back(black_taken[i]);
	}
}

string history::get_white_taken(){
	string out = "";
	if(this->white_taken.size() == 0){
		return out;
	}
	
	for(auto it = this->white_taken->begin();it != this->white_taken->end();it++){
		if(it != this->white_taken->begin()){
			out += DATA_SEP;
		}
		out += *it;
	}
	return out;
}

string history::get_black_taken(){
	string out = "";
	if(this->black_taken.size() == 0){
		return out;
	}
	
	for(auto it = this->black_taken.begin();it != this->black_taken.end();it++){
		if(it != this->black_taken.begin()){
			out += DATA_SEP;
		}
		out += *it;
	}
	return out;
}

string history::get_moves(){
	string out = "";
	if(this->moves.size() == 0){
		return out;
	}
	
	for(auto it = this->moves.begin();it != this->moves.end();it++){
		if(it != this->moves.begin()){
			out += DATA_SEP;
		}
		out += *it;
	}
	return out;
}

string history::get_past(){
	if(this->past.size() == 0){
		return "";
	}
	return this->past.back();
}

vector<string> history::get_last_n_moves(int n){
	vector<string> out;
	if(this->turns < n){
		return out;
	}
	int i = 0;
	for(auto it = this->moves.rbegin();i<n; it++,i++){
		out->push_back(*it);
	}
	return out;
}

void history::add_white_taken(char p){
	this->white_taken.push_back(p);
}

void history::add_black_taken(char p){
	this->white_taken.push_back(p);
}

void history::add_move(string move){
	this->moves.push_back(move);
	this->turns++;
}

void history::add_past(string past){
	this->past.push_back(past);
}

void history::remove_last_white_taken(){
	if(this->white_taken.size()>0){
		this->white_taken.pop_back();
	}
}

void history::remove_last_black_taken(){
	if(this->black_taken.size()>0){
		this->black_taken.pop_back();
	}
}

void history::remove_last_move(){
	if(this->moves.size()>0){
		this->moves.pop_back();
		this->turns--;
	}
}

void history::remove_last_past(){
	if(this->past.size()>0){
		this->past.pop_back();
	}
}

bool history::has_white_taken(){
	return this->white_taken.size() > 0;
}

bool history::has_black_taken(){
	return this->black_taken.size() > 0;
}

bool history::has_moves(){
	return this->moves.size() > 0;
}

bool history::has_past(){
	return this->past.size() > 0;
}