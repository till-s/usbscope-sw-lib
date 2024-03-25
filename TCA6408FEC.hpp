#include <I2CFEC.hpp>

class TCA6408FEC : public I2CFEC {
public:
	TCA6408FEC( FWPtr fwp );

	virtual void init();

	virtual unsigned getOutReg(int channel)
	{
		return 0x01;
	}

	virtual void setAllOutputs();

	virtual int hasACModeCtl(int channel);

	virtual int hasAttenuatorCtl(int channel);

	virtual int hasDACRangeCtl(int channel);

	virtual int hasTerminationCtl(int channel);
};
