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

	static LEDPtr create(FWPtr);
};

class LEDNone : public LED {
public:
	LEDNone()
	{
	}

	virtual bool getVal(const std::string &name) override
	{
		return false;
	}

	virtual void setVal(const std::string &name, bool val) override
	{
	}

};
