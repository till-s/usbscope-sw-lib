#pragma once

#include <utility>

#include <scopeSup.h>

#include <BoardRef.hpp>

class ClockOut : public BoardRef {
	bool present_;
public:
	ClockOut(BoardInterface *);

	bool isPresent() const { return present_; }

	double getMinFrequencyHz();
	double getMaxFrequencyHz();

	std::pair<double, bool> getFrequencyHz();
	void   setFrequencyHz(double);

	double getReferenceFrequencyHz();

	void   setToReference();
};
