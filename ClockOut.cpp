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

#include <ClockOut.hpp>
#include <cmath>


ClockOut::ClockOut(BoardInterface *brd)
: BoardRef( brd ),
  present_( ! std::isnan( scope_get_clock_out_min_freq( (*this)->scope() ) ) )
{
}

double
ClockOut::getMinFrequencyHz()
{
	double f = scope_get_clock_out_min_freq( (*this)->scope() );
	if ( std::isnan( f ) ) {
		throw FWCommError( -ENOTSUP, "ClockOut generator not supported on this board" );
	}
	return f;
}

double
ClockOut::getReferenceFrequencyHz()
{
	return scope_get_reference_freq( (*this)->scope() );
}

double
ClockOut::getMaxFrequencyHz()
{
	double f = scope_get_clock_out_max_freq( (*this)->scope() );
	if ( std::isnan( f ) ) {
		throw FWCommError( -ENOTSUP, "ClockOut generator not supported on this board" );
	}
	return f;
}

std::pair<double, bool>
ClockOut::getFrequencyHz()
{
	double f;
	int    isRef;
	int st = scope_get_clock_out_freq( (*this)->scope(), &f, &isRef );
	if ( st ) {
		throw FWCommError( st, "ClockOut generator reading frequency failed" );
	}

	return std::pair<double,bool>(f, !!isRef);
}

void
ClockOut::setFrequencyHz(double f)
{
	int st = scope_set_clock_out_freq( (*this)->scope(), f );
	if ( st ) {
		throw FWCommError( st, "ClockOut generator setting frequency failed" );
	}
}

void
ClockOut::setToReference()
{
	int st = scope_set_clock_out_to_ref( (*this)->scope() );
	if ( st ) {
		throw FWCommError( st, "ClockOut generator setting to reference failed" );
	}
}

