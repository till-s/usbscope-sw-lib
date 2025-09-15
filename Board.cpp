#include <Board.hpp>
#include <VersaClk.hpp>
#include <FEC.hpp>
#include <PGAImpl.hpp>
#include <Max195xxADC.hpp>
#include <DAC47cx.hpp>
#include <scopeSup.h>
#include <string.h>

using std::make_shared;
using std::shared_ptr;
using std::map;
using std::string;

class BoardADCClk : public BoardRef, public ADCClk {
public:
	BoardADCClk( BoardInterface *brd ) : BoardRef( brd ) {}

	virtual double getFreq() override
	{
		return buf_get_sampling_freq( (*this)->scope() );
	}
};

class BoardDAC : public BoardRef, public virtual SlowDAC  {
public:
	BoardDAC( BoardInterface *brd )
	: BoardRef( brd )
	{
	}

	// DAC interface
	virtual void
	resetDAC() override;

	virtual void
	getVoltsRange( double *vmin, double *vmax ) override;

	virtual void
	setVolts(int channel, double volt) override;

	virtual double
	getVolts(int channel) override;

	virtual bool
	getRangeHigh(int channel) override;

	virtual void
	setRangeHigh(int channel, bool on) override;
};

ADCClkPtr
ADCClk::create( BoardInterface *brd )
{
	return make_shared<BoardADCClk>( brd );
}

FECPtr
FEC::create( BoardInterface *brd )
{
	try {
		return make_shared<FEC>( brd );
	} catch ( std::runtime_error &e ) {
		return shared_ptr<FEC>();
	}
}

typedef map<const string,unsigned> LedMap;

class LEDv1 : public LED, public FWRef {
private:
	uint8_t               cache_[3];
	static const unsigned BASE = 0;
	static LedMap         map_;

public:
	LEDv1(FWPtr fw);

	virtual bool getVal(const string &name) override;
	virtual void setVal(const string &name, bool val) override;
};

LEDv1::LEDv1(FWPtr fw)
: FWRef( fw )
{
	int st = fw_reg_read( (*this)->fw_, BASE, cache_, sizeof(cache_), 0 );
	if ( st < 0 ) {
		throw std::runtime_error( string("LEDv1::LEDv1() error reading registers: ") + FWComm::what( st ) );
	}
}

bool
LEDv1::getVal(const string &name)
{
unsigned idx = map_.at( name );
unsigned adr = (idx >> 3);
unsigned msk = (1 << (idx & 0x7));
	return !!(this->cache_[adr] & msk);
}

void
LEDv1::setVal(const string &name, bool val)
{
FWRef::Guard g( this );

unsigned idx = map_.at( name );
unsigned adr = (idx >> 3);
unsigned msk = (1 << (idx & 0x7));
uint8_t  buf;

	buf = this->cache_[adr];
	if ( val ) {
		buf |= msk;
	} else {
		buf &= ~msk;
	}

	int st = fw_reg_write( (*this)->fw_, BASE + adr, &buf, 1, 0 );
	if ( st < 0 ) {
		throw std::runtime_error( string("LEDv1::LEDv1() error writing registers: ") + FWComm::what( st ) );
	}

	this->cache_[adr] = buf;
}


LedMap
LEDv1::map_({
    { "FrontRight_R",  8 },
    { "FrontRight_G",  9 },
    { "FrontRight_B", 10 },

    { "CHA_R"       ,  4 },
    { "CHA_G"       ,  5 },
    { "CHA_B"       ,  6 },

    { "CHB_R"       ,  0 },
    { "CHB_G"       ,  1 },
    { "CHB_B"       ,  2 },

    { "FrontMid_R"  , 12 },
    { "FrontMid_G"  , 13 },
    { "FrontMid_B"  , 14 },

    { "FrontLeft"   , 15 },

    { "Trig"        , 16 },
    { "TermA"       ,  6 },
    { "TermB"       ,  2 },

    { "OVRA"        ,  4 },
    { "OVRB"        ,  0 },
});

