#pragma once

#include <stdexcept>
#include <memory>
#include <FWComm.hpp>

class FEC;

typedef std::shared_ptr<FEC> FECPtr;

class FEC : public FWRef {

	double min_, max_;

	FEC( const FEC & )       = delete;

	FEC &
	operator=( const FEC & ) = delete;
public:
	FEC( FWPtr fwp );

	virtual void setAttenuator(int channel, bool on);

	virtual bool getAttenuator(int channel);

	virtual void setTermination(int channel, bool on);

	virtual bool getTermination(int channel);

	virtual void setACMode(int channel, bool on);

	virtual bool getACMode(int channel);

	virtual void setDACRangeHi(int channel, bool on);

	virtual bool getDACRangeHi(int channel);

	virtual ~FEC()
	{
	}

	static FECPtr
	create( FWPtr );
};
