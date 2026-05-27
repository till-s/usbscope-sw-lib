#pragma once
#include <memory>
#include <vector>
#include <stdexcept>

#include <scopeSup.h>

#include <BoardRef.hpp>
#include <FWComm.hpp>
#include <LED.hpp>
#include <PGA.hpp>
#include <FEC.hpp>
#include <ADCClk.hpp>
#include <AcqCtrl.hpp>
#include <SlowDAC.hpp>
#include <Flash.hpp>
#include <ClockOut.hpp>

class Max195xxADC;

typedef std::shared_ptr<Max195xxADC> ADCPtr;
typedef std::shared_ptr<Flash>       FlashPtr;
typedef std::shared_ptr<ClockOut>    ClockOutPtr;

class Board;

struct ScopePvt;

class Board : public BoardInterface, public BoardRef {
protected:
	bool                       sim_;
	AcqCtrl                    acq_;
	ADCClkPtr                  adcClk_;
	PGAPtr                     pga_;
	LEDPtr                     leds_;
	FECPtr                     fec_;
	SlowDACPtr                 dac_;
	ADCPtr                     adc_;
	FlashPtr                   flash_;
	ClockOutPtr                clockOut_;

	Board( const Board & )     = delete;

	Board &
	operator=( const Board & ) = delete;
public:

	Board(FWPtr fwp, bool sim = false );

	bool simulation() const
	{
		return sim_;
	}

	void lock();
	void unlock();

	void hwInit(bool force);

	unsigned getNumChannels();

	unsigned getSampleSize();

	// full-scale volt at max. gain
	virtual double getVoltScale(int channel);

	int getFullScaleTicks();

	virtual void
	setVoltScale(int channel, double scl);
};

