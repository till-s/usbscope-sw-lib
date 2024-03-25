#pragma once

class PGA {
public:
	virtual void
	getDBRange(int *min, int *max) = 0;

	virtual void
	setDBAtt(unsigned channel, float att) = 0;

	virtual float
	getDBAtt(unsigned channel) = 0;
};