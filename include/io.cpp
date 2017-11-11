#include "io.h"

char* IO::force_concat(char* s1,size_t l1,char*s2,size_t l2,uint8_t mem){
	size_t length = l1 + l2;
	char * out = (char *)calloc(sizeof(char),(length+1));
	register uint_fast64_t i;
	for(i = 0;i<l1;i++){
		out[i] = s1[i];
	}
	for(i = 0;i<l2;i++){
		out[i+l1] = s2[i];
	}
	if((mem >> 1)& 1){
		free(s2);
	}
	if((mem & 1)){
		free(s1);
	}
	out[length] = '\0';
	return out;
}

string IO::get_content_type(string directory){

	string command = "/usr/bin/file -i " + directory;
	FILE* fd = popen(command.c_str(),"r");
	char* file_type;

	if(!IO::read_file(fd,&file_type)){
		puts("Error getting file type.\nAborting");
		exit(EXIT_FAILURE);
	}
	string out(file_type);
	//free(file_type);
	out = out.substr(out.find(':') + 1);

	if(out.find(';')!=string::npos){
		out = out.substr(0,out.find(';'));
	}
	cout<<"Requested item is of type: "<<file_type<<endl;
	return out;
}

void IO::create_tmp_file(char* directory){
	cout<<"Creating the tmp file"<<endl;
	string src(WEB_ROOT);
	src.append(directory);
	string dest("./tmp");
	dest.append(directory);
	dest.append(".gz");
	string command = "gzip -c " + src + " > " + dest;
	system(command.c_str());
	cout<<"done"<<endl;
}


uint64_t IO::read_file(char* directory,char** data){
	char buffer[FILE_BUFFER_SIZE];
	char* dat = NULL;
	size_t n;
	uint64_t out = 0;
	string src(WEB_ROOT);
	src.append(directory);
	/*if(access(src,F_OK)==-1){
		create_tmp_file(directory);
	}*/
	FILE* fd =  fopen((char*)src.c_str(),"r");
	/*printf("reading the contents of the file...");*/
	while(!feof(fd)){
		memset(buffer,'\0',FILE_BUFFER_SIZE);
		n = fread(buffer,sizeof(char),FILE_BUFFER_SIZE - 1,fd);
		dat = concat(dat,buffer,(dat)? FIRST : FALSE);
		out += n;
	}
	*data = dat;
	printf("done\n");
	fclose(fd);
	return out;
}
uint64_t IO::prepare_media(char* directory,char** data){
	//char* src = concat("./tmp",concat(directory,".gz",FALSE),SECOND);
	/*if(access(src,F_OK)==-1){
		create_tmp_file(directory);
	}*/
	string src(WEB_ROOT);
	src.append(directory);
	FILE* fd =  fopen((char*)src.c_str(),"r");
	size_t n;
	char buffer[FILE_BUFFER_SIZE];
	char* dat = NULL;
	uint64_t out = 0;
	while(!feof(fd)){
		memset(buffer,'\0',FILE_BUFFER_SIZE);
		n = fread(buffer,sizeof(char),FILE_BUFFER_SIZE - 1,fd);
		dat = force_concat(dat,out,buffer,n,(dat != NULL)? FIRST : FALSE);
		out += n;	
	}
	*data = dat;
	fclose(fd);
	return out;
}

uint64_t IO::read_file(FILE* fd,char** data){
	char buffer[FILE_BUFFER_SIZE];
	char* dat = NULL;
	size_t n;
	uint64_t out = 0;
	while(!feof(fd)){
		memset(buffer,'\0',FILE_BUFFER_SIZE);
		n = fread(buffer,sizeof(char),FILE_BUFFER_SIZE - 1,fd);
		dat = concat(dat,buffer,(dat)? FIRST : FALSE);
		out += n;
		
	}
	*data = dat;
	fclose(fd);
	return out;
}

