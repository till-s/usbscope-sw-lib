#pragma once
#include <memory>
#include <vector>
#include <stdexcept>

#include <FWComm.hpp>
#include <LED.hpp>
#include <PGA.hpp>
#include <FEC.hpp>
#include <ADCClk.hpp>
#include <AcqCtrl.hpp>
#include <SlowDAC.hpp>
#include <BoardRef.hpp>

#include <scopeSup.h>

class Max195xxADC;

typedef std::shared_ptr<Max195xxADC> ADCPtr;

class Board;

struct ScopePvt;

class Board : public BoardInterface, public BoardRef {
protected:
	AcqCtrl                    acq_;
	ADCClkPtr                  adcClk_;
	PGAPtr                     pga_;
	LEDPtr                     leds_;
	FECPtr                     fec_;
	SlowDACPtr                 dac_;
	ADCPtr                     adc_;
	bool                       sim_;

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

	// full-scale volts at max. gain
	double getVoltScale(int channel);

	int getFullScaleTicks();

	void
	setVoltScale(int channel, double scl);
};

