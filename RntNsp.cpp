#include <chrono>
#include <cstring>
#include <wiringPi.h>
#include "RntNsp.h"
#include <iostream>

using namespace std::chrono;
using namespace std;


RntNsp::RntNsp() {
	clearBuffer();
	timeout = 10;
}

void RntNsp::setTimeout(uint32_t t) {
	if(t > 100) t = 100;
	timeout = t;
}

uint64_t RntNsp::getTimestamp() {
	return (uint64_t)duration_cast<std::chrono::seconds>(system_clock::now().time_since_epoch()).count();
}

bool RntNsp::isValid(Parameter p) {
	Param *pParam;

	switch (p) {
		case ParamTemp: pParam = &temp; break;
		case ParamRH: pParam = &rh; break;
		case ParamAH: pParam = &ah; break;
		case ParamPress: pParam = &press; break;
		case ParamHPPL: pParam = &hppl; break;
		case ParamDP: pParam = &dp; break;
	}

	if((getTimestamp() - pParam->timestamp) <= timeout) return true;
	return false;
}


uint32_t RntNsp::getError(Error e) {
	switch(e) {
		case ErrTime: return errTime;
		case ErrFrame: return errFrame;
		case ErrHeader: return errHeader;
		case ErrCrc: return errCrc;
		case ErrAll: return (errTime + errFrame + errHeader + errCrc);
	}
	return 0;
}

uint32_t RntNsp::getFrameCounter() {
	return frameCounter;
}


void RntNsp::clearBuffer() {
	memset(buff, 0, NSP_BUFF_SIZE);
	bit = 0;
	byte = 0;
	start = false;
	stop = false;
}

void RntNsp::checkBuffer() {


	if(byte != 0) errFrame++;
	else if((buff[0] != '$') || (buff[NSP_BUFF_SIZE-1] != '#')) errHeader++;
	else if(crc8(buff, NSP_BUFF_SIZE-2) != buff[NSP_BUFF_SIZE-2]) errCrc++;
	else {
		//dekodowanie ramki
		frameCounter++;
	}

	clearBuffer();
}

void RntNsp::tick(int state) {

	uint64_t tick = duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
	uint64_t diff = (tick - lastTick);
	lastTick = tick;

	if((state == 0) || (stop == true)) return;

	if((diff >= NSP_START_MIN) && (diff <= NSP_START_MAX)) {
		if((bit != 0) || (byte != 0)) errFrame++;
		clearBuffer();
		start = true;
		return;
	}

	if(start == true) {
		if((diff >= NSP_STOP_MIN) && (diff <= NSP_STOP_MAX)) {
			stop = true;
			checkBuffer();
		} else if((diff >= NSP_BIT0_MIN) && (diff <= NSP_BIT0_MAX)) {
			buff[byte] &= ~(1<<bit++);
		} else if((diff >= NSP_BIT1_MIN) && (diff <= NSP_BIT1_MAX)) {
			buff[byte] |= (1<<bit++);
		} else {
			errTime++;
			clearBuffer();
		}
	}

	if(bit == 8) {
		bit = 0;
		byte++;
		if(byte == NSP_BUFF_SIZE) byte = 0;
	}
}

uint8_t RntNsp::crc8(uint8_t *buffer, uint8_t size) {

	uint8_t	 crc = 0;
	uint8_t loop_count;
	uint8_t  bit_counter;
	uint8_t  data;
	uint8_t  feedback_bit;

	for(loop_count = 0; loop_count != size; loop_count++) {
		data = buffer[loop_count];
		bit_counter = 8;
		do {
			feedback_bit = (crc ^ data) & 0x01;
			if(feedback_bit == 0x01) crc = crc ^ 0x18;	//0X18 = X^8+X^5+X^4+X^0
			crc = (crc >> 1) & 0x7F;
			if(feedback_bit == 0x01) crc = crc | 0x80;
			data = data >> 1;
			bit_counter--;
		} while (bit_counter > 0);
	}
	return crc;
}
