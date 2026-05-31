/**LB-MIT
 *
 * MIT License
 *
 * Copyright (c) 2026 Till Straumann
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **LE-MIT*/

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

