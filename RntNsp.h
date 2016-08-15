#ifndef _RtNsp_h
#define _RtNsp_h

#include <cstdint>

//wielkosc bufora musi byc rowna dlugosci ramki
#define NSP_BUFF_SIZE		20

// Ramka
// - start 2000 us
// - stop  4000 us
// - bit1  1000 us
// - bit1   500 us

#define NSP_START_MIN		1500
#define NSP_START_MAX		3000

#define NSP_STOP_MIN		3500
#define NSP_STOP_MAX		5000

#define NSP_BIT1_MIN		 900
#define NSP_BIT1_MAX		1200

#define NSP_BIT0_MIN		 400
#define NSP_BIT0_MAX		 700

//wszystkie wrtosci parametrow razy 10
class RntNsp {

	private:

		typedef struct {
			int16_t value;
			uint64_t timestamp;
		} Param;

		Param press;
		Param temp;
		Param hppl;
		Param rh;
		Param ah;
		Param dp;

		uint64_t lastTick;
		uint64_t lastFrame;

		uint32_t frameCounter;
		uint32_t timeout;

		uint8_t bit;
		uint8_t byte;

		uint8_t buff[NSP_BUFF_SIZE];

		uint32_t errTime;
		uint32_t errFrame;
		uint32_t errHeader;
		uint32_t errCrc;

		bool start;
		bool stop;

		uint8_t crc8(uint8_t *buffer, uint8_t size);
		void clearBuffer();
		void checkBuffer();


	public:

		enum Parameter {ParamTemp, ParamRH, ParamAH, ParamPress, ParamHPPL};
		enum Error {ErrTime, ErrFrame, ErrHeader, ErrCrc};

		RntNsp();

		void tick(int state);
		void setTimeout(uint32_t t);
		bool isValid(Parameter p);
		int16_t getParam(Parameter p);
		uint32_t getError(Error e);
		uint64_t getLastFrameTime();
		uint32_t getFrameCounter();

		uint64_t getTimestamp();

};

#endif //_RtNsp_h
