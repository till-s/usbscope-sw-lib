#include <I2CFEC.hpp>

I2CFEC::I2CFEC( FWPtr fwp, unsigned sla )
: I2CDev( fwp ), sla_ ( sla )
{
}

bool
I2CFEC::getBit(int channel, int bit)
{
	FWRef::Guard( this );
	return !!(readReg( sla_, getOutReg( channel ) ) & (1<<bit));
}

void
I2CFEC::setBit(int channel, int bit, int val)
{
	FWRef::Guard( this );

	unsigned reg = getOutReg( channel );

	uint8_t ctl = readReg( sla_, reg );
	if ( val ) {
		ctl |=  (1 << bit);
	} else {
		ctl &= ~(1 << bit);
	}
	writeReg( sla_, reg, ctl );
}

void
I2CFEC::setAttenuator(int channel, bool on)
{
	setBit( channel, hasAttenuatorCtl( channel ), on );
}

bool
I2CFEC::getAttenuator(int channel)
{
	return getBit( channel, hasAttenuatorCtl( channel ) );
}

void
I2CFEC::setTermination(int channel, bool on)
{
	setBit( channel, hasTerminationCtl( channel ), on );
}

bool
I2CFEC::getTermination(int channel)
{
	return getBit( channel, hasTerminationCtl( channel ) );
}

void
I2CFEC::setACMode(int channel, bool on)
{
	setBit( channel, hasACModeCtl( channel ), on );
}

bool
I2CFEC::getACMode(int channel)
{
	return getBit( channel, hasACModeCtl( channel ) );
}

void
I2CFEC::setDACRangeHi(int channel, bool on)
{
	setBit( channel, hasDACRangeCtl( channel ), ! on );
}

bool
I2CFEC::getDACRangeHi(int channel)
{
	return ! getBit( channel, hasDACRangeCtl( channel ) );
}