LEDPtr
LED::create(FWPtr fwp)
{
	unsigned boardVersion = fwp->getBoardVersion();
	if ( 1 == boardVersion || 2 == boardVersion ) {
		return make_shared<LEDv1>( fwp );
	} else {
		return std::make_shared<LEDNone>();
	}
}

PGAPtr
PGA::create(BoardInterface *brd)
{
	return std::make_shared<PGAImpl>( brd );
}

void
Board::hwInit(bool force)
{
}

unsigned
Board::getNumChannels()
{
	return scope_get_num_channels( (*this)->scope() );
}

Board::Board( FWPtr fwp, bool sim )
// Create a temporary FWRef to lock; Board is not constructed yet!
: BoardInterface      ( fwp, scope_open( (*FWRef(fwp))->fw_ ) ),
  BoardRef            ( this                                  ),
  acq_                ( this                                  ),
  adcClk_             ( ADCClk::create          ( this )      ),
  pga_                ( PGA::create             ( this )      ),
  leds_               ( LED::create             ( fwp  )      ),
  fec_                ( FEC::create             ( this )      ),
  dac_                ( make_shared<BoardDAC>   ( this )      ),
  adc_                ( make_shared<Max195xxADC>( fwp  )      ),
  sim_                ( sim                                   )
{
	if ( ! simulation() ) {
		hwInit( false );
	}
}

void
Board::setVoltScale(int channel, double voltScale)
{
int st;
	st = scope_set_full_scale_volts( (*this)->scope(), channel, voltScale );
	if ( st < 0 ) {
		throw std::runtime_error( string("Board::setVoltScale() - Error: ") + strerror( -st ) );
	}
}

double
Board::getVoltScale(int channel)
{
int    st;
double val;
	st = scope_get_full_scale_volts( (*this)->scope(), channel, &val );
	if ( st < 0 ) {
		throw std::runtime_error( string("Board::getVoltScale() - Error: ") + strerror( -st ) );
	}
	return val;
}


unsigned
Board::getSampleSize()
{
int st = buf_get_sample_size( (*this)->scope() );
	if ( st < 0 ) {
		throw std::runtime_error( string("Board::getSampleSize() - Error: ") + strerror( -st ) );
	}
	return st;
}

int
Board::getFullScaleTicks()
{
	return buf_get_full_scale_ticks( (*this)->scope() );
}

struct ScopeParamsDel {
	void operator()(ScopeParams *p) {
		scope_free_params(p);
	}
};

std::shared_ptr<ScopeParams>
Board::makeScopeParams(std::shared_ptr<ScopeParams> old) {
	auto p = scope_alloc_params( (*this)->scope() );
	if ( ! p ) {
		throw std::runtime_error("Board::makeScopeParam() -- no memory");
	}
	auto rval = std::shared_ptr<ScopeParams>( p, ScopeParamsDel() );

	if ( old ) {
		*rval = *old;
	}

	return rval;
}

// DAC interface
void
BoardDAC::resetDAC()
{
	throw std::runtime_error("Board::resetDAC(): Not supported");
}

void
BoardDAC::getVoltsRange( double *vmin, double *vmax )
{
	if ( dacGetVoltsRange( (*this)->scope(), vmin, vmax ) < 0 ) {
		throw std::runtime_error("Board::getVoltsRange: dacGetVoltsRange() failed");
	}
}

void
BoardDAC::setVolts(int channel, double volt)
{
int st;
	if ( (st = dacSetVolts( (*this)->scope(), channel, volt )) < 0 ) {
		throw std::runtime_error(string("Board::setVolts: Error - ") + strerror(-st));
	}
}

double
BoardDAC::getVolts(int channel)
{
int st;
double volt;
	if ( (st = dacGetVolts( (*this)->scope(), channel, &volt )) < 0 ) {
		throw std::runtime_error(string("Board::getVolts: Error - ") + strerror(-st));
	}
	return volt;
}

bool
BoardDAC::getRangeHigh(int channel)
{
	throw std::runtime_error(string("Board::getRangeHigh() - board does not support DAC ranges"));
}

void
BoardDAC::setRangeHigh(int channel, bool val)
{
	throw std::runtime_error(string("Board::getRangeHigh() - board does not support DAC ranges"));
}
