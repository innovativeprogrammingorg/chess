#include "chess/location.h"

using namespace std;

Location::Location(int row, int col){
	this->row = row;
	this->col = col;
}

vector<unique_ptr<Location>>* Location::locationsBetween(Location* l1,Location* l2){
	int i,r,c;
	int col1 = l1->col;
	int col2 = l2->col;
	int row1 = l1->row;
	int row2 = l2->row; 
	    
	if(l1->equals(l2)||l1->isAdjacent(l2)){	
		return nullptr;
	}
	vector<unique_ptr<Location>>* out = new vector<unique_ptr<Location>>();
	if(row1==row2){
		if(col1>col2){
			for(i = col2 + 1;i<col1;i++){
				out->push_back(make_unique<Location>(row1,i));
			}
			
		}else{
			for(i = col1 + 1;i<col2;i++){
				out->push_back(make_unique<Location>(row1,i));
			}
		}
		
	}else if(col1==col2){ 
		if(row1>row2){
			for(i = row2 + 1;i<row1;i++){
				out->push_back(make_unique<Location>(i,col1));
			}
		}else{
			for(i=row1+1;i<row2;i++){
				out->push_back(make_unique<Location>(i,col1));
			}
		}		
	}else if((row1-row2)/(col1-col2) == -1){
		if(row1>row2){
			r = row1 - 1;
			c = col1 + 1;
			while(r > row2){
				out->push_back(make_unique<Location>(r,c));
				r--;
				c++;
			}
		}else{
			r = row1 + 1;
			c = col1 - 1;
			while(r<row2){
				out->push_back(make_unique<Location>(r,c));
				r++;
				c--;
			}
		}	
	}else if((row1-row2)/(col1-col2) == 1){
		if(row1>row2){
			r = row2 + 1;
			c = col2 + 1;
			while(r<row1){
				out->push_back(make_unique<Location>(r,c));
				r++;
				c++;
			}
		}else{
			r = row2 - 1;
			c = col2 - 1;
			while(r>row1){
				out->push_back(make_unique<Location>(r,c));
				r--;
				c--;
			}
		}	
	}else{
		cerr<<"Location::locationsBetween:Error: Cannot find any locations between (";
		cerr<<row1<<","<<col1<<") and ("<<row2<<","<<col2<<")"<<endl;
		delete out;
		return nullptr;
	}
	return out;
}

vector<unique_ptr<Location>>* Location::locationsBetween(int row1, int col1,int row2,int col2){
	Location* l1 = new Location(row1,col1);
	Location* l2 = new Location(row2,col2);
	vector<unique_ptr<Location>>* out = Location::locationsBetween(l1,l2);
	delete l1;
	delete l2;
	return out;
}

bool Location::isAdjacent(Location* l){
	return !(this->equals(l)||abs(this->row - l->row) > 1 || abs(this->col - l->col) > 1);
}

bool Location::equals(Location* l){
	return this->col == l->col && this->row == l->row;
}

vector<unique_ptr<Location>>* Location::getAdjacent(){
	int r = this->row;
	int c = this->col;
	vector<unique_ptr<Location>>* out = new vector<unique_ptr<Location>>();
	if(c<8){
		out->push_back(make_unique<Location>(r,c+1));
		if(r>1){
			out->push_back(make_unique<Location>(r-1,c+1));
		}
	}
	if(r<8){
		out->push_back(make_unique<Location>(r+1,c));
		if(c<8){
			out->push_back(make_unique<Location>(r+1,c+1));
		}
	}
	if(c>1){
		out->push_back(make_unique<Location>(r,c-1));
		if(r<8){
			out->push_back(make_unique<Location>(r+1,c-1));
		}
	}
	if(r>1){
		out->push_back(make_unique<Location>(r-1,c));
		if(c>1){
			out->push_back(make_unique<Location>(r-1,c-1));
		}
	}

	return out;
}