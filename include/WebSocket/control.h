#ifndef _CONTROL_H
#define _CONTROL_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "frame.h"
#include "../client.h"
#include "../../lib/manager.h"

using namespace std;

class Frame_Buffer{
	public:
		vector<Frame*>* data;

	Frame_Buffer(): data(new vector<Frame*>()){}  
	virtual ~Frame_Buffer();
};

class Control{
	public:
		/**
		 * Buffer for handling fragmented messages
		 */
		map<Client*, Frame_Buffer*>* data;
		static Control* control;
		static map<string,int>* lookup_table;

		Control();
		static void init_tables();
		static void flush_frames(Client* c);
		static Frame_Buffer* get_frames(Client* c);
		static void push_frame(Client* c,Frame* frame);

		static int get_action(Frame* frame,string* data = nullptr);
		static void handle_request(Client* c, char* raw_data);
};






/**
 * END GAME RELATED CONSTANTS, BEGIN WEB SOCKET ACTIONS
 */




#endif