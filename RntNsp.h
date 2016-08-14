#ifndef _RtNsp_h
#define _RtNsp_h

#include <cstdint>

//  errory:
//  - niewlasciwa dlugosc impulsu
//  - niewlasciwa dlugosc ramki
//  - niewlasciwe znaki poczatku i konca ramki
//  - błąd sumy kontrolnej
//
//  wspolne:
//  - maksymalny czas od ostanio dobrze odebranwgo parametru
//
// osobne:
// - parametru
// - timestamp parametru
// - dopuszczalny zakres
//
// funkcje:
// - tick
// - getParam
// - isValid
// - setRange
// - getError
// - getLastTime

class RntNsp {

	private:

		typedef struct {
			int16_t value;
			int16_t min;
			int16_t max;
			uint64_t timestamp;
		} Param;

		uint64_t lastTick;

		Param pressure;
		Param temperature;
		Param hppl;
		Param rh;
		Param ah;

		uint64_t pressTs;
		uint64_t tempTs;
		uint64_t hpplTs;
		uint64_t rhTs;
		uint64_t ahTs;

		uint32_t bi;
		uint32_t by;
		uint8_t buf[10];
		uint32_t errTime;
		uint32_t errFrame;
		uint32_t errHeader;
		uint32_t errCrc;
		uint32_t frame;

		bool res;
	public:
		enum Edge  {EdgeFall, EdgeRise};
		enum Param {ParamTemp, ParamRH, ParamAH, ParamPress, ParamHPPL};

		void tick(int state);
		bool isValid(Param p);
		int32_t getParam(Param p);

};

#endif //_RtNsp_h
