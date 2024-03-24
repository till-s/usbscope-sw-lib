#pragma once

#include <FWComm.hpp>
#include <cstdint>

#include <max195xxSup.h>

class Max195xxADC : public FWRef {
public:
	Max195xxADC( FWPtr fwp );

	virtual void
	reset();

	virtual uint8_t
	readReg( unsigned off );

	virtual void
	writeReg( unsigned off, uint8_t val );

	virtual void
	setTestMode( Max195xxTestMode );

	virtual void
	setCMVolt( Max195xxCMVolt cmA, Max195xxCMVolt cmB );

	virtual bool
	dllLocked();

	virtual void
	setTiming( int dclkDelay, int dataDelay );

	virtual void
	setMuxedModeB();
};
