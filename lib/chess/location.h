#ifndef _LOCATION_H_
#define _LOCATION_H_

#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

class Location{
	public:
		int col;
		int row;
	
		Location(int row, int col);
		vector<Location*>* getAdjacent();
		bool equals(Location* l);
		bool isAdjacent(Location* l);
		static vector<Location*>* locationsBetween(Location* l1,Location* l2);
		static vector<Location*>* locationsBetween(int row1, int col1,int row2,int col2);
};


#endif