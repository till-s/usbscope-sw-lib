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

#include <VersaClk.hpp>
#include <stdexcept>
#include <system_error>
#include <memory>
using std::shared_ptr;
using std::make_shared;
using std::map;
using std::string;

VersaClk::VersaClk( FWPtr fwp )
: FWRef( fwp )
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

VersaClkFODRoute
VersaClk::getFODRoute(int out)
{
	VersaClkFODRoute rte;
	int st = versaClkGetFODRoute( (*this)->fw_, out, &rte );
	checkStatus(st, "getFODRoute()" );
	return rte;
}
