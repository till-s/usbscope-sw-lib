#pragma once

#include <stdexcept>
#include <memory>
#include <FWComm.hpp>
#include <BoardRef.hpp>

class FEC;
class Board;

typedef std::shared_ptr<FEC> FECPtr;

class FEC : public BoardRef {

	double min_, max_;

	FEC( const FEC & )       = delete;

	FEC &
	operator=( const FEC & ) = delete;
public:
	FEC( BoardInterface * );

	virtual void getDBRange( double *min, double *max );

	virtual void setAttenuator(int channel, bool on);
	virtual void setAttenuator(int channel, double db);

	virtual double getAttenuator(int channel);
	virtual bool isAttenuatorOn(int channel);

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
	create( BoardInterface * );
};
