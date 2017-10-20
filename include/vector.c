#include "vector.h"

static Vector createVector(){
	Vector out = (Vector) malloc(sizeof(struct vect));
	out->next = NULL;
	out->prev = NULL;
	out->data = NULL;
	return out;
}

size_t vector_length(Vector v){
	size_t length = 0;
	Vector tmp = v;
	if(v==NULL){
		return 0;
	}
	while(tmp != NULL){
		tmp = tmp->next;
		length++;
	}
	return length;
}

void* vector_get(Vector v,size_t index){
	Vector tmp = v;
	int i = 0;
	while(i !=  index && tmp != NULL){
		tmp = tmp->next;
		i++;
	}
	return tmp->data;
}

void* vector_get_f(Vector v,size_t index){
	char * out = vector_get(v,index);
	vector_clean(v);
	return out;
}

void vector_pop(Vector* v, size_t index){
	if(v==NULL || *v == NULL){
		return;
	}
	Vector tmp = *v;
	Vector p;
	int i = 0;
	if(index == 0){
		*v = tmp->next;
		free(tmp);
		if(*v != NULL){
			(*v)->prev = NULL;
		}
		return;
	}
	while(i !=  index && tmp != NULL){
		tmp = tmp->next;
		i++;
	}
	p = tmp->prev;
	p->next = tmp->next;
	tmp->next->prev = p;
	free(tmp);
}



void vector_push(Vector * v,void* data){
	Vector add = createVector();
	Vector tmp;
	add->data = data;
	if(v==NULL||*v==NULL){
		*v = add;
		return;
	}

	tmp = *v;
	while(tmp->next != NULL){
		tmp = tmp->next;
	}
	tmp->next = add;
	add->prev = tmp;
}

void vector_clean(Vector v){
	Vector tmp = v;
	Vector clear = NULL;
	if(v==NULL){
		return;
	}
	
	while(tmp != NULL){
		
		clear = tmp;
		tmp = tmp->next;
		clear->next = NULL;
		clear->prev = NULL;
		if(clear->data)
			free(clear->data);
		free(clear);
	}
}

void vector_free(Vector v){
	Vector tmp = v;
	Vector clear = NULL;
	if(v==NULL){
		return;
	}
	
	while(tmp->next != NULL){
		tmp = tmp->next;
		clear = tmp->prev;
		clear->next = NULL;
		clear->prev = NULL;
		clear->data = NULL;
		free(clear);
	}
	free(tmp);
}

Vector vector_merge(Vector * v1, Vector * v2){
	int i;
	for(i = 0;i<vector_length(*v2);i++){
		vector_push(v1,vector_get(*v2,i));
	}
	if(*v2!=NULL){
		free(*v2);
	}
	return *v1;
}

void vector_print_i(Vector v1){
	Vector v = v1;
	puts("Printing vector contents as ints");
	while(v!=NULL){
		printf("%d\n",*((int *)v->data));
		v = v->next;
	}
	puts("done");
}

void ** vector_to_array(Vector v){
	size_t size = vector_length(v);
	void ** out = (void**)calloc(sizeof(void*),size+1);
	out[size] = NULL;
	int32_t i = 0;
	for(i = 0;i<size;i++){
		void* value = vector_get(v,i);
		out[i] = value;
	}
	return out;
}