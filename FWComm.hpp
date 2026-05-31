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

#include <mutex>
#include <stdio.h>
#include <memory>
#include <fwComm.h>
#include <system_error>
#include <string>

class FWCommError : public std::system_error {
public:
	FWCommError(int errnoCode, const std::string &what)
	: std::system_error( errnoCode < 0 ? -errnoCode : errnoCode, std::generic_category(), what )
	{
	}
};

class FWCommIOError : public FWCommError {
public:
	FWCommIOError(const std::string &what)
	: FWCommError( EIO, what )
	{
	}
};

class FWComm;

typedef std::shared_ptr<FWComm> FWPtr;

class FWComm {
private:
	std::recursive_mutex  mtx_;
	unsigned              boardVersion_;
	unsigned              APIVersion_;
	unsigned              FWVersion_;

	FWComm(const FWComm &)    = delete;

	FWComm&
	operator=(const FWComm &) = delete;

	class Key {};

public:

	FWInfo     *const fw_;

	FWComm(Key k, const char *);
	~FWComm();

	void lock();
	void unlock();

	unsigned
	getBoardVersion()
	{
		return boardVersion_;
	}

	unsigned
	getAPIVersion()
	{
		return APIVersion_;
	}

	unsigned
	getFWVersion()
	{
		return FWVersion_;
	}

	FWInfo *
	operator*()
	{
		return fw_;
	}

	static FWPtr
	create(const char *nm)
	{
		return std::make_shared<FWComm>( Key(), nm );
	}

	static std::string
	what(int fwErr);
};

template <typename PT>
class LockedRef {
private:
	PT ptr_;

public:

	LockedRef(PT ptr)
	: ptr_(ptr)
	{
	}

	class Guard {
	private:
		LockedRef<PT> *r_;
	
		Guard(const Guard &)             = delete;
		Guard & operator=(const Guard &) = delete;

	public:

		Guard(LockedRef<PT> *r)
		: r_(r)
		{
			r_->ptr_->lock();
		}
	
		PT
		operator->()
		{
			return r_->ptr_;//.get();
		}

		~Guard()
		{
			r_->ptr_->unlock();
		}

		friend class LockedRef<PT>;
	};

	PT
	unlockedPtr()
	{
		return ptr_;
	}

	Guard
	operator*()
	{
		return Guard(this);
	}

	Guard
	operator->()
	{
		return Guard(this);
	}
};

typedef LockedRef<std::shared_ptr<FWComm>> FWRef;
