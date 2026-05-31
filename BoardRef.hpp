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

#include <FWComm.hpp>

struct ScopePvt;

class BoardInterface {
protected:
	ScopePvt *scope_;
	FWPtr     fwp_;
public:
	BoardInterface(FWPtr fwp, ScopePvt *scope)
	: scope_( scope ),
	  fwp_  ( fwp   )
	{
	}

	ScopePvt *
	scope()
	{
		return scope_;
	}

	FWPtr
	fwp()
	{
		return fwp_;
	}

	FWInfo *
	fw()
	{
		return fwp_->fw_;
	}


	void lock()
	{
		fwp_->lock();
	}

	void unlock()
	{
		fwp_->unlock();
	}
};

typedef LockedRef<BoardInterface *> BoardRef;
