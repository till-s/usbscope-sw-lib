#pragma once

#include <FWComm.hpp>

class SlowDAC {
public:

	virtual void
	reset()  = 0;

	virtual void
	init()  = 0;

	virtual void
	getVoltRange( float *vmin, float *vmax ) = 0;

	virtual void
	setTicks(int channel, int ticks) = 0;

	virtual int
	getTicks(int channel) = 0;

	virtual void
	setVolt(int channel, float volt) = 0;

	virtual float
	getVolt(int channel) = 0;
};
