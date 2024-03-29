#pragma once

#include <PGA.hpp>
#include <FWComm.hpp>

class PGAAD8370 : public PGA, public FWRef {
public:
	PGAAD8370( FWPtr fwp )
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


