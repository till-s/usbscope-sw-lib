#pragma once

#include <memory>
#include <FWComm.hpp>

class PGA;
typedef std::shared_ptr<PGA> PGAPtr;

class PGA {
public:
	virtual void
	getDBRange(int *min, int *max) = 0;

	virtual void
	setDBAtt(unsigned channel, float att) = 0;

	virtual float
	getDBAtt(unsigned channel) = 0;

	static PGAPtr
	create(FWPtr fw);
};
