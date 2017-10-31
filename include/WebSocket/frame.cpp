#include "frame.h"

using namespace std;

uint8_t* Frame::c32_to_8(uint32_t in){
	uint8_t* out = (uint8_t*)calloc(sizeof(uint8_t),4);
	out[0] = (in>>24) & 0xFF;
	out[1] = (in>>16) & 0xFF; 
	out[2] = (in>>8)  & 0xFF;
	out[3] = in & 0xFF;
	return out;
}

Frame::Frame(): fin(1),rsv1(0), rsv2(0), rsv3(0), mask(1){
	this->mask_key = rand()<<1;
	this->mask_key += (this->mask_key%2 == 0)? 0 : 1;
	this->data = new vector<uint8_t>();
	this->length = 0;
	this->opcode = 0xF;
}

Frame::Frame(uint8_t* in){
	uint32_t h = (in[0]<<24) + (in[1]<<16) + (in[2]<<8) + in[3];
	uint32_t loc = 0;
	//cout<<"H is "<<h<<endl;
	this->fin = (h & 1<<31)>>31;
	this->rsv1 = (h & 1<<30)>>30;
	this->rsv2 = (h & 1<<29)>>29;
	this->rsv3 = (h & 1<<28)>>28;
	this->opcode = (h & 0xF<<24)>>24;
	this->mask = (h & 1<<23)>>23;
	this->length = (h & 0x7F<<16)>>16;
	loc = 3;
	if(this->length == 126){
		this->length = (in[2]<<8) + in[3];
	}else if(this->length == 127){
		this->length = (in[4]<<24) + (in[5]<<16) + (in[6]<<8) + in[7];
		loc += 4;
	}
	this->data = new vector<uint8_t>();
	//loc+=2;
	loc--;
	this->mask_key = (in[loc++]<<24) + (in[loc++]<<16) + (in[loc++]<<8) + in[loc++];
	uint8_t* mask = Frame::c32_to_8(this->mask_key);
	for(uint32_t i = 0;i<this->length;i++){
		this->data->push_back(in[i+loc] ^ mask[i%4]);
	}
}

Frame::~Frame(){
	delete this->data;
}

void Frame::add(uint8_t* data){
	for(uint32_t i = 0;data[i]!='\0';i++){
		this->data->push_back(data[i]);
	}
	this->length = this->data->size();
}


void Frame::debug(){
	cout<<"Fin: "<<(int)this->fin<<endl;
	cout<<"RSV1: "<<(int)this->rsv1<<endl;
	cout<<"RSV2: "<<(int)this->rsv2<<endl;
	cout<<"RSV3: "<<(int)this->rsv3<<endl;
	cout<<"opcode: "<<(int)this->opcode<<endl;
	cout<<"mask: "<<(int)this->mask<<endl;
	cout<<"mask_key: "<<this->mask_key<<endl;
	cout<<"Length: "<<this->length<<endl;

}

char* Frame::get_cstr(){
	char* out = (char*)malloc(sizeof(char)*(this->length + 1));
	out[this->length] = '\0';
	for(uint32_t i = 0;i<this->length;i++){
		out[i] = this->data->at(i);
	}
	return out;
}

uint8_t* Frame::encode(uint32_t* out_size){
	uint8_t* out;
	uint64_t size = 0;
	uint32_t h = 0;
	this->length = this->data->size();

	h = h | (this->fin << static_cast<int>(BitLoc::FIN));
	h = h | (this->rsv1 << static_cast<int>(BitLoc::RSV1));
	h = h | (this->rsv2 << static_cast<int>(BitLoc::RSV2));
	h = h | (this->rsv3 << static_cast<int>(BitLoc::RSV3));
	h = h | (this->opcode << static_cast<int>(BitLoc::OPCODE));
	h = h | (this->mask << static_cast<int>(BitLoc::MASK));
	if(this->length>125){
		if(this->length>0xFFFF){
			h = h | (0x7F << static_cast<int>(BitLoc::LENGTH));
		}else{
			h = h | (0x7E << static_cast<int>(BitLoc::LENGTH));
			h = h | (this->length + 2);
		}
	}else{
		h = h | ((this->length + 4)<< static_cast<int>(BitLoc::LENGTH));
	}
	out = Frame::c32_to_8(h);

	size = 4;
	if((h >> static_cast<int>(BitLoc::LENGTH) & 0x7F) == 0x7F){
		out = (uint8_t*)realloc((void*)out,8);
		out[4] = this->length >> 24;
		out[5] = (this->length >> 16) & 0xFF; 
		out[6] = (this->length >> 8)  & 0xFF;
		out[7] = this->length & 0xFF;
		size = 8;
	}
	out = (uint8_t*)realloc((void*)out, size + 6 + this->length + 1);
	out[size + 6 + this->length] = '\0';
	out[size++] = 0;
	out[size++] = 0;
	//uint8_t* tmp = c32_to_8(h);
	cout<<"\n\nHEADER:"<<convertToBinary((char*)out,2)<<endl<<endl;
	//free(tmp);
	if(this->mask == 1){
		
		uint8_t* mask = c32_to_8(this->mask);
		out[size++] = mask[0];
		out[size++] = mask[1];
		out[size++] = mask[2];
		out[size++] = mask[3];
		free(mask);
	}
	
	for(uint32_t i = 0;i<this->length;i++){
		out[size++] = this->data->at(i);
	}
	if(out_size != nullptr){
		*out_size = size;
	}
	return out;
}


void Frame::send(int sd){
	uint32_t size;
	uint8_t* data = this->encode(&size);
	write(sd,(void*)data,size);
}

void Frame::merge(Frame* frame){
	for(uint32_t i = 0;i<frame->data->size();i++){
		this->data->push_back(frame->data->at(i));
	}
	this->length = this->data->size();
}
