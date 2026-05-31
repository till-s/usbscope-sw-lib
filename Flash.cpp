/**LB-MIT
 *
 * MIT License
 *
 * Copyright (c) 2026 Till Straumann
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **LE-MIT*/

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
		int status = at25_open1(unlockedPtr()->fw_, &flash_, 0);
		if ( status ) {
			throw FlashError(status, "at25_open failed");
		}
	}
	return flash_;
}

FlashWriterState::FlashWriterState(FlashWriterProgress *progress, FlashWriterProgress::Operation operation)
: operation(operation), index(0), progress(progress)
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
		state->address   = addr;
		state->size      = remain;
	}
	FlashWriterProgress::Operation op = flag2op( flag );
	state->completed                  = state->size - remain;
	state->operation                  = op;
	int rval = state->progress->advance(state);
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
	ssize_t got = at25_spi_read( flash(), addr, buf, len );
	if ( got < 0 ) {
		throw FlashError(got, "at25_spi_read failed");
	}
	return got;
}

ssize_t
Flash::erase(unsigned addr, size_t len, FlashWriterProgress *progress)
{
	FlashWriterState state( progress, FlashWriterProgress::Operation::ERASE );
	// at25_area_erase may adjust/align addr and/or len;
	// handled by the first execution of the progress callback
	FWRef::Guard lg(this);
	ssize_t status = at25_area_erase( flash(), addr, len, progressC, &state );
	if ( status < 0 ) {
		throw FlashError(status, "at25_area_erase failed");
	}
	return status;
}

ssize_t
Flash::write(unsigned addr, const uint8_t *buf, size_t len, FlashWriterProgress *progress)
{
	FlashWriterState state( progress, FlashWriterProgress::Operation::VERIFY_ERASED );
	FWRef::Guard lg(this);
	int flags = ( AT25_CHECK_ERASED | AT25_EXEC_PROG | AT25_CHECK_VERIFY );
	ssize_t put = at25_prog( flash(), addr, buf, len, flags, progressC, &state );
	if ( put < 0 ) {
		throw FlashError(put, std::string("at25_prog failed during ") + FlashWriterProgress::toString( state.operation ) );
	}
	return put;
}

void
Flash::setWP(bool on)
{
	int status;
	FWRef::Guard lg(this);
	if ( on ) {
		if ( (status = at25_write_dis( flash() )) ) {
			throw FlashError(status, "at25_write_dis failed");
		}
	} else {
		if ( (status = at25_write_ena( flash() )) ) {
			throw FlashError(status, "at25_write_ena failed");
		}
	}
}

Flash::WriteEnable::WriteEnable(Flash *flash)
: flash_( flash )
{
	flash_->setWP( false );
}

Flash::WriteEnable::~WriteEnable()
{
	flash_->setWP( true );
}

const std::string &
FlashWriterProgress::toString(Operation op) {
	static std::string opErase        ( "ERASE"          );
	static std::string opVerifyErased ( "VERIFY_ERASED"  );
	static std::string opWrite        ( "WRITE"          );
	static std::string opVerifyWritten( "VERIFY_WRITTEN" );
	switch ( op ) {
		case Operation::ERASE          : return opErase;
		case Operation::VERIFY_ERASED  : return opVerifyErased;
		case Operation::WRITE          : return opWrite;
		case Operation::VERIFY_WRITTEN : return opVerifyWritten;
	}
	// avoid compiler warning about end of non-void function
	throw std::runtime_error("internal error -- unhandled case");
}
