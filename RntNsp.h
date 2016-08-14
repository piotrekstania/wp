#ifndef _RtNsp_h
#define _RtNsp_h

#include <cstdint>

class RntNsp {

	private:
		uint64_t lastTick;

		uint8_t bit;
		uint8_t byte;

		int32_t press;
		int32_t temp;
		int32_t hppl;
		int32_t rh;
		int32_t ah;

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
