#include "chess/timer.h"

using namespace std;

Timer::Timer(int white_time,int black_time,int increment,uint8_t turn){
	this->last = 0;
	this->undo = 0;
	this->white_time = white_time;
	this->black_time = black_time;
	this->increment = increment;
	this->turn = turn;
}

Timer::Timer(int white_time,int black_time,int increment,uint8_t turn,uint64_t last,int undo){
	this->white_time = white_time;
	this->black_time = black_time;
	this->increment = increment;
	this->turn = turn;
	this->last = last;
	this->undo = undo;
}

void Timer::next(){
	uint8_t t = turn;
	if(this->turn == WHITE){
		this->turn = BLACK;
		this->undo = black_time;
	}else{
		this->turn = WHITE;
		this->undo = white_time;
	}
	if(this->last == 0){
		this->last = time(NULL);
		return;
	}
	if(t == WHITE){
		this->white_time -= time(NULL) - this->last;
		this->white_time += this->increment;
	}else{
		this->black_time -= time(NULL) - this->last;
		this->black_time += this->increment;
	}
	this->last = time(NULL);

}

void Timer::update(){
	if(this->last == 0){
		return;
	}
	if(this->turn == WHITE){
		this->white_time -= time(NULL) - this->last;
		this->white_time += this->increment;
	}else{
		this->black_time -= time(NULL) - this->last;
		this->black_time += this->increment;
	}
	this->last = time(NULL);

}

void Timer::stop(){
	this->last = 0;
}

void Timer::back(){
	if(this->undo == 0){
		return;
	}
	if(this->turn == WHITE){
		this->white_time = this->undo;
		this->turn = BLACK;
		this->last = time(NULL);
		this->black_time -= increment;
	}else{
		this->black_time = this->undo;
		this->turn = WHITE;
		this->last = time(NULL);
		this->white_time -= increment;
	}
}

int Timer::get_white_time(){
	return this->white_time;
}

int Timer::get_black_time(){
	return this->black_time;
}

int Timer::get_increment(){
	return this->increment;
}

uint64_t Timer::get_last(){
	return this->last;
}

int Timer::get_undo(){
	return this->undo;
}

string Timer::format_time(time_t seconds){
	int sec = seconds % 60;
	int min = (int)(seconds / 60);
	string out = itoa(min);
	out += ":";
	if(sec<10){
		out += "0";	
	}
	out += itoa(sec);
	return out;
}