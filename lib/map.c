#include "map.h"

Map createMap(){
	Map out = (Map)malloc(sizeof(struct map));
	out->key = -1;
	out->data = -1;
	out->next = NULL;
	out->prev = NULL;
	return out;
}

void map_update(Map* v, int64_t key,int64_t data){
	Map tmp;
	if(v==NULL||*v==NULL){
		Map add = createMap();
		add->key = key;
		add->data = data;
		*v = add;
		return;
	}
	tmp = *v;
	while(tmp->next != NULL){
		if(tmp->key == key){
			tmp->data = data;
			return;
		}
		tmp = tmp->next;
	}
	Map add = createMap();
	add->key = key;
	add->data = data;
	tmp->next = add;
	add->prev = tmp;
}
int64_t map_lookup(Map m,int64_t key){
	Map tmp = m;
	while(tmp!= NULL){
		if(tmp->key == key){
			return tmp->data;
		}
		tmp = tmp->next;
	}
	return -1;
}
