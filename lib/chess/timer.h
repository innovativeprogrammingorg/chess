#ifndef _CHESS_TIMER_H_
#define _CHESS_TIMER_H_

#include <time.h>
#include <sys/time.h>
#include <string>
#include <cstdint>
#include <thread>
#include "chess/chess_types.h"
#include "str.h"

using namespace std;

class Timer{
	private:
		uint64_t last;
		int undo;
		int white_time;
		int black_time;
		int increment;
		uint8_t turn;
	public:
		Timer(int white_time,int black_time,int increment,uint8_t turn);
		Timer(int white_time,int black_time,int increment,uint8_t turn,uint64_t last,int undo);

		void update();
		void next();
		void stop();
		void back();

		int get_white_time();
		int get_black_time();
		int get_increment();
		uint64_t get_last();
		int get_undo();
		uint8_t get_turn();
		static string format_time(time_t seconds);
};

#endif