#include <TCA6408FEC.hpp>

using std::make_shared;

FECPtr
FEC::create( FWPtr fwp )
{
	FECPtr rv;
	switch ( fwp->getBoardVersion() ) {
		case 1:
			rv = make_shared<TCA6408FEC>( fwp );
			break;
		default:
			rv = make_shared<FEC>();
			break;
	}
	rv->init();
	return rv;
}
