#pragma once

#include <PGA.hpp>

class PGAImpl : public PGA, public FWRef {
public:
	PGAImpl( FWPtr fwp );

	virtual void
	getDBRange(int *min, int *max) override;

	virtual void
	setDBAtt(unsigned channel, float att) override;

	virtual float
	getDBAtt(unsigned channel) override;
};
