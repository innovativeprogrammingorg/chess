#ifndef _LOCATION_H_
#define _LOCATION_H_

#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <memory>

using namespace std;

class location{
	public:
		int col;
		int row;
	
		location(int row, int col);
		vector<location> get_adjacent();
		bool operator==(const location& l);
		bool equals(const location& l);
		bool is_adjacent(const location* l);
		static vector<location> locations_between(const location& l1,const location& l2);
		static vector<location> locations_between(int row1, int col1,int row2,int col2);
		static void debug(int row,int col);
		static bool validate(int row,int col);
		void debug();

};


#endif