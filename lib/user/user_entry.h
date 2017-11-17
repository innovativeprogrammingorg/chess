#ifndef _USER_ENTRY_H_
#define _USER_ENTRY_H_

#include <iostream>
#include <string>
#include <map>
#include <cstdint>

using namespace std;

struct UEkey{
	string* s_key;
	int64_t i_key;
};

typedef struct UEkey* UEKey;


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
		
		void add(UEKey key,int sd);
		int find(UEKey key);
		
		void remove(UEKey key);
		static UEKey new_key(string key);
		static UEKey new_key(int64_t key);

};

#endif