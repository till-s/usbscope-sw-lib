#pragma once

#include <FWComm.hpp>
#include <LED.hpp>
#include <PGA.hpp>
#include <FEC.hpp>
#include <ADCClk.hpp>
#include <AcqCtrl.hpp>
#include <SlowDAC.hpp>
#include <memory>
#include <vector>

class Max195xxADC;

typedef std::shared_ptr<Max195xxADC> ADCPtr;

class Board : public FWRef {
protected:
	AcqCtrl                    acq_;
	ADCClkPtr                  adcClk_;
	PGAPtr                     pga_;
	LEDPtr                     leds_;
	FECPtr                     fec_;
	SlowDACPtr                 dac_;
	ADCPtr                     adc_;
	bool                       sim_;
	std::vector<double>        vVoltScale_;

	Board( const Board & )     = delete;

	Board &
	operator=( const Board & ) = delete;
public:

	static const int NumChannels = 2;

	Board( FWPtr fwp, bool sim = false );

	bool simulation() const
	{
		return sim_;
	}

	void hwInit(bool force);

	// full-scale volts at max. gain
	double getVoltScale(int channel);

	void
	setVoltScale(int channel, double scl);

protected:
	void CLKInit();
	void ADCInit();
	void FECInit();
	void DACInit();
};

