#pragma once

#include <PGA.hpp>
#include <FWComm.hpp>

class PGALmh6882 : public PGA, public FWRef {
public:
	PGALmh6882( FWPtr fwp )
	: FWRef( fwp )
	{
	}

	virtual void
	getDBRange(int *min, int *max);

	virtual void
	setDBAtt(unsigned channel, float att);

	virtual float
	getDBAtt(unsigned channel);
};
