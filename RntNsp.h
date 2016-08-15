#ifndef _RtNsp_h
#define _RtNsp_h

#include <cstdint>

//wielkosc bufora musi byc rowna dlugosci ramki
#define NSP_BUFF_SIZE		20

#define NSP_START_MIN		4000
#define NSP_START_MAX		6000

#define NSP_STOP_MIN		1900
#define NSP_STOP_MAX		2100

#define NSP_BIT0_MIN		250
#define NSP_BIT0_MAX		649

#define NSP_BIT1_MIN		650
#define NSP_BIT1_MAX		1100


class RntNsp {

	private:

		typedef struct {
			int16_t value;
			int16_t min;
			int16_t max;
			uint64_t timestamp;
		} Param;

		Param press;
		Param temp;
		Param hppl;
		Param rh;
		Param ah;

		uint64_t lastTick;
		uint64_t lastFrame;

		uint32_t frameCounter;

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
		void setRange(Parameter p, int16_t min, int16_t max);
		bool isValid(Parameter p);
		int16_t getParam(Parameter p);
		uint32_t getError(Error e);
		uint64_t getLastFrameTime();
		uint32_t getFrameCounter();

};

#endif //_RtNsp_h
