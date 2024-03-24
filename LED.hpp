#pragma once

#include <FWComm.hpp>
#include <string>

class LED;

typedef std::shared_ptr<LED> LEDPtr;

class LED {
public:
	virtual bool getVal(const std::string &name)           = 0;
	virtual void setVal(const std::string &name, bool val) = 0;
	virtual ~LED()
	{
	}

	virtual void test() = 0;

	static LEDPtr create(FWPtr);
};
