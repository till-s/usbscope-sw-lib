#include <I2CFEC.hpp>

class TCA6408FEC : public I2CFEC {
public:
	TCA6408FEC( FWPtr fwp );

	virtual unsigned getOutReg(int channel) override
	{
		return 0x01;
	}

	virtual void setAllOutputs() override;

	virtual int hasACModeCtl(int channel) override;

	virtual int hasAttenuatorCtl(int channel) override;

	virtual int hasDACRangeCtl(int channel) override;

	virtual int hasTerminationCtl(int channel) override;
};
