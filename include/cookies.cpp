#include "cookies.h"

using namespace std;

Cookie::Cookie(string value,time_t created,time_t exp){
	this->value = value;
	this->created = (created) ? created : time(NULL);
	this->expires = (exp) ? exp : time(NULL) + COOKIES_EXP;
}

bool Cookie::store(string ip, string key){
	string data = key + "\035" + this->value + "\035" + ltos((int64_t)this->created) + "\035" + ltos((int64_t)this->expires) + "\n";
	IO::append_to_file(COOKIES_DIR,ip,data);
	return true;
}

bool Cookie::store_cookies(string ip, map<string,Cookie*>* cookies){
	string loc = COOKIES_DIR + ip;
	bool out = true;
	for(map<string,Cookie*>::iterator it = cookies->begin();it!=cookies->end();++it){
		out = out && it->second->store(ip,it->first);
	}
	delete cookies;
	return out;
}

map<string,Cookie*>* Cookie::load_cookies(string ip){
	string loc = COOKIES_DIR + ip;
	string data;
	vector<string>* lines;
	
	map<string,Cookie*>* cookies = new map<string,Cookie*>();
	vector<string>* cookie_dat;
	time_t current_time = time(NULL);
	time_t expires;

	data = IO::read_from_file(loc);

	lines = explode("\n",data);

	for(int i = 0;i<lines->size();i++){
		cookie_dat = explode("\035",lines->at(i));
		expires = stoi(cookie_dat->at(3));
		if(expires<current_time){
			continue;
		}
		cookies->insert(pair<string,Cookie*>(cookie_dat->at(0),
			new Cookie(cookie_dat->at(1),stoi(cookie_dat->at(2)), expires)));
		delete cookie_dat;
	}
	delete lines;
	return cookies;

}

map<string,Cookie*>* Cookie::parse_cookies(string ip,HTTP_Header* r){

	try {
	   	r->at("Cookie");   
	}catch (const std::out_of_range& oor) {
	  	return nullptr;
	}

	vector<string>* key_value;
	map<string,Cookie*>* cookies = Cookie::load_cookies(ip);
	string ** cookie_dat = r->at("Cookie")->data;
	uint_fast64_t i = 0;
	string key;
	while(cookie_dat[i]){
		key_value = split('=',*cookie_dat[i]);
		key = key_value->at(0);
		try{
			cookies->at(key) = new Cookie(key_value->at(1));
		}catch(const out_of_range& oor){
			cookies->insert(pair<string,Cookie*>(key,new Cookie(key_value->at(1))));
		}
		i++;
	}

	return cookies;
}





