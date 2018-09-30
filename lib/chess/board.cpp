#include "chess/board.h"

using namespace std;

board::board(const string& FEN, const string& spec, const string& castle){
	this->tiles = (tile**)calloc(sizeof(tile*),8);
	string f = FEN;
	char c;
	//cout<<"Processing the Castle data...";
	this->bCastle = ((castle.find("bk")==string::npos)? 0 : KING_SIDE) | ((castle.find("bq")==string::npos)? 0 : QUEEN_SIDE);
	this->wCastle = ((castle.find("wk")==string::npos)? 0 : KING_SIDE) | ((castle.find("wq")==string::npos)? 0 : QUEEN_SIDE);
	//cout<<"done, WHITE - "<<(int)this->wCastle<<" : BLACK - "<<(int)this->bCastle<<endl;
	this->taken = 'X';
	//cout<<"Creating the tiles\n";
	for(int i = 0;i < 8;i++){
		this->tiles[i] = (tile*)calloc(sizeof(tile),8);
		for(int j = 0;j<8;j++){
			c = f.back();
			f.pop_back();
			if(c == '/'){
				c = f.back();
				f.pop_back();
			}
			this->tiles[i][j] = tile(c);
		}	
	}
	//cout<<"Created tiles\n";
	if(spec.size()>1){
		this->special_data(spec);
	}
	this->special = spec;
	//cout<<"Processed the special data\n";
	cout<<"Given: "<<FEN<<endl;
	cout<<"Produced: "<<this->to_string()<<endl;
	
}

board::~board(){
	for(int i = 0;i < 8;i++){
		free(this->tiles[i]);
	}
	free(this->tiles);
}

tile& board::get_tile(int row, int col){
	if((uint32_t)row > 8 || (uint32_t)col > 8 || col == 0 || row == 0){
		throw out_of_range("board::getTile:Error:Tried to get a tile which is out of bounds");
	}
	return this->tiles[row-1][col-1];
}

bool board::empty(int row,int col){
	if(row == 0 || col == 0){
		throw out_of_range("board::getTile:Error:Tried to get a tile which is out of bounds");
	}
	tile t = this->get_tile(row,col);
	return t->empty();
}

void board::special_data(const string& data){
	cout<<"Getting the data from the string\n";
	int r = data[0] - 48;
	int c = data[1] - 48;
	cout<<"POS is ("<<r<<","<<c<<") data is "<<data<<endl;
	char side = data[2];
	cout<<"Checking to see if tile is valid\n";
	if(!location::validate(r,c)){
		return;
	}
	cout<<"The tile is not empty and not NULL\n";
	if(! this->get_tile(r,c).p.is(PAWN)){
		return;
	}
	cout<<"Setting the special bit\n";
	if(side =='r'){
		this->get_tile(r,c).p.set_special(RIGHT);
	}else{
		this->get_tile(r,c).p.set_special(LEFT);
	}
}

void board::force_change(int r,int c,char FEN){ 
	this->tiles[r-1][c-1].change(FEN);
}

void board::force_move(int r,int c,int r2, int c2){
	this->tiles[r2-1][c2-1].p.change(this->tiles[r-1][c-1].p.FEN);
	this->tiles[r-1][c-1].p.clear();
}

bool board::placePiece(int r,int c,char FEN){
	if(!this->get_tile(r,c).empty() || piece::get_name(FEN) == 0){
		return false;
	}
	this->force_change(r,c,FEN);
	return true;
}

location board::findKing(char side){
	tile t;
	for(int i = 1;i < 9;i++)
		for(int j = 1;j < 9;j++){
			t = this->get_tile(i,j);
			if(!t.empty() && t.p.is(KING) && t.p.side == side){
				return location(i,j);
			}	
	}
	throw "Unable to the find the king";
}

piece& board::get_king(char side){
	location loc = this->find_king(side);
	
	return this->get_tile(loc.row,loc.col).p;
}

char board::other_side(char side){
	return (side == WHITE)? BLACK : WHITE;
}

string board::generate_FEN(){
	string output;
	output.reserve(73);
	tile t;
	for(int i = 8;i>0;i--){ 
		for(int j = 8;j>0;j--){
			t = this->get_tile(i,j);
			if(t.empty()){
				output += EMPTY_SPACE;
			}else{
				output += t.p.FEN;			
			}
		}	
		if(i>1)
			output = output + "/";
	}
	return output;	
}

string board::get_castle_data(){
	string out = (this->black_castle & KING_SIDE) ? "bk" : "";
	out += (this->black_castle & QUEEN_SIDE)? "bq" : "";
	out += (this->white_castle & KING_SIDE)? "wk" : "";
	out += (this->white_castle & QUEEN_SIDE)? "wq" : "";
	return out;
}

string board::to_string(){
	string out = this->generate_FEN();
	out += ";";
	out += this->get_castle_data();
	out += ";";
	out += this->special;
	return out;
}

char board::num_to_col(int c){
	return (char)(c + 96);
}