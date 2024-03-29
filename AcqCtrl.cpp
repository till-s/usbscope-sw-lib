#include <AcqCtrl.hpp>
#include <string>
#include <math.h>

using std::string;

AcqCtrl::AcqCtrl( FWPtr fwp )
: FWRef ( fwp ),
  bufsz_( buf_get_size( (*this)->fw_ )  ),
  buffl_( buf_get_flags( (*this)->fw_ ) )
{
}

static void
checkStat(int st, const char *where)
{
	if ( st ) {
		throw FWCommIOError( string( where ) + ": " + FWComm::what( st ) );
	}
}

void
AcqCtrl::xfer(AcqParams *set, AcqParams *get)
{
	int st = acq_set_params( (*this)->fw_, set, get );
	checkStat(st, __func__ );
}

double
AcqCtrl::getTriggerLevelPercent()
{
	AcqParams p;
	xfer( NULL, &p );
	return 100.0*(double)p.level/32767.0;
}

void
AcqCtrl::setTriggerLevelPercent(double lvl)
{
	AcqParams p;
	if ( lvl < -100.0 || lvl > 100.0 ) {
		throw std::invalid_argument( "AcqCtrl::setTriggerLevelPercent()" );
	}
	int16_t    l = round( 32767.0 * lvl/100.0 );
	int       st = acq_set_level( (*this)->fw_, l, 1000 );
	checkStat(st, __func__ );
}

unsigned
AcqCtrl::getNPreTriggerSamples()
{
	AcqParams p;
	xfer( NULL, &p );
	return p.npts;
}

void
AcqCtrl::setNPreTriggerSamples(unsigned npts)
{
	int st = acq_set_npts( (*this)->fw_, npts );
	checkStat( st, __func__ );
}

unsigned
AcqCtrl::getNSamples()
{
	AcqParams p;
	xfer( NULL, &p );
	return p.nsamples;
}

void
AcqCtrl::setNSamples(unsigned n)
{
	if ( n < 1 || n > bufsz_ ) {
		throw std::invalid_argument( string(__func__) );
	}
	int st = acq_set_nsamples( (*this)->fw_, n );
	checkStat( st, __func__ );
}

void
AcqCtrl::getDecimation(unsigned *cic0, unsigned *cic1)
{
	AcqParams p;
	xfer( NULL, &p );
	if ( cic0 ) *cic0 = p.cic0Decimation;
	if ( cic1 ) *cic1 = p.cic1Decimation;
}

void
AcqCtrl::setDecimation(unsigned n0,  unsigned n1)
{
	uint8_t  cic0Dec;
	uint32_t cic1Dec;
	if ( n1 != 0 ) {
		if ( n0 < 1 || n0 > 16 || n1 > (1<<12) ) {
			throw std::invalid_argument( string(__func__) + " decimation out of range" );
		}
		if ( n1 != 1 && 1 == n0 ) {
			throw std::invalid_argument( string(__func__) + " if CIC1 decimation > 1 then CIC0 decimation must be > 1, too " );
		}
		cic0Dec = n0;
		cic1Dec = n1;
	} else {
		if ( n0 < 1 || n0 > 16 * (1<<12) ) {
			throw std::invalid_argument( string(__func__) + " decimation out of range" );
		}
		if ( 1 == n0 ) {
			cic0Dec = 1;
			cic1Dec = 1;
		} else {
			for ( cic0Dec = 16; cic0Dec > 1; cic0Dec-- ) {
				if ( (n0 % cic0Dec) == 0 ) {
					cic1Dec = n0 / cic0Dec;
					break;
				}
			}
			if ( 1 == cic0Dec ) {
				throw std::invalid_argument( string(__func__) + " decimation must have a factor in 2..16" );
			}
		}
	}
	int st = acq_set_decimation( (*this)->fw_, cic0Dec, cic1Dec );
	checkStat( st, __func__ );
}

void
AcqCtrl::getTriggerSrc(TriggerSource *src, bool *edg)
{
	AcqParams p;
	xfer( NULL, &p );
	if ( src ) *src = p.src;
	if ( edg ) *edg = p.rising;
}

void
AcqCtrl::setTriggerSrc(TriggerSource  src, bool  rising)
{
	int st = acq_set_source( (*this)->fw_, src, rising );
	checkStat( st, __func__ );
}

int
AcqCtrl::getAutoTimeoutMS()
{
	AcqParams p;
	xfer( NULL, &p );
	if ( ACQ_PARAM_TIMEOUT_INF == p.autoTimeoutMS )
		return -1;
	return (int)p.autoTimeoutMS;
}

void
AcqCtrl::setAutoTimeoutMS(int timo)
{
	if ( timo < 0 ) {
		timo = ACQ_PARAM_TIMEOUT_INF;
	}
	int st = acq_set_autoTimeoutMs( (*this)->fw_, timo );
	checkStat( st, __func__ );
}

double
AcqCtrl::getScale()
{
	AcqParams p;
	xfer( NULL, &p );
	return ((double) p.scale) / ldexp(1.0,30);
}

void
AcqCtrl::setScale(double scl)
{
	int32_t iscl = round( scl * ldexp(1.0, 30) );
	int     st   = acq_set_scale( (*this)->fw_, 0, 0, iscl );
	checkStat( st, __func__ );
}

unsigned
AcqCtrl::getBufSampleSize()
{
	return (buffl_ & FW_BUF_FLG_16B) ? 2 : 1;
}

void
AcqCtrl::flushBuf()
{
	int st;
	if ( (st = buf_flush( (*this)->fw_ ) ) < 0 ) {
		checkStat( st, __func__ );
	}
}

void
AcqCtrl::readBuf(uint16_t *hdr, uint8_t *buf, size_t len)
{
	int st;
	if ( (st = buf_read( (*this)->fw_, hdr, buf, len ) ) < 0 ) {
		checkStat( st, __func__ );
	}
}
