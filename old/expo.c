#include "./include/server.h"
int expo(int n,int i){
	int inc = 1;
	int out = n;
	while(inc<i){
		out = out *n;
		inc++;
	}
	return out;
}
