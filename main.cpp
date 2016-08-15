#include <unistd.h>
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
	piHiPri(100);

	pinMode(NSP_PIN, INPUT);
	pullUpDnControl(NSP_PIN, PUD_UP);
	wiringPiISR(NSP_PIN, INT_EDGE_BOTH, &nspISR);

	while(1) {
		system("clear");
		
		delay(1000);
	}
	return 0;
}
