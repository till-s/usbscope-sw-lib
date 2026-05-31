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

#include <FWComm.hpp>
#include <stdexcept>
#include <string>
#include <string.h>
#include <errno.h>

using std::string;

FWComm::FWComm(Key k, const char *nm)
  :  fw_ ( fw_open( nm, 115200 ) )
{
	if ( ! fw_ ) {
		throw std::runtime_error( std::string( "Unable to connect to firmware: " ) + strerror( errno ) );
	}
	boardVersion_ = fw_get_board_version( this->fw_ );
	APIVersion_   = fw_get_api_version( this->fw_ );
	FWVersion_    = fw_get_version( this->fw_ );
}

FWComm::~FWComm()
{
	fw_close( fw_ );
}

void
FWComm::lock()
{
	mtx_.lock();
}

void
FWComm::unlock()
{
	mtx_.unlock();
}

std::string
FWComm::what(int fwErr)
{
	return strerror( - fwErr );
}
