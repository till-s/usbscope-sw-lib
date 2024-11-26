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


