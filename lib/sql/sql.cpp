#include "sql.h"


using namespace std;

string SQLConn::location = "tcp://127.0.0.1:3306";
string SQLConn::user = "root";
string SQLConn::password = "password";
sql::Driver* SQLConn::driver = sql::mysql::get_driver_instance();

SQLConn::SQLConn(){
	this->conn = SQLConn::driver->connect(SQLConn::location,SQLConn::user,SQLConn::password);
}

SQLConn::SQLConn(string database){
	this->conn = SQLConn::driver->connect(SQLConn::location,SQLConn::user,SQLConn::password);
	this->update_database(database);
}

SQLConn::~SQLConn(){
	delete this->conn;
}

void SQLConn::update_database(string database){
	if(database.size()>0){
		this->database = database;
	}
	this->conn->setSchema(this->database);
}

void SQLConn::execute(string types,string query,...){
	va_list valist;
	int args = types.size();
	sql::PreparedStatement* prep_stmt = this->conn->prepareStatement(query);

	va_start(valist, args);
	for(int i = 1;i<=args;i++){
		switch((char)types.at(i-1)){
			case 's':
			{
				string arg = (string)va_arg(valist, string);
				prep_stmt->setString(i,arg);
				break;
			}
			case 'i':
			{
				int arg = (int)va_arg(valist,int);
				prep_stmt->setInt(i,arg);
				break;
			}
			default:
			{
				cerr<<"Unknown type given to execute"<<endl;
			}
		}
	}
	va_end(valist);
	prep_stmt->execute();
	delete prep_stmt;
	return;
}

void SQLConn::execute(string query){
	sql::Statement* stmt;
	stmt = this->conn->createStatement();
	stmt->execute(query);
	delete stmt;
}

sql::ResultSet* SQLConn::fetch(string types,string query,...){
	va_list valist;
	int args = types.size();
	sql::PreparedStatement* prep_stmt = this->conn->prepareStatement(query);

	va_start(valist, args);
	for(int i = 1;i<=args;i++){
		switch((char)types.at(i-1)){
			case 's':
			{
				string arg = (string)va_arg(valist, string);
				prep_stmt->setString(i,arg);
				break;
			}
			case 'i':
			{
				int arg = (int)va_arg(valist,int);
				prep_stmt->setInt(i,arg);
				break;
			}
			default:
			{
				cerr<<"Unknown type given to execute"<<endl;
			}
		}
	}
	va_end(valist);
	sql::ResultSet* res = prep_stmt->executeQuery();
	delete prep_stmt;
	return res;
}

sql::ResultSet*  SQLConn::fetch(string query){
	sql::Statement* stmt;
	sql::ResultSet* res;
	stmt = this->conn->createStatement();
	res = stmt->executeQuery(query);
	delete stmt;
	return res;
}