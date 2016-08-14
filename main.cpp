#include <wiringPi.h>
#include "RntNsp.h"
#include <iostream>

#define NSP_PIN		1

using namespace std;


RntNsp nsp;

void nspISR() {
	nsp.tick(digitalRead(NSP_PIN));
}

int main(int argc, const char* argv[]) {

	wiringPiSetup();
	//piHiPri(50);
	pinMode(NSP_PIN, INPUT);
	pullUpDnControl(NSP_PIN, PUD_UP);
	wiringPiISR(NSP_PIN, INT_EDGE_BOTH, &nspISR);

	pinMode (4, OUTPUT);
	digitalWrite(4, HIGH);

	while(1) {
		digitalWrite(4, LOW);
		delayMicroseconds(300);
		digitalWrite(4, HIGH);
		delay(1000);
	}
	return 0;
}
