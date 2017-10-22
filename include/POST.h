#ifndef _POST_H_
#define _POST_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <map>
#include <string>

#include "io.h"


using namespace std;
string process_through_PHP(map<string*,string*> _POST,string dir);
string* parse_PHP_args(map<string*,string*> args);


#endif