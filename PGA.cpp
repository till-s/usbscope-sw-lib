#include <PGA.hpp>
#include <AD8370PGA.hpp>
#include <Lmh6882PGA.hpp>

PGAPtr
PGA::create(FWPtr fw)
{
	PGAPtr rv;
	switch ( fw->getBoardVersion() ) {
		case 1:
			rv = std::make_shared<PGAAD8370> ( fw );
			break;
		default:
			rv = std::make_shared<PGALmh6882>( fw );
			break;
	}
	return rv;
}
