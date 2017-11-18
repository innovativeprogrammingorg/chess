#ifndef _SQL_H_
#define _SQL_H_


#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdarg.h>
#include <cstdint>
#include "mysql_driver.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

/**
 * double 'd'
 * int32_t 'i'
 * string 's'
 * int64_t 'l'
 * uint64_t 'u'
 */

using namespace std;

class SQLConn{
	private:
		static sql::Driver* driver;
	  	static string user;
	  	static string password;
	  	static string location;
	public:
		string database;
		sql::Connection* conn;
	  	//sql::Statement* stmt;
	  	//sql::ResultSet* res;
	  	//sql::PreparedStatement* prep_stmt;
		SQLConn();
		SQLConn(string database);
		virtual ~SQLConn();
		void update_database(string database = "");
		void execute(string types,string query,...);
		void execute(string query);
		sql::ResultSet* fetch(string types,string query,...);
		sql::ResultSet* fetch(string query);


};


#endif