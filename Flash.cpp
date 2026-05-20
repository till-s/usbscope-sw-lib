#include <Flash.hpp>

#include <at25Sup.h>

Flash::Flash(FWPtr fwp)
: FWRef(fwp), flash_(nullptr) // lazy init
{
}

Flash::~Flash()
{
	if ( flash_ ) {
		FWRef::Guard g(this);
		at25_close( flash_ );
	}
}

AT25Flash *
Flash::flash()
{
	// lazy-init
	if ( ! flash_ ) {
		// caller must hold the FWRef
		flash_ = at25_open(unlockedPtr()->fw_, 0);
		if ( ! flash_ ) {
			throw std::runtime_error("at25_open failed");
		}
	}
	return flash_;
}

FlashWriterState::FlashWriterState(FlashWriterProgress *progress)
: index(0), progress(progress)
{
	// other fields initialized by progress callback
}

namespace {

FlashWriterProgress::Operation
flag2op(int flag) {
	switch ( flag ) {
		case AT25_CHECK_ERASED: return FlashWriterProgress::Operation::VERIFY_ERASED;
		case AT25_CHECK_VERIFY: return FlashWriterProgress::Operation::VERIFY_WRITTEN;
		case AT25_EXEC_PROG   : return FlashWriterProgress::Operation::WRITE;
		case AT25_ERASE       : return FlashWriterProgress::Operation::ERASE;
		default               : break;
	}
	throw std::logic_error("Flash::progressC: Unexpected flag/operation");
}

int
progressC(AT25Flash *flash, void *closure, int flag, unsigned addr, unsigned remain)
{
	FlashWriterState *state = static_cast<FlashWriterState*>(closure);
	if ( ! state->progress ) {
		return 0;
	}
	if ( 0 == state->index ) {
		state->address = addr;
		state->size    = remain;
	}
	state->completed                  = state->size - remain;
	FlashWriterProgress::Operation op = flag2op( flag );
	int rval = state->progress->advance(op, state);
	if ( 0 == remain ) {
		// reset index in case the state is reused (prog->verify)
		state->index = 0;
	} else {
		state->index++;
	}
	return rval;
}

}

ssize_t
Flash::read(unsigned addr, uint8_t *buf, size_t len)
{
	FWRef::Guard lg(this);
	return at25_spi_read( flash(), addr, buf, len );
}

ssize_t
Flash::erase(unsigned addr, size_t len, FlashWriterProgress *progress)
{
	FlashWriterState state( progress );
	// at25_area_erase may adjust/align addr and/or len;
	// handled by the first execution of the progress callback
	FWRef::Guard lg(this);
	return at25_area_erase( flash(), addr, len, progressC, &state );
}

ssize_t
Flash::write(unsigned addr, uint8_t *buf, size_t len, FlashWriterProgress *progress)
{
	FlashWriterState state( progress );
	FWRef::Guard lg(this);
	int flags = ( AT25_CHECK_ERASED | AT25_EXEC_PROG | AT25_CHECK_VERIFY );
	return at25_prog( flash(), addr, buf, len, flags, progressC, &state );
}

void
Flash::setWP(bool on)
{
	FWRef::Guard lg(this);
	if ( on ) {
		if ( at25_write_dis( flash() ) ) {
			throw std::runtime_error("Flash: set write disable failed");
		}
	} else {
		if ( at25_write_ena( flash() ) ) {
			throw std::runtime_error("Flash: set write enable failed");
		}
	}
}
