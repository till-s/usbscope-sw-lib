#include <TCA6408FEC.hpp>

using std::make_shared;

FECPtr
FEC::create( FWPtr fwp )
{
	switch ( fwp->getBoardVersion() ) {
		case 1:
			return make_shared<TCA6408FEC>( fwp );
		default:
			return make_shared<FEC>();
	}
}
