#include <cstdlib>
#include <cstring>
#include <wiringPi.h>
#include "RntNsp.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;


// Ramka +/- 100 us:
// 	- start: 4000 us (3900..4100)
// 	- bit 0:  300 us (200..400)
// 	- bit 1:  700 us (600..800)
// 	- stop:	 2000 us (1900..2100)

#define START_MIN		4000
#define START_MAX		6000

#define BIT0_MIN		200
#define BIT0_MAX		500

#define BIT1_MIN		600
#define BIT1_MAX		900

#define STOP_MIN		1900
#define STOP_MAX		2100

uint8_t crc8(uint8_t *buffer, uint8_t size) {
	
	uint8_t	 crc = 0;
	uint8_t loop_count;
	uint8_t  bit_counter;
	uint8_t  data;
	uint8_t  feedback_bit;

	for (loop_count = 0; loop_count != size; loop_count++)
	{
		data = buffer[loop_count];
		
		bit_counter = 8;
		do {
			feedback_bit = (crc ^ data) & 0x01;
			
			if ( feedback_bit == 0x01 ) {
				crc = crc ^ 0x18;	             //0X18 = X^8+X^5+X^4+X^0
			}
			crc = (crc >> 1) & 0x7F;
			if ( feedback_bit == 0x01 ) {
				crc = crc | 0x80;
			}
			
			data = data >> 1;
			bit_counter--;
			
		} while (bit_counter > 0);
	}

	return crc;
}

void RntNsp::tick(int state) {
	uint64_t tick = duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
	uint64_t diff = (tick - lastTick);
	lastTick = tick;

	uint32_t err=0;

	if(state == 0) return;

	if((diff >= START_MIN) && (diff <= START_MAX)) {
		if(by != 0) { errFrame++; err++;}
		bi = 0;
		by = 0;
		memset(buf, 0, sizeof(buf));
	} else if((diff >= BIT0_MIN) && (diff <= BIT0_MAX)) {
		bi++;
	} else if((diff >= BIT1_MIN) && (diff <= BIT1_MAX)) {
		buf[by] |= (1<<bi);
		bi++;
	} else {
		errTime++;
		err++;
		bi = 0;
		by = 10;
	}


	if(bi == 8) {
		bi = 0;
		by++;
	}

	if(by == 10) {

		if(err==0) {
 			if((buf[0] != '$') || (buf[9] != '#')) errHeader++;
			else if(err==0) if(buf[8] != crc8(buf, 8)) errCrc++;
		}

		int16_t temp = buf[4];
		temp <<= 8;
		temp += buf[5];
		temp /= 10;

		uint16_t rh = buf[6];
		rh <<= 8;
		rh += buf[7];
		rh /= 10;

		bi = 0;
		by = 0;
		memset(buf, 0, sizeof(buf));

		system ("clear");
		cout<<"Frame: "<<++frame<<endl;
		cout<<"T: "<<(temp/10)<<"."<<(temp%10)<<" °C"<<endl;
		cout<<"H: "<<(rh/10)<<"."<<(rh%10)<<" %"<<endl;
		uint32_t total = (errTime+errFrame+errHeader+errCrc);
		float proc = (total * 10000)/frame;
		proc /= 100;
		cout<<"Errors: "<<total<<" ("<<proc<<" %)"<<" (Time: "<<errTime<<" Frame: "<<errFrame<<" Header: "<<errHeader<<" Crc: "<<errCrc<<")"<<endl;
	}

}
