#include <lmh6882Sup.h>
#include <PGALmh6882.hpp>
#include <string>

using std::string;

static void
checkRes(float st, const char *where)
{
	if ( st < 0.0 ) {
			if ( -2.0 == st ) {
				throw std::invalid_argument(string("PGALmh6882::") + where + " -- invalid channel or attenuation");
			}
			throw FWCommIOError( string("PGALmh6882::") + where );
	}
}


void
PGALmh6882::getDBRange(int *min, int *max)
{
	if ( min ) *min = 0;
	if ( max ) *max = 0;
}

void
PGALmh6882::setDBAtt(unsigned channel, float att)
{
	float rv = lmh6882SetAtt( (*this)->fw_, channel, att );
	checkRes( rv, "setDBAtt()" );
}

float
PGALmh6882::getDBAtt(unsigned channel)
{
	float rv = lmh6882GetAtt( (*this)->fw_, channel );
	checkRes( rv, "getDBAtt()" );
	return rv;
}
