#include "chess/location.h"

using namespace std;

location::location(int row, int col) : row(row),col(col){}

vector<location> location::locations_between(const location& l1,const location& l2)
{
	int i,r,c;
	int col1 = l1.col;
	int col2 = l2.col;
	int row1 = l1.row;
	int row2 = l2.row; 
	
	vector<location> out;
	if(l1.equals(l2) || l1.isAdjacent(l2)){	
		return out;
	}
	
	if(row1==row2){
		if(col1>col2){
			for(i = col2 + 1;i<col1;i++){
				out->push_back(location(row1,i));
			}
			
		}else{
			for(i = col1 + 1;i<col2;i++){
				out->push_back(location(row1,i));
			}
		}
		
	}else if(col1==col2){ 
		if(row1>row2){
			for(i = row2 + 1;i<row1;i++){
				out->push_back(location(i,col1));
			}
		}else{
			for(i=row1+1;i<row2;i++){
				out->push_back(location(i,col1));
			}
		}		
	}else if((row1-row2)/(col1-col2) == -1){
		if(row1>row2){
			r = row1 - 1;
			c = col1 + 1;
			while(r > row2){
				out->push_back(location(r,c));
				r--;
				c++;
			}
		}else{
			r = row1 + 1;
			c = col1 - 1;
			while(r < row2){
				out->push_back(location(r,c));
				r++;
				c--;
			}
		}	
	}else if((row1-row2)/(col1-col2) == 1){
		if(row1>row2){
			r = row2 + 1;
			c = col2 + 1;
			while(r<row1){
				out->push_back(location(r,c));
				r++;
				c++;
			}
		}else{
			r = row2 - 1;
			c = col2 - 1;
			while(r>row1){
				out->push_back(location(r,c));
				r--;
				c--;
			}
		}	
	}else{
		cerr<<"location::locations_between Error: Cannot find any locations between (";
		cerr<<row1<<","<<col1<<") and ("<<row2<<","<<col2<<")"<<endl;
	}
	return out;
}

vector<location> location::locations_between(int row1, int col1,int row2,int col2)
{
	location l1(row1,col1);
	location l2(row2,col2);
	return location::locationsBetween(l1,l2);
}

bool location::is_adjacent(const location& l){
	return !(this->equals(l)||abs(this->row - l->row) > 1 || abs(this->col - l->col) > 1);
}

bool location::equals(const location& l){
	return this->col == l->col && this->row == l->row;
}

vector<location> location::get_adjacent(){
	int r = this->row;
	int c = this->col;
	vector<location> out;
	if(c<8){
		out->push_back(location(r,c+1));
		if(r>1){
			out->push_back(location(r-1,c+1));
		}
	}
	if(r<8){
		out->push_back(location(r+1,c));
		if(c<8){
			out->push_back(location(r+1,c+1));
		}
	}
	if(c>1){
		out->push_back(location(r,c-1));
		if(r<8){
			out->push_back(location(r+1,c-1));
		}
	}
	if(r>1){
		out->push_back(location(r-1,c));
		if(c>1){
			out->push_back(location(r-1,c-1));
		}
	}

	return out;
}

bool location::validate(int row,int col){
	return !(row>9 || row<1 || col>9||col<1);
}

void location::debug(int row,int col){
	cout<<"location is ("<<row<<","<<col<<")"<<endl;
}

void location::debug(){
	location::debug(this->row,this->col);
}