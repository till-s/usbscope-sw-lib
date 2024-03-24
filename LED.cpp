#include <LED.hpp>
#include <map>
#include <string>
#include <stdexcept>

using std::map;
using std::string;
using std::shared_ptr;
using std::make_shared;

typedef map<const string,unsigned> LedMap;

static void tf(FWInfo *p) { printf("TEST %p\n", p); }

class LEDNone : public LED, FWRef {
public:
	LEDNone(FWPtr fw)
    : FWRef( fw )
	{
	}
	virtual bool getVal(const std::string &name)
	{
		return false;
	}

	virtual void setVal(const std::string &name, bool val)
	{
	}

	virtual void test()
	{
		tf( (*this)->fw_  );
	}
};

class LEDv1 : public LED, public FWRef {
private:
	uint8_t               cache_[3];
	static const unsigned BASE = 0;
	static LedMap         map_;

public:
	LEDv1(FWPtr fw);

	virtual bool getVal(const std::string &name);
	virtual void setVal(const std::string &name, bool val);
	virtual void test()
	{
		tf( (*this)->fw_  );
	}

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
LEDv1::getVal(const std::string &name)
{
unsigned idx = map_.at( name );
unsigned adr = (idx >> 3);
unsigned msk = (1 << (idx & 0x7));
	return !!(this->cache_[adr] & msk);
}

void
LEDv1::setVal(const std::string &name, bool val)
{
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
		return std::make_shared<LEDNone>( fwp );
	}
}
