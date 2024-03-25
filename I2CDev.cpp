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
