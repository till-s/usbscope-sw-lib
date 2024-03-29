#include <TCA6408FEC.hpp>

TCA6408FEC::TCA6408FEC(FWPtr fw)
: I2CFEC( fw, 0x20 )
{
}

void
TCA6408FEC::init()
{
	for (int ch = 0; ch < 2; ch++ ) {
		setTermination( ch, false );
		setAttenuator ( ch, true  );
		setACMode     ( ch, false );
		setDACRangeHi ( ch, true  );
	}
	setAllOutputs();
}

void
TCA6408FEC::setAllOutputs()
{
	writeReg( getSla(), 0x03, 0x00 );
}

int
TCA6408FEC::hasACModeCtl(int channel)
{
	switch ( channel ) {
		case 0 : return 7;
		case 1 : return 3;
		default:
			throw std::invalid_argument( "TCA6408FEC::hasACModeCtl() - invalid channel" );
	}
}

int
TCA6408FEC::hasAttenuatorCtl(int channel)
{
	switch ( channel ) {
		case 0 : return 6;
		case 1 : return 2;
		default:
			throw std::invalid_argument( "TCA6408FEC::hasAttenuatorCtl() - invalid channel" );
	}
}

int
TCA6408FEC::hasDACRangeCtl(int channel)
{
	switch ( channel ) {
		case 0 : return 1;
		case 1 : return 0;
		default:
			throw std::invalid_argument( "TCA6408FEC::hasDACRangeCtl() - invalid channel" );
	}
}

int
TCA6408FEC::hasTerminationCtl(int channel)
{
	switch ( channel ) {
		case 0 : return 5;
		case 1 : return 4;
		default:
			throw std::invalid_argument( "TCA6408FEC::hasTerminationCtl() - invalid channel" );
	}
}
