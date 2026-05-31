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

#include <I2CDev.hpp>

I2CDev::I2CDev(FWPtr fwp)
: FWRef( fwp )
{
}

uint8_t
I2CDev::readReg(unsigned sla, unsigned off)
{
	uint8_t sla8 = sla << 1;
	int st = bb_i2c_read_reg( (*this)->fw_, sla8, off );
	if ( st < 0 ) {
		throw FWCommIOError( "I2CDev::readReg()" );
	}
	return st;
}

void
I2CDev::writeReg(unsigned sla, unsigned off, uint8_t val)
{
	uint8_t sla8 = sla << 1;
	int st = bb_i2c_write_reg( (*this)->fw_, sla8, off, val );
	if ( st < 0 ) {
		throw FWCommIOError( "I2CDev::writeReg()" );
	}
}
