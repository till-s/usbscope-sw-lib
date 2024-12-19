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
