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

#include <FWComm.hpp>
#include <BoardRef.hpp>
#include <scopeSup.h>

class AcqCtrl : public BoardRef {
	unsigned bufsz_;
	unsigned buffl_;
public:
	AcqCtrl( BoardInterface * );

	virtual void
	xfer( AcqParams *set, AcqParams *get );

	virtual double
	getTriggerLevelPercent();

	virtual void
	setTriggerLevelPercent(double);

	virtual unsigned
	getNPreTriggerSamples();

	virtual void
	setNPreTriggerSamples(unsigned);

	virtual unsigned
	getNSamples();

	virtual unsigned long
	getMaxNSamples();

	virtual void
	setNSamples(unsigned);

	virtual void
	getDecimation(unsigned *cic0, unsigned *cic1);

	// auto factorize decimation into cic0, cic1
	virtual void
	computeCICDecimation(unsigned decimation, unsigned *cic0, unsigned *cic1);

	virtual void
	setDecimation(unsigned cic0,  unsigned cic1 = 0);

	virtual void
	getTriggerSrc(TriggerSource *src, bool *rising);

	virtual void
	setTriggerSrc(TriggerSource  src, bool  rising);

	virtual bool
	getExtTrigOutEnable();

	virtual void
	setExtTrigOutEnable(bool enabled);

	virtual int
	getAutoTimeoutMS();

	virtual void
	setAutoTimeoutMS(int);

	virtual double
	getScale();

	virtual void
	setScale(double);

	virtual unsigned
	getBufSampleSize();

	virtual void
	flushBuf();

	virtual unsigned
	readBuf(uint16_t *hdr, uint8_t *buf, size_t len);

	virtual unsigned
	readBuf(uint16_t *hdr, float   *buf, size_t len);

	virtual int
	getIrqFD(unsigned mask);

	static bool
	bufHdrFlagOverrange(uint16_t hdr, unsigned ch)
	{
		return !! (hdr & FW_BUF_HDR_FLG_OVR(ch));
	}
};
