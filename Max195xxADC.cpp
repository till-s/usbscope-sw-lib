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

#include <FWComm.hpp>
#include <Max195xxADC.hpp>

Max195xxADC::Max195xxADC( FWPtr fwp )
: FWRef( fwp )
{
}

void
Max195xxADC::reset()
{
	if ( max195xxReset( (*this)->fw_ ) < 0 ) {
		throw FWCommIOError( "Max195xxADC::reset()" );
	}
}

uint8_t
Max195xxADC::readReg( unsigned off )
{
	uint8_t val;
	if ( max195xxReadReg( (*this)->fw_, off, &val ) < 0 ) {
		throw FWCommIOError( "Max195xxADC::readReg()" );
	}
	return val;
}

void
Max195xxADC::writeReg( unsigned off, uint8_t val )
{
	if ( max195xxWriteReg( (*this)->fw_, off, val ) < 0 ) {
		throw FWCommIOError( "Max195xxADC::writeReg()" );
	}
}

void
Max195xxADC::setTestMode( Max195xxTestMode mod )
{
	if ( max195xxSetTestMode( (*this)->fw_, mod ) < 0 ) {
		throw FWCommIOError( "Max195xxADC::setTestMode()" );
	}
}

void
Max195xxADC::setCMVolt( Max195xxCMVolt cmA, Max195xxCMVolt cmB )
{
	if ( max195xxSetCMVolt( (*this)->fw_, cmA, cmB ) < 0 ) {
		throw FWCommIOError( "Max195xxADC::setCMVolt()" );
	}
}

bool
Max195xxADC::dllLocked()
{
	return max195xxDLLLocked( (*this)->fw_ ) == 0;
}

void
Max195xxADC::setTiming( int dclkDelay, int dataDelay )
{
	int st = max195xxSetTiming( (*this)->fw_, dclkDelay, dataDelay );
	if ( st < 0 ) {
		if ( -2 == st ) {
			throw std::invalid_argument( "Max195xxADC::setTiming(): delay values not in -3..+3");
		}
		throw FWCommIOError( "Max195xxADC::setTiming()" );
	}
}

void
Max195xxADC::setMuxedModeB()
{
	// muxed/DDR mode on port B
	writeReg( 1, 0x06 );
}

void
Max195xxADC::setMuxedModeA()
{
	// muxed/DDR mode on port A
	writeReg( 1, 0x02 );
}

bool
Max195xxADC::getClkTermination()
{
	int st;
	if ( (st = max195xxGetClkTermination( (*this)->fw_ )) < 0 ) {
		throw FWCommIOError( "Max195xxADC::getClkTermination()" );
	}
	return !!st;
}

void
Max195xxADC::enableClkTermination(bool on)
{
	int st;
	if ( (st = max195xxEnableClkTermination( (*this)->fw_, on )) < 0 ) {
		throw FWCommIOError( "Max195xxADC::enableClkTermination()" );
	}
}
