#pragma once

#include <FWComm.hpp>
#include <LED.hpp>
#include <PGA.hpp>
#include <FEC.hpp>
#include <ADCClk.hpp>
#include <AcqCtrl.hpp>
#include <SlowDAC.hpp>
#include <memory>

class Max195xxADC;

typedef std::shared_ptr<Max195xxADC> ADCPtr;

class Board : public FWRef {
protected:
	AcqCtrl      acq_;
	ADCClkPtr    adcClk_;
	PGAPtr       pga_;
	LEDPtr       leds_;
	FECPtr       fec_;
	SlowDACPtr   dac_;
	ADCPtr       adc_;

	Board( const Board & )     = delete;

	Board &
	operator=( const Board & ) = delete;
public:

	static const int NumChannels = 2;

	Board( FWPtr fwp );

	void hwInit(bool force);

protected:
	void CLKInit();
	void ADCInit();
	void FECInit();
	void DACInit();
};

