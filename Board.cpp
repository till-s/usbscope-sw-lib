#include <Board.hpp>
#include <VersaClk.hpp>
#include <FEC.hpp>
#include <PGAImpl.hpp>
#include <Max195xxADC.hpp>
#include <DAC47cx.hpp>

#include <scopeInit.h>
#include <string.h>

using std::make_shared;
using std::map;
using std::string;

class BoardADCClk : public FWRef, public ADCClk {
public:
	BoardADCClk( FWPtr fwp ) : FWRef( fwp ) {}

	virtual double getFreq() override
	{
		return buf_get_sampling_freq( (*this)->fw_ );
	}
};

ADCClkPtr
ADCClk::create( FWPtr fwp )
{
	return make_shared<BoardADCClk>( fwp );
}

FECPtr
FEC::create( FWPtr fwp )
{
	return make_shared<FEC>( fwp );
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
PGA::create(FWPtr fw)
{
	return std::make_shared<PGAImpl>( fw );
}

SlowDACPtr
SlowDAC::create( FWPtr fwp )
{
	return make_shared<DAC47cx>( fwp );
}

void
Board::hwInit(bool force)
{
	int st;
	if ( (st = scopeInit( (*this)->fw_, force )) < 0 ) {
		throw std::runtime_error( std::string("scopeInit failed: ") + strerror(-st));
	}
}

Board::Board( FWPtr fwp, bool sim )
: FWRef   ( fwp                             ),
  acq_    ( fwp                             ),
  adcClk_ ( ADCClk::create          ( fwp ) ),
  pga_    ( PGA::create             ( fwp ) ),
  leds_   ( LED::create             ( fwp ) ),
  fec_    ( FEC::create             ( fwp ) ),
  dac_    ( SlowDAC::create         ( fwp ) ),
  adc_    ( make_shared<Max195xxADC>( fwp ) ),
  sim_    ( sim                             )
{
	if ( ! simulation() ) {
		hwInit( false );
	}
	double   dfltScl   = 0.075;
	unsigned boardVers = (*this)->getBoardVersion();
	if        ( 1 == boardVers ) {
		dfltScl = 0.0098; // empirical
	} else if ( 2 == boardVers ) {
		dfltScl = 0.0102; // empirical
	}
	for ( int i = 0; i < NumChannels; i++ ) {
		vVoltScale_.push_back( dfltScl );
	}
}

void
Board::setVoltScale(int channel, double scl)
{
	if ( channel < 0 || channel >= NumChannels ) {
		throw std::invalid_argument( "channel # out of range " );
	}
	vVoltScale_[channel] = scl;
}

double
Board::getVoltScale(int channel)
{
	if ( channel < 0 || channel >= NumChannels ) {
		throw std::invalid_argument( "channel # out of range " );
	}
	return vVoltScale_[channel];
}
