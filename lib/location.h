#ifndef _LOCATION_H_
#define _LOCATION_H_

#include <vector>
#include <cstdlib>

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
};


#endif