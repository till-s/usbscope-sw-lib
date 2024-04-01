#pragma once

#include <stdexcept>
#include <memory>
#include <FWComm.hpp>

class FEC;

typedef std::shared_ptr<FEC> FECPtr;

class FEC {
public:

	virtual int hasACModeCtl(int channel)
	{
		throw std::runtime_error("Front-End has no AC-coupling controller switch");
	}

	virtual int hasAttenuatorCtl(int channel)
	{
		throw std::runtime_error("Front-End has no Attenuator controls");
	}

	virtual int hasDACRangeCtl(int channel)
	{
		throw std::runtime_error("Front-End has no DAC range controls");
	}

	virtual int hasTerminationCtl(int channel)
	{
		throw std::runtime_error("Front-End has no Termination controls");
	}

	virtual void setAttenuator(int channel, bool on)
	{
		throw std::runtime_error("Front-End has no Attenuator controls");
	}

	virtual bool getAttenuator(int channel)
	{
		throw std::runtime_error("Front-End has no Attenuator controls");
	}

	virtual void setTermination(int channel, bool on)
	{
		throw std::runtime_error("Front-End has no Termination controls");
	}

	virtual bool getTermination(int channel)
	{
		throw std::runtime_error("Front-End has no Termination controls");
	}

	virtual void setACMode(int channel, bool on)
	{
		throw std::runtime_error("Front-End has no AC-coupling controller switch");
	}

	virtual bool getACMode(int channel)
	{
		throw std::runtime_error("Front-End has no AC-coupling controller switch");
	}

	virtual void setDACRangeHi(int channel, bool on)
	{
		throw std::runtime_error("Front-End has no DAC range controls");
	}

	virtual bool getDACRangeHi(int channel)
	{
		throw std::runtime_error("Front-End has no DAC range controls");
	}

	virtual ~FEC()
	{
	}

	static FECPtr
	create( FWPtr );
};
