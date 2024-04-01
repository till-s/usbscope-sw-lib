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
	reset() override;

	virtual void
	getVoltRange( float *vmin, float *vmax ) override;

	virtual void
	setTicks(int channel, int ticks) override;

	virtual int
	getTicks(int channel) override;

	virtual void
	setVolt(int channel, float volt) override;

	virtual float
	getVolt(int channel) override;

	virtual void
	setRefInternalX1();
};
