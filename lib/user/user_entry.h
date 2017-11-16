#ifndef _USER_ENTRY_H_
#define _USER_ENTRY_H_

#include <iostream>
#include <string>
#include <map>
#include <cstdint>

using namespace std;

class User_Entry{
	public:
		map<int64_t,int>* i_map;
		map<string,int>* s_map;

		User_Entry();
		virtual ~User_Entry();
		void add(string key,int sd);
		void add(int64_t key,int sd);
		int find(string key);
		int find(int64_t key);
		void remove_sd(int sd);
		void remove(string key);
		void remove(int64_t key);
		bool empty();

};

#endif