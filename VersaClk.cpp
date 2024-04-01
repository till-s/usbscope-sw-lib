#include <VersaClk.hpp>
#include <stdexcept>
#include <system_error>
#include <memory>
using std::shared_ptr;
using std::make_shared;
using std::map;
using std::string;

VersaClk::VersaClk( FWPtr fwp )
: FWRef( fwp ), fADC_( 0.0 )
{
}

void
VersaClk::checkStatus(int st, const char *where)
{
	if ( st < 0 ) {
			if ( -3 == st ) {
				throw std::invalid_argument(string("VersaClk::") + where + " -- invalid output");
			}
			throw FWCommIOError( string("VersaClk::") + where );
	}
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
	checkStatus( st, "setOutDiv()" );
}

double
VersaClk::getOutDiv(int out)
{
	double div;
	int st = versaClkGetOutDivFlt( (*this)->fw_, out, &div );
	checkStatus( st, "getOutDiv()" );
	return div;
}

void
VersaClk::setFODRoute(int out, VersaClkFODRoute rte)
{
	int st = versaClkSetFODRoute( (*this)->fw_, out, rte );
	checkStatus(st, "setFODRoute()" );
}
