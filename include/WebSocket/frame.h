#ifndef _FRAME_H_
#define _FRAME_H_

#include <vector>
#include <cstdint>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "../base64.h"

using namespace std;


class Frame{
	public:
		/**
		 * Bits 
		 */
		uint8_t fin;
		uint8_t rsv1;
		uint8_t rsv2;
		uint8_t rsv3;
		uint8_t mask;

		uint8_t opcode;
		uint32_t mask_key;
		vector<uint8_t>* data;
		uint32_t length;

		Frame();
		Frame(uint8_t fin,uint8_t rsv1, uint8_t rsv2,uint8_t rsv3,uint8_t mask,uint8_t opcode);
		Frame(uint8_t* in);
		virtual ~Frame();
		void add(uint8_t* data);
		void merge(Frame* frame);
		void debug();
		char* get_cstr();
		static uint8_t* c32_to_8(uint32_t in);
		uint8_t* encode(uint32_t* out_size = nullptr);
		void send(int sd);
		void clear();

};

enum opcodes{
	CONTINUE = 0x0,
	TEXT = 0x1,
	BINARY = 0x2,
	ERROR = 0x8,
	PING = 0x9,
	PONG = 0xA
};

enum class BitLoc{ 
	FIN = 31,
	RSV1 = 30,
	RSV2 = 29,
	RSV3 = 28,
	OPCODE = 24,
	MASK = 23,
	LENGTH = 16
};


#endif