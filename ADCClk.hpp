#pragma once

#include <FWComm.hpp>
#include <memory>

class ADCClk;

typedef std::shared_ptr<ADCClk> ADCClkPtr;

class ADCClk {
public:
	virtual double
	getFreq() = 0;

	virtual void
	init()    = 0;

	virtual ~ADCClk() {}

	static ADCClkPtr create( FWPtr );
};
