#pragma once

#include <FWComm.hpp>

class AcqCtrl : public FWRef {
	unsigned bufsz_;
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
};
