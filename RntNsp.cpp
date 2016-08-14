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

#define START_MIN		4900
#define START_MAX		5100

#define BIT0_MIN		200
#define BIT0_MAX		400

#define BIT1_MIN		600
#define BIT1_MAX		800

#define STOP_MIN		1900
#define STOP_MAX		2100


void RntNsp::tick(int state) {
	uint64_t tick = duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count();
	uint64_t diff = tick - lastTick;
	lastTick = tick;

	if(state == 0) return;
	cout<<diff<<endl;
}
