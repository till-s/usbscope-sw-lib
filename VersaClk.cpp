#include <ADCClk.hpp>
#include <versaClkSup.h>

#include <memory>
#include <map>
#include <string>
#include <stdexcept>
#include <system_error>

using std::shared_ptr;
using std::make_shared;
using std::map;
using std::string;

class VersaClk : public ADCClk, public FWRef {
	typedef struct OutCfg {
		int              output;
		VersaClkOutMode  iostd;
		VersaClkOutSlew  slew;
		VersaClkOutLevel level;
	} OutCfg;

	double              fRef_;
	double              fADC_;
	map<string, OutCfg> outMap_;

public:
	VersaClk( FWPtr fwp );

	virtual double
	getFreq()
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

	virtual void
	init();

	virtual int
	mapOut(const char *name)
	{
		return outMap_.at( name ).output;
	}

};

VersaClk::VersaClk( FWPtr fwp )
: FWRef( fwp ), fADC_( 0.0 )
{
	if ( 0 == fwp->getBoardVersion() ) {
		outMap_["EXT"]  = {1, OUT_CMOS, SLEW_100, LEVEL_18};
		outMap_["ADC"]  = {2, OUT_CMOS, SLEW_100, LEVEL_18};
		outMap_["FPGA"] = {4, OUT_CMOS, SLEW_100, LEVEL_18};
		fRef_           = 25.0e6;
		fADC_           = 130.0e6;
	} else {
		outMap_["EXT"]  = {2, OUT_CMOS, SLEW_100, LEVEL_18};
		outMap_["ADC"]  = {3, OUT_LVDS, SLEW_100, LEVEL_18};
		outMap_["FPGA"] = {1, OUT_CMOS, SLEW_100, LEVEL_18};
		fRef_           = 26.0e6;
		fADC_           = 120.0e6;
	}
}


static void
checkRes(int st, const char *where)
{
	if ( st < 0 ) {
			if ( -3 == st ) {
				throw std::invalid_argument(string("VersaClk::") + where + " -- invalid output");
			}
			throw FWCommIOError( string("VersaClk::") + where );
	}
}

void
VersaClk::init()
{
	auto it  = outMap_.cbegin();
	auto ite = outMap_.cend();
	while ( it != ite ) {
		int st = versaClkSetOutCfg( (*this)->fw_, it->second.output, it->second.iostd, it->second.slew, it->second.level );
		checkRes( st, "setOutCfg()" );
		it++;
	}
	double fVCO   = fRef_ * getFBDiv();
	double outDiv = fVCO / fADC_ / 2.0;
	setOutDiv( outMap_["ADC"].output, outDiv );
	setOutDiv( outMap_["EXT"].output, 4095.0 );
	setFODRoute( outMap_["ADC"].output, NORMAL );
	setFODRoute( outMap_["EXT"].output, CASC_FOD );
}

double
VersaClk::getFBDiv()
{
	double div;
	if ( versaClkGetFBDivFlt( (*this)->fw_, &div ) < 0 ) {
		throw FWCommIOError( "VersaClk::getFBDiv()" );
	}
	return div;
}

void
VersaClk::setOutDiv(int out, double div)
{
	int st = versaClkSetOutDivFlt( (*this)->fw_, out, div );
	checkRes( st, "setOutDiv()" );
}

double
VersaClk::getOutDiv(int out)
{
	double div;
	int st = versaClkGetOutDivFlt( (*this)->fw_, out, &div );
	checkRes( st, "getOutDiv()" );
	return div;
}

void
VersaClk::setFODRoute(int out, VersaClkFODRoute rte)
{
	int st = versaClkSetFODRoute( (*this)->fw_, out, rte );
	checkRes(st, "setFODRoute()" );
}

ADCClkPtr
ADCClk::create( FWPtr fwp )
{
	return make_shared<VersaClk>( fwp );
}
