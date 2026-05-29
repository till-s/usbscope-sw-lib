#include <ClockOut.hpp>
#include <cmath>


ClockOut::ClockOut(BoardInterface *brd)
: BoardRef( brd ),
  present_( ! std::isnan( scope_get_clock_out_min_freq( (*this)->scope() ) ) )
{
}

double
ClockOut::getMinFrequencyHz()
{
	double f = scope_get_clock_out_min_freq( (*this)->scope() );
	if ( std::isnan( f ) ) {
		throw FWCommError( -ENOTSUP, "ClockOut generator not supported on this board" );
	}
	return f;
}

double
ClockOut::getReferenceFrequencyHz()
{
	return scope_get_reference_freq( (*this)->scope() );
}

double
ClockOut::getMaxFrequencyHz()
{
	double f = scope_get_clock_out_max_freq( (*this)->scope() );
	if ( std::isnan( f ) ) {
		throw FWCommError( -ENOTSUP, "ClockOut generator not supported on this board" );
	}
	return f;
}

std::pair<double, bool>
ClockOut::getFrequencyHz()
{
	double f;
	int    isRef;
	int st = scope_get_clock_out_freq( (*this)->scope(), &f, &isRef );
	if ( st ) {
		throw FWCommError( st, "ClockOut generator reading frequency failed" );
	}

	return std::pair<double,bool>(f, !!isRef);
}

void
ClockOut::setFrequencyHz(double f)
{
	int st = scope_set_clock_out_freq( (*this)->scope(), f );
	if ( st ) {
		throw FWCommError( st, "ClockOut generator setting frequency failed" );
	}
}

void
ClockOut::setToReference()
{
	int st = scope_set_clock_out_to_ref( (*this)->scope() );
	if ( st ) {
		throw FWCommError( st, "ClockOut generator setting to reference failed" );
	}
}

