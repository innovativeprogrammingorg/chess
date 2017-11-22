#ifndef _LOCATION_H_
#define _LOCATION_H_

#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <memory>

using namespace std;

class Location{
	public:
		int col;
		int row;
	
		Location(int row, int col);
		vector<unique_ptr<Location>>* getAdjacent();
		bool equals(Location* l);
		bool isAdjacent(Location* l);
		static vector<unique_ptr<Location>>* locationsBetween(Location* l1,Location* l2);
		static vector<unique_ptr<Location>>* locationsBetween(int row1, int col1,int row2,int col2);
		static void debug(int row,int col);
		static bool validate(int row,int col);
		void debug();

};


#endif