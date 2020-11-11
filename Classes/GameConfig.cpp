#include "GameConfig.h"

extern "C"
{
	double deltaT=0.0;

	double getDeviceTime()
	{
		double ts = time(NULL);
		return ts;
	}

	double getServerTime()
	{
		return getDeviceTime()-deltaT;
	}

	double getDeltaT(){
		return deltaT;
	}

	void setDeltaT(double t){
		deltaT = t;
	}

	tm* getTm(double t){
		time_t _time = (time_t)t;
		return localtime(&_time);
	}

}