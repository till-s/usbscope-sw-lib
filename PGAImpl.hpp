#pragma once

#include <PGA.hpp>
#include <BoardRef.hpp>

class PGAImpl : public PGA, public BoardRef {
public:
	PGAImpl( BoardInterface * );

	virtual void
	getDBRange(int *min, int *max) override;

	virtual void
	setDBAtt(unsigned channel, float att) override;

	virtual float
	getDBAtt(unsigned channel) override;
};
