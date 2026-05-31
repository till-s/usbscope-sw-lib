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

#include <ADCClk.hpp>
#include <versaClkSup.h>

#include <map>
#include <string>

class VersaClk : public FWRef {
	typedef struct OutCfg {
		int              output;
		VersaClkOutMode  iostd;
		VersaClkOutSlew  slew;
		VersaClkOutLevel level;
	} OutCfg;

protected:

	std::map<std::string, OutCfg> outMap_;

	static void
	checkStatus(int st, const char *where);

public:
	VersaClk( FWPtr fwp );

	virtual double
	getFBDiv();

	virtual void
	setOutDiv(int out, double div);

	virtual double
	getOutDiv(int out);

	virtual void
	setFODRoute(int out, VersaClkFODRoute rte);

	virtual VersaClkFODRoute
	getFODRoute(int out);

	virtual int
	mapOut(const char *name)
	{
		return outMap_.at( name ).output;
	}

};
