#pragma once

#include <FWComm.hpp>
#include <SlowDAC.hpp>

class DAC47cx : public virtual SlowDAC, public FWRef {
public:
	DAC47cx( FWPtr fwp )
	: FWRef( fwp )
	{
	}

	virtual void
	resetDAC() override;

	virtual void
	getVoltRange( double *vmin, double *vmax ) override;

	virtual void
	setTicks(int channel, int ticks);

	virtual int
	getTicks(int channel);

	virtual void
	setVolt(int channel, double volt) override;

	virtual double
	getVolt(int channel) override;

	virtual void
	setRefInternalX1();
};
