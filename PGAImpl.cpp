#include <PGAImpl.hpp>
#include <math.h>
#include <string>

using std::string;

static void
checkRes(int st, const char *where)
{
	if ( st < 0 ) {
			if ( -2 == st ) {
				throw std::invalid_argument(string("PGAImpl::") + where + " -- invalid channel or attenuation");
			}
			throw FWCommIOError( string("PGAImpl::") + where );
	}
}

PGAImpl::PGAImpl( FWPtr fwp )
: FWRef( fwp )
{
}

void
PGAImpl::getDBRange(int *min, int *max)
{
	double attMin, attMax;
	int rv = pgaGetAttRange( (*this)->fw_, &attMin, &attMax );

	checkRes( rv, "getDBRange()" );
	if ( min ) *min = round( attMin );
	if ( max ) *max = round( attMax );
}

void
PGAImpl::setDBAtt(unsigned channel, float att)
{
	int rv = pgaSetAtt( (*this)->fw_, channel, att );
	checkRes( rv, "setDBAtt()" );
}

float
PGAImpl::getDBAtt(unsigned channel)
{
	double att;
	int   rv = pgaGetAtt( (*this)->fw_, channel, &att );
	checkRes( rv, "getDBAtt()" );
	return att;
}
