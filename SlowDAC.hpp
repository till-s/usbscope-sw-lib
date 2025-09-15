#pragma once

#include <FWComm.hpp>
#include <memory>

class SlowDAC;

typedef std::shared_ptr<SlowDAC> SlowDACPtr;

class SlowDAC {
public:

	virtual void
	resetDAC()  = 0;

	virtual void
	getVoltsRange( double *vmin, double *vmax ) = 0;

	virtual void
	setVolts(int channel, double volt) = 0;

	virtual double
	getVolts(int channel) = 0;

	virtual bool
	getRangeHigh(int channel) = 0;

	virtual void
	setRangeHigh(int channel, bool on) = 0;

};
