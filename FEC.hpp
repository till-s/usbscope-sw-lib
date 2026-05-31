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

#include <stdexcept>
#include <memory>
#include <FWComm.hpp>
#include <BoardRef.hpp>

class FEC;
class Board;

typedef std::shared_ptr<FEC> FECPtr;

class FEC : public BoardRef {

	double min_, max_;

	FEC( const FEC & )       = delete;

	FEC &
	operator=( const FEC & ) = delete;
public:
	FEC( BoardInterface * );

	virtual void getDBRange( double *min, double *max );

	virtual void setAttenuator(int channel, bool on);
	virtual void setAttenuatorDb(int channel, double db);

	virtual double getAttenuatorDb(int channel);
	virtual bool isAttenuatorOn(int channel);

	virtual void setTermination(int channel, bool on);

	virtual bool getTermination(int channel);

	virtual void setACMode(int channel, bool on);

	virtual bool getACMode(int channel);

	virtual void setDACRangeHi(int channel, bool on);

	virtual bool getDACRangeHi(int channel);

	virtual ~FEC()
	{
	}

	static FECPtr
	create( BoardInterface * );
};
