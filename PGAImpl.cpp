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

#include <PGAImpl.hpp>
#include <math.h>
#include <string>
#include <scopeSup.h>

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

PGAImpl::PGAImpl( BoardInterface *brd )
: BoardRef( brd )
{
}

void
PGAImpl::getDBRange(int *min, int *max)
{
	double attMin, attMax;
	int rv = pgaGetAttRangeDb( (*this)->scope(), &attMin, &attMax );

	checkRes( rv, "getDBRange()" );
	if ( min ) *min = round( attMin );
	if ( max ) *max = round( attMax );
}

void
PGAImpl::setDBAtt(unsigned channel, float att)
{
	int rv = pgaSetAttDb( (*this)->scope(), channel, att );
	checkRes( rv, "setDBAtt()" );
}

float
PGAImpl::getDBAtt(unsigned channel)
{
	double att;
	int   rv = pgaGetAttDb( (*this)->scope(), channel, &att );
	checkRes( rv, "getDBAtt()" );
	return att;
}
