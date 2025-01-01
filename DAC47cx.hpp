#pragma once

#include <FWComm.hpp>
#include <SlowDAC.hpp>

class DAC47cx : public SlowDAC, public FWRef {
public:
	DAC47cx( FWPtr fwp )
	: FWRef( fwp )
	{
	}

	virtual void
	resetDAC() override;

	virtual void
	getVoltsRange( double *vmin, double *vmax ) override;

	virtual void
	setTicks(int channel, int ticks);

	virtual int
	getTicks(int channel);

	virtual void
	setVolts(int channel, double volt) override;

	virtual double
	getVolts(int channel) override;

	virtual void
	setRefInternalX1();
};
