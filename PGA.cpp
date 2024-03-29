#include <PGA.hpp>
#include <PGAAD8370.hpp>
#include <PGALmh6882.hpp>

std::shared_ptr<PGA>
PGA::create(FWPtr fw)
{
	switch ( fw->getBoardVersion() ) {
		case 1:
			return std::make_shared<PGAAD8370> ( fw );
		default:
			return std::make_shared<PGALmh6882>( fw );
	}
}
