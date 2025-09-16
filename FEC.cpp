#include <FEC.hpp>
#include <string.h>
#include <string>
#include <math.h>
#include <scopeSup.h>

using std::string;

static void check(int st, const char *notsupmsg)
{
	if ( st < 0 ) {
		if ( -ENOTSUP == st ) {
			throw std::runtime_error( notsupmsg );
		}
		throw std::runtime_error( string("FEC control operation failed: ") + strerror( - st ));
	}
}

FEC::FEC( BoardInterface *brd )
: BoardRef( brd )
{
int st;
	st = fecGetAttRange( (*this)->scope(), &min_, &max_ );
	check( st, "Front-End has no Attenuator controls");
}


void
FEC::setAttenuator(int channel, bool on)
{
int st;
	st = fecSetAtt( (*this)->scope(), channel, on ? max_ : min_ );
	check( st, "Front-End has no Attenuator controls");
}

bool
FEC::getAttenuator(int channel)
{
int    st;
double val;
	st = fecGetAtt( (*this)->scope(), channel, &val );
	check( st, "Front-End has no Attenuator controls" );
	return abs( max_ - val ) < abs( min_ - val );
}

void
FEC::setTermination(int channel, bool on)
{
int    st;
	st = fecSetTermination( (*this)->scope(), channel, on );
	check( st, "Front-End has no Termination controls" );
}

bool
FEC::getTermination(int channel)
{
int    st;
	st = fecGetTermination( (*this)->scope(), channel );
	check( st, "Front-End has no Termination controls" );
	return !!st;
}

void
FEC::setACMode(int channel, bool on)
{
int    st;
	st = fecSetACMode( (*this)->scope(), channel, on );
	check( st, "Front-End has no AC-coupling controller switch" );
}

bool
FEC::getACMode(int channel)
{
int    st;
	st = fecGetACMode( (*this)->scope(), channel );
	check( st, "Front-End has no AC-coupling controller switch" );
	return !!st;
}

void
FEC::setDACRangeHi(int channel, bool on)
{
int    st;
	st = fecSetDACRangeHi( (*this)->scope(), channel, on );
	check( st, "Front-End has no DAC range controls" );
}

bool
FEC::getDACRangeHi(int channel)
{
int    st;
	st = fecGetDACRangeHi( (*this)->scope(), channel );
	check( st, "Front-End has no DAC range controls" );
	return !!st;
}

void
FEC::getDBRange(double *min, double *max)
{
	if ( min ) {
		*min = min_;
	}

	if ( max ) {
		*max = max_;
	}
}
