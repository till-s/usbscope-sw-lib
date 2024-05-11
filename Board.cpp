#include <Board.hpp>
#include <VersaClk.hpp>
#include <TCA6408FEC.hpp>
#include <AD8370PGA.hpp>
#include <Lmh6882PGA.hpp>
#include <Max195xxADC.hpp>
#include <DAC47cx.hpp>

using std::make_shared;
using std::map;
using std::string;

class BrdClk : public VersaClk {
public:
	BrdClk( FWPtr fwp );

	virtual void init();
};

BrdClk::BrdClk( FWPtr fwp )
: VersaClk( fwp )
{
	if ( 0 == (*this)->getBoardVersion() ) {
		outMap_["EXT"]  = {1, OUT_CMOS, SLEW_100, LEVEL_18};
		outMap_["ADC"]  = {2, OUT_CMOS, SLEW_100, LEVEL_18};
		outMap_["FPGA"] = {4, OUT_CMOS, SLEW_100, LEVEL_18};
		fRef_           = 25.0e6;
		fADC_           = 130.0e6;
	} else {
		outMap_["EXT"]  = {2, OUT_CMOS, SLEW_100, LEVEL_18};
		outMap_["ADC"]  = {3, OUT_LVDS, SLEW_100, LEVEL_18};
		outMap_["FPGA"] = {1, OUT_CMOS, SLEW_100, LEVEL_18};
		fRef_           = 26.0e6;
		fADC_           = 120.0e6;
	}
}

void
BrdClk::init()
{
	auto it  = outMap_.cbegin();
	auto ite = outMap_.cend();
	while ( it != ite ) {
		int st = versaClkSetOutCfg( (*this)->fw_, it->second.output, it->second.iostd, it->second.slew, it->second.level );
		checkStatus( st, "setOutCfg()" );
		it++;
	}
	double fVCO   = fRef_ * getFBDiv();
	double outDiv = fVCO / fADC_ / 2.0;
	setOutDiv( outMap_["ADC"].output, outDiv );
	setOutDiv( outMap_["EXT"].output, 4095.0 );
	setFODRoute( outMap_["ADC"].output, NORMAL );
	setFODRoute( outMap_["EXT"].output, CASC_FOD );
}

ADCClkPtr
ADCClk::create( FWPtr fwp )
{
	return make_shared<BrdClk>( fwp );
}

FECPtr
FEC::create( FWPtr fwp )
{
	FECPtr rv;
	switch ( fwp->getBoardVersion() ) {
		case 1:
			{
			auto fec = make_shared<TCA6408FEC>( fwp );
			fec->setAllOutputs();
			rv = fec;
			}
			break;
		default:
			rv = make_shared<FEC>();
			break;
	}
	return rv;
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
	if ( 1 == fwp->getBoardVersion() ) {
		return make_shared<LEDv1>( fwp );
	} else {
		return std::make_shared<LEDNone>();
	}
}

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

void
Board::FECInit()
{
	// FEC may not be available or have partial support...
	for (int ch = 0; ch < NumChannels; ch++ ) {
		try { fec_->setTermination( ch, false ); }
			catch ( std::runtime_error &e ) {}
		try { fec_->setAttenuator ( ch, true  ); }
			catch ( std::runtime_error &e ) {}
		try { fec_->setACMode     ( ch, false ); }
			catch ( std::runtime_error &e ) {}
		try { fec_->setDACRangeHi ( ch, true  ); }
			catch ( std::runtime_error &e ) {}
	}
}

void
Board::DACInit()
{
	dac_->reset();
	std::static_pointer_cast<DAC47cx>( dac_ )->setRefInternalX1();
}

void
Board::CLKInit()
{
	std::static_pointer_cast<BrdClk>( adcClk_ )->init();
}

SlowDACPtr
SlowDAC::create( FWPtr fwp )
{
	return make_shared<DAC47cx>( fwp );
}

void
Board::ADCInit()
{
	struct timespec per;
	per.tv_sec  = 0;
	per.tv_nsec = 200*1000*1000;
	// sleep a little bit to let clock stabilize
	nanosleep( &per, NULL );
	adc_->reset();
	// sleep a little bit to let DLL lock
	nanosleep( &per, NULL );
	if ( ! adc_->dllLocked() ) {
		throw std::runtime_error("ADC DLL not locking -- no clock?");
	}
	adc_->setMuxedModeB();
	switch( (*this)->getBoardVersion() ) {
		case 0:
			// Empirically found setting for the prototype board
			adc_->setTiming( -1, 3 );
			// set common-mode voltage (also important for PGA output)
			//
			// ADC: common mode input voltage range 0.4..1.4V
			// ADC: controls common mode voltage of PGA
			// PGA: output common mode voltage: 2*OCM
			// Resistive divider 232/(232+178)
			//
			// PGA VOCM = 2*ADC_VCM
			//
			// Valid range for PGA: 2..3V (2.5V best)
			//
			// Common-mode register 8:
			//   bit 6..4, 2..0:
			//         000       -> 0.9 V
			//         001       -> 1.05V
			//         010       -> 1.2V
			//
			// With 1.2V -> VOCM of PGA becomes 2.4V   (near optimum)
			//           -> VICM of ADC becomes 1.358V (close to max)
			// With 1.05 -> VOCM of PGA becomes 2.1V   (close to min)
			//           -> VICM of ADC becomes 1.188V (OK)
			//
			adc_->setCMVolt( Max195xxCMVolt::CM_1050mV, Max195xxCMVolt::CM_1050mV );
			break;
		default:
		case 1:
			// on artix board with constraints the 'nominal' settings
			// seem much better
			adc_->setTiming( 0, 0 );
			break;
	}
}

void
Board::hwInit(bool force)
{
	if ( adc_->dllLocked() and not force ) {
		// assume board is already initialized
		return;
	}
	CLKInit();
	FECInit();
	DACInit();
	ADCInit();
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
	double dfltScl = 0.075;
	if ( 1 == (*this)->getBoardVersion() ) {
		dfltScl = 0.0075;
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
