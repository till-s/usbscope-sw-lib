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
	printf("Lock\n");
	mtx_.lock();
}

void
FWComm::unlock()
{
	printf("Unlock\n");
	mtx_.unlock();
}

std::string
FWComm::what(int fwErr)
{
	switch ( fwErr ) {
		case FW_CMD_ERR_TIMEOUT:
			return "Timeout";
		case FW_CMD_ERR_NOTSUP:
			return "Unsupported command";
		case FW_CMD_ERR_INVALID:
			return "Invalid arguments";
		case FW_CMD_ERR: /* fall through */
		default:
			return "Unspecified error";
	}
}
