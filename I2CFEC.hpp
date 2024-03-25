#pragma once

#include <FEC.hpp>
#include <I2CDev.hpp>

class I2CFEC : public FEC, public I2CDev {
private:
	unsigned sla_;

public:

	I2CFEC( FWPtr fwp, unsigned sla );

	virtual unsigned getSla()
	{
		return sla_;
	}

	virtual unsigned getOutReg(int channel) = 0;
	virtual void     allOutputs()           = 0;

	virtual bool getBit(int channel, int bit);
	virtual void setBit(int channel, int bit, int val);

	virtual void setAttenuator(int channel, bool on);

	virtual bool getAttenuator(int channel);

	virtual void setTermination(int channel, bool on);

	virtual bool getTermination(int channel);

	virtual void setACMode(int channel, bool on);

	virtual bool getACMode(int channel);

	virtual void setDacRangeHi(int channel, bool on);

	virtual bool getDacRangeHi(int channel);
};
