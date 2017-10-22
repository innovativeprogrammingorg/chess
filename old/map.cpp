#include "map.h"

static Map createMap(){
	Map out = (Map)malloc(sizeof(struct map));
	out->head = NULL;
	return out;
}
static Meta createMeta(){
	Meta out = (Meta)malloc(sizeof(struct meta));
	out->key = NULL;
	out->value = NULL;
	out->next = NULL;
	out->prev = NULL;
	out->type = STRING_TYPE;
	return out;
}

uint8_t map_has_key(Map m, char* key){
	Meta tmp = m->head;
	while(tmp != NULL){
		if(strcompare(key,tmp->key)){
			return TRUE;
		}
		tmp = tmp->next;
	}
	return FALSE;
}

void map_add(Map* m, char* key, void* value,uint8_t type){
	Meta add = createMeta();
	Meta tmp;
	add->value = value;
	add->key = (char*)calloc(sizeof(char),strlen(key)+1);
	add->type = type;
	memcpy(add->key,key,strlen(key)+1);
	if(m==NULL||*m==NULL){
		*m = createMap();
		(*m)->head = add;
		return;
	}
	tmp = (*m)->head;
	while(tmp->next != NULL){
		tmp = tmp->next;
	}
	tmp->next = add;
	add->prev = tmp;
}

void map_clean(Map m){
	Meta tmp;
	Meta clear = NULL;
	uint64_t i;
	if(m==NULL){
		return;
	}
	tmp = m->head;
	while(tmp != NULL){
		
		clear = tmp;
		tmp = tmp->next;

		clear->next = NULL;
		clear->prev = NULL;
		if(clear->value){
			switch(clear->type){
				case MAP_TYPE:
					map_clean(clear->value);
					break;
				case ARRAY_TYPE:
					 i = 0;
					while( (((void**)clear->value)[i]) ){
						free( (((void**)clear->value)[i]) );
						i++;
					}
					free(clear->value);
					break;
				default:
					free(clear->value);
			}
		}
		
		
		free(clear->key);
		clear->value = NULL;
		clear->key = NULL;
		free(clear);
		
	}
	free(tmp);
	free(m);
}

void map_remove(Map* m, char* key){
	if(m==NULL || *m == NULL){
		return;
	}
	Meta tmp = (*m)->head;
	Meta p;
	int i = 0;
	while(!strcompare(tmp->key,key) && tmp != NULL){
		tmp = tmp->next;
		i++;
	}
	if(tmp==NULL){
		puts("Error, key not found");
		return;
	}
	p = tmp->prev;
	p->next = tmp->next;
	tmp->next->prev = p;
	free(tmp);
}

void* map_value_at(Map m, char* key){
	Meta tmp;
	if(m == NULL){
		puts("Error map is null");
		return NULL;
	}
	tmp = m->head;
	while(tmp != NULL && !strcompare(key,tmp->key)){
		tmp = tmp->next;
	}
	if(tmp == NULL){
		puts("Error, key not found!");
		print_map_contents(m);
		return NULL;
	}
	return tmp->value;
}

void map_update(Map *m, char* key, void* value, uint8_t type){
	Meta tmp;
	if(m == NULL){
		puts("Error map is null");
		return;
	}
	tmp = (*m)->head;
	while(tmp != NULL && !strcompare(key,tmp->key)){
		tmp = tmp->next;
	}
	if(tmp == NULL){
		/*puts("Error, key not found!");
		print_map_contents(m);*/
		return;
	}
	tmp->value = value;
	tmp->type = type;
}

size_t map_size(Map m){
	Meta tmp = m->head;
	size_t size = 0;
	while(tmp != NULL){
		size++;
		tmp = tmp->next;
	}
	return size;
}

char** map_get_keys(Map m){
	size_t size = map_size(m);
	char** keys = (char **)calloc(sizeof(char*),size+1);
	keys[size] = NULL;
	int i = 0;
	char* key;
	Meta tmp = m->head;
	for(i = 0;i<size;i++){
		key = tmp->key;
		keys[i] = (char*)calloc(sizeof(char*),strlen(key)+1);
		strcpy(keys[i],key);
		tmp = tmp->next;
	}
	return keys;
}

void print_map_contents(Map m){
	size_t size = map_size(m);
	int i = 0;
	Meta tmp;
	int j = 0;
	if(m==NULL){
		puts("Map is NULL");
		return;
	}
	tmp = m->head;
	for(i = 0;i<size;i++){
		printf("%s=>",tmp->key);
		switch(tmp->type){
			case ARRAY_TYPE:
				j = 0;
				printf("[");
				while((char*)((char**)tmp->value)[j]){
					if(j != 0){
						printf(",");
					}
					printf("\n%d=>%s",j,(char*)((char**)tmp->value)[j]);		
					j++;
				}
				puts("]");
				break;
			case MAP_TYPE:
				print_map_contents(tmp->value);
				break;
			case STRING_TYPE:
				printf("%s\n",(char*)tmp->value);

		}
		printf("\n");
		tmp = tmp->next;
	}
}