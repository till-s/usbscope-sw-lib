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
	computeCIC(unsigned decimation, unsigned *cic0, unsigned *cic1);

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
