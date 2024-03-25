#pragma once

#include <FWComm.hpp>
#include <cstdint>
#include <fwComm.h>

class I2CDev : public FWRef {
public:
	I2CDev( FWPtr fwp );

	virtual uint8_t
	readReg(unsigned sla, unsigned off);

	virtual void
	writeReg(unsigned sla, unsigned off, uint8_t val);
};
