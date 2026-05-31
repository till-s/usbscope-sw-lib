/**LB-MIT
 *
 * MIT License
 *
 * Copyright (c) 2026 Till Straumann
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **LE-MIT*/

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
	st = fecGetAttRangeDb( (*this)->scope(), &min_, &max_ );
	check( st, "Front-End has no Attenuator controls");
}


void
FEC::setAttenuator(int channel, bool on)
{
	setAttenuatorDb( channel, (on ? max_ : min_) );
}

void
FEC::setAttenuatorDb(int channel, double db)
{
	int st = fecSetAttDb( (*this)->scope(), channel, db );
	check( st, "Front-End has no Attenuator controls");
}

bool
FEC::isAttenuatorOn(int channel)
{
	double val = getAttenuatorDb( channel );
	return abs( max_ - val ) < abs( min_ - val );
}

double
FEC::getAttenuatorDb(int channel)
{
int    st;
double val;
	st = fecGetAttDb( (*this)->scope(), channel, &val );
	check( st, "Front-End has no Attenuator controls" );
	return val;
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
