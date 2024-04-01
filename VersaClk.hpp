#pragma once

#include <ADCClk.hpp>
#include <versaClkSup.h>

#include <map>
#include <string>

class VersaClk : public ADCClk, public FWRef {
	typedef struct OutCfg {
		int              output;
		VersaClkOutMode  iostd;
		VersaClkOutSlew  slew;
		VersaClkOutLevel level;
	} OutCfg;

protected:

	double                        fRef_;
	double                        fADC_;
	std::map<std::string, OutCfg> outMap_;

	static void
	checkStatus(int st, const char *where);

public:
	VersaClk( FWPtr fwp );

	virtual double
	getFreq() override
	{
		return fADC_;
	}

	virtual double
	getFBDiv();

	virtual void
	setOutDiv(int out, double div);

	virtual double
	getOutDiv(int out);

	virtual void
	setFODRoute(int out, VersaClkFODRoute rte);

	virtual int
	mapOut(const char *name)
	{
		return outMap_.at( name ).output;
	}

};
