#include <AD8370PGA.hpp>
#include <ad8370Sup.h>
#include <math.h>
#include <string>

using std::string;

static void
checkRes(int st, const char *where)
{
	if ( st < 0 ) {
			if ( -2 == st ) {
				throw std::invalid_argument(string("PGAAD8370::") + where + " -- invalid channel or attenuation");
			}
			throw FWCommIOError( string("PGAAD8370::") + where );
	}
}


void
PGAAD8370::getDBRange(int *min, int *max)
{
	if ( min ) *min = 0;
	if ( max ) *max = 40;
}

void
PGAAD8370::setDBAtt(unsigned channel, float att)
{
	int rv = ad8370SetAtt( (*this)->fw_, channel, att );
	checkRes( rv, "setDBAtt()" );
}

float
PGAAD8370::getDBAtt(unsigned channel)
{
	float rv = ad8370GetAtt( (*this)->fw_, channel );
	if ( isnan( rv ) ) {
		throw std::runtime_error( "ad8370GetAtt() failed" );
	}
	return rv;
}
