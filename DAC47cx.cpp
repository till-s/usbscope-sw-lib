#include <DAC47cx.hpp>
#include <dac47cxSup.h>
#include <string>

using std::string;
static void
checkRes(int st, const char *where)
{
	if ( st < 0 ) {
			if ( -2 == st ) {
				throw std::invalid_argument(string("DAC47cx::") + where + " -- invalid channel");
			}
			throw FWCommIOError( string("DAC47cx::") + where );
	}
}

void
DAC47cx::resetDAC()
{
	if ( dac47cxReset( (*this)->fw_ ) ) {
		throw FWCommIOError( "DAC47xx::reset()" );
	}
}

void
DAC47cx::getVoltRange( double *pvmin, double *pvmax )
{
	float vmin, vmax;

	dac47cxGetRange( (*this)->fw_, NULL, NULL, &vmin, &vmax );
	if ( pvmin ) {
		*pvmin = (double)vmin;
	}
	if ( pvmax ) {
		*pvmax = (double)vmax;
	}
}

void
DAC47cx::setTicks(int channel, int ticks)
{
	int st = dac47cxSet( (*this)->fw_, channel, ticks );
	checkRes( st, "setTicks()" );
}

int
DAC47cx::getTicks(int channel)
{
	uint16_t ticks;
	int st = dac47cxGet( (*this)->fw_, channel, &ticks );
	checkRes( st, "getTicks()" );
	return ticks;
}

void
DAC47cx::setVolt(int channel, double volt)
{
	int st = dac47cxSetVolt( (*this)->fw_, channel, (float)volt );
	checkRes( st, "setVolt()" );
}

double
DAC47cx::getVolt(int channel)
{
	float volt;
	int st = dac47cxGetVolt( (*this)->fw_, channel, &volt );
	checkRes( st, "getVolt()" );
	return (double)volt;
}

void
DAC47cx::setRefInternalX1()
{
	int st = dac47cxSetRefSelection( (*this)->fw_, DAC47XX_VREF_INTERNAL_X1 );
	if ( st < 0 ) {
		throw FWCommIOError( "DAC47cx::setRefInternalX1()" );
	}
}
