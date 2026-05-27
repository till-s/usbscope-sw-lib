#pragma once

#include <utility>

#include <scopeSup.h>

#include <BoardRef.hpp>

class ClockOut : public BoardRef {
public:
	ClockOut(BoardInterface *);

	bool isPresent();

	double getMinFrequencyHz();
	double getMaxFrequencyHz();

	std::pair<double, bool> getFrequencyHz();
	void   setFrequencyHz(double);

	void   setToReference();
};
