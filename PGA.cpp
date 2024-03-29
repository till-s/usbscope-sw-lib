#include <PGA.hpp>
#include <AD8370PGA.hpp>
#include <Lmh6882PGA.hpp>

PGAPtr
PGA::create(FWPtr fw)
{
	switch ( fw->getBoardVersion() ) {
		case 1:
			return std::make_shared<PGAAD8370> ( fw );
		default:
			return std::make_shared<PGALmh6882>( fw );
	}
}
