#pragma once

#include <FWComm.hpp>
#include <memory>

class ADCClk;

typedef std::shared_ptr<ADCClk> ADCClkPtr;

class BoardInterface;

class ADCClk {
public:
	virtual double
	getFreq() = 0;

	virtual ~ADCClk() {}

	static ADCClkPtr create( BoardInterface * );
};
