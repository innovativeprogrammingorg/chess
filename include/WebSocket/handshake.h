#ifndef _HANDSHAKE_H_
#define _HANDSHAKE_H_

#include <openssl/sha.h>
#include <string>
#include <string.h>
#include <iostream>
#include "../base64.h"

using namespace std;
char* WS_accept(string key);

#endif