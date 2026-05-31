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

#pragma once

#include <system_error>
#include <string>

#include <BoardRef.hpp>

class FlashWriterState;

class FlashWriterProgress {
public:
	enum class Operation { ERASE, VERIFY_ERASED, WRITE, VERIFY_WRITTEN };

	virtual int advance(const FlashWriterState *state) = 0;
	virtual ~FlashWriterProgress() = default;

	static const std::string &toString(Operation);
};

struct FlashWriterState {
	FlashWriterProgress::Operation operation;
	// initially 0, increments after each block
	unsigned                       index;
	// starting address of area, remains constant
	unsigned                       address;
	// size of area
	unsigned                       size;
	// completed # bytes
	unsigned                       completed; 

	FlashWriterProgress *progress;

	FlashWriterState(FlashWriterProgress *, FlashWriterProgress::Operation);
};


class FlashError : public std::system_error {
public:
	FlashError()
	: system_error(0, std::generic_category())
	{
	}

	FlashError(int code, const std::string &msg)
	: system_error(code < 0 ? -code : code, std::generic_category(), msg)
	{
	}
};


struct AT25Flash;

class Flash : public FWRef {
	Flash(const Flash&)            = delete;
	Flash &operator=(const Flash&) = delete;

	// The AT25Flash (C) object holds a (non-shared) pointer
	// to FWInfo. But since the FWRef base does the shared pointer
	// management we are sure that the FWInfo is valid as long
	// as our base is alive.
	AT25Flash *flash_;

protected:
	virtual AT25Flash *flash();

public:
	Flash(FWPtr);

	virtual ssize_t read(unsigned addr, uint8_t *buf, size_t len);

	virtual ssize_t erase(unsigned addr, size_t len, FlashWriterProgress *progress = nullptr);
	virtual ssize_t write(unsigned addr, const uint8_t *buf, size_t len, FlashWriterProgress *progress = nullptr);

	virtual void setWP(bool);

	virtual ~Flash();

	class WriteEnable {
		WriteEnable(const WriteEnable &)            = delete;
		WriteEnable& operator=(const WriteEnable &) = delete;
		Flash *flash_;
	public:
		WriteEnable(Flash *flash);
		~WriteEnable();
	};
};