uint64_t IO::read_file(int fd,char** data){
	char buffer[FILE_BUFFER_SIZE];
	char* dat = NULL;
	ssize_t n;
	uint64_t out = 0;
	memset(buffer,'\0',FILE_BUFFER_SIZE);

	while((n = read(fd,buffer,FILE_BUFFER_SIZE - 1))){
		dat = concat(dat,buffer,(dat)? FIRST : FALSE);
		out += n;
		memset(buffer,'\0',FILE_BUFFER_SIZE);
	}
	*data = dat;
	close(fd);
	return out;
}

void IO::respond(int fd, char* data){
	//printf("HTTP MESSAGE\n%s\n\n\n",data);
	int chunk_size = 1000;
	int64_t size = strlen(data); 
	int chunk = 0;
	uintptr_t rd_ptr = reinterpret_cast<uintptr_t>(data);

	printf("Sending %ld bytes of data to the client...",size);
	while(size>0){
		if(size>=chunk_size){
			//buffer = substring(data,chunk*chunk_size,chunk_size);
			write(fd,(char*)rd_ptr,chunk_size);
		}else{
			//buffer = substring(data,chunk*chunk_size,size);
			write(fd,(char*)rd_ptr,size);
		}
		size -=  chunk_size;
		rd_ptr += chunk_size;
		chunk++;
	}
	puts("done");
}

void IO::respond(int fd, string data){
	IO::respond(fd,(char*)data.c_str());
}
void IO::force_print(char* in,size_t length){
	uint64_t i;
	for(i = 0;i<length;i++){
		printf("%c",in[i]);
	}
	printf("\n");
}

void IO::respond(int fd, Response r){
	if(r== NULL || r == nullptr){
		cerr<<"Response cannot be null"<<endl;
		return;
	}
	int chunk_size = 1000;
	int64_t size = r->data_size;
	int chunk = 0;
	char* buffer;
	
	cout<<"Sending the following header:\n"<<*r->header;
	IO::respond(fd,*r->header);
	if(r->body == nullptr){
		return;
	}

	printf("Sending %ld bytes of data to the client...",size);
	uintptr_t rd_ptr = reinterpret_cast<uintptr_t>(r->body);

	while(size>0){
		if(size>=chunk_size){
			write(fd,(char*)rd_ptr,chunk_size);
		}else{
			write(fd,(char*)rd_ptr,size);
		}
		size = size - chunk_size;
		rd_ptr += chunk_size;
		chunk++;
	}
	puts("done");
	if(r->body != NULL){
		free(r->body);
	}
	if(r->header != nullptr){
		delete r->header;
	}
	free(r);
	/*puts("DATA sent is ______________________________");
	force_print(r->body,r->data_size);
	puts("_____________________________");*/
}


void IO::append_to_file(string dir, string filename,string data){
	string loc = dir + filename;
	ofstream file;
	file.open(loc,ios::out|ios::app);
	file<<data;
	file.close();
}

void IO::write_to_file(string dir, string filename,string data){
	string loc = dir + filename;
	char* tmp = to_cstr(loc);
	unlink(tmp);
	free(tmp);
	ofstream file;
	file.open(loc,ios::out);
	file<<data;
	file.close();
}


string IO::read_from_file(string dir,string filename){
	string loc = dir + filename;
	ifstream file;
	file.open(loc);
	if(!file){
		throw exception();
	}
	string out = "";
	string tmp;
	while(!file.eof()){
		getline(file,tmp);
		out = out + tmp + "\n";
	}
	
	file.close();
	return out;
}


uint64_t IO::read_from_file(string dir,string filename, string* data){
	string loc = dir + filename;
	ifstream file;
	file.open(loc);
	if(!file){
		throw exception();
	}
	string out = "";
	string tmp;
	uint64_t size = 0;
	while(!file.eof()){
		getline(file,tmp);
		size +=  tmp.length();
		out = out + tmp + "\n";
	}
	
	file.close();
	*data = out;
	return size;
}

