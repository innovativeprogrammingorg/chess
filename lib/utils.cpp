#include "utils.h"

int * intmemcopy(int * ptr){
	int* out = (int *)malloc(sizeof(int));
	*out = *ptr;
	return out;
}

