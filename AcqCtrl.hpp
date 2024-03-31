#pragma once

#include <FWComm.hpp>

class AcqCtrl : public FWRef {
	unsigned bufsz_;
	unsigned buffl_;
public:
	AcqCtrl( FWPtr );

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

	virtual void
	setDecimation(unsigned cic0,  unsigned cic1 = 0);

	virtual void
	getTriggerSrc(TriggerSource *src, bool *rising);

	virtual void
	setTriggerSrc(TriggerSource  src, bool  rising);

	virtual int
	getAutoTimeoutMS();

	virtual void
	setAutoTimeoutMS(int);

	virtual double
	getScale();

	virtual void
	setScale(double);

	virtual unsigned long
	getBufSize();

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
