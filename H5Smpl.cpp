
#include <H5Smpl.hpp>
#include <stdexcept>
#include <scopeSup.h>

H5Smpl::H5Smpl(const std::string &name, ScopeH5SampleType dsetType, unsigned offset, unsigned precision, const std::vector<Dimension> &dims)
 : ndims_( dims.size() )
{
	if ( ! (h5d_ = scope_h5_create_only( name.c_str(), dsetType, precision, offset, &dims[0], dims.size() )) ) {
		throw std::runtime_error("scope_h5_create_only() failed");
	}
}

void
H5Smpl::addHSlab(const std::vector<Dimension> *fileSelector, const H5DSpace *memSpace, const void *data)
{
	if ( fileSelector && (ndims_ != fileSelector->size()) ) {
		throw std::runtime_error("H5Smpl::addHSlab(): invalid # dimensions of selector");
	}
	if ( scope_h5_add_hslab( h5d_, fileSelector ? &(*fileSelector)[0] : nullptr, memSpace ? memSpace->get() : nullptr, data ) ) {
		throw std::runtime_error("H5Smpl::addHSlab(): scope_h5_add_hslab() failed");
	}
}

void
H5Smpl::addAttribute(const std::string &name, const std::vector<unsigned> &vals)
{
	if ( scope_h5_add_uint_attr( h5d_, name.c_str(), &vals[0], vals.size() ) < 0 ) {
		throw std::runtime_error("scope_h5_add_uint_attr() failed");
	}
}

void
H5Smpl::addAttribute(const std::string &name, const std::vector<int> &vals)
{
	if ( scope_h5_add_int_attr( h5d_, name.c_str(), &vals[0], vals.size() ) < 0 ) {
		throw std::runtime_error("scope_h5_add_int_attr() failed");
	}
}

void
H5Smpl::addAttribute(const std::string &name, const std::vector<double> &vals)
{
	if ( scope_h5_add_double_attr( h5d_, name.c_str(), &vals[0], vals.size() ) < 0 ) {
		throw std::runtime_error("scope_h5_add_double_attr() failed");
	}
}

void
H5Smpl::addAttribute(const std::string &name, unsigned val)
{
	if ( scope_h5_add_uint_attr( h5d_, name.c_str(), &val, 1 ) < 0 ) {
		throw std::runtime_error("scope_h5_add_uint_attr() failed");
	}
}

void
H5Smpl::addAttribute(const std::string &name, int val)
{
	if ( scope_h5_add_int_attr( h5d_, name.c_str(), &val, 1 ) < 0 ) {
		throw std::runtime_error("scope_h5_add_int_attr() failed");
	}
}

void
H5Smpl::addAttribute(const std::string &name, double val)
{
	if ( scope_h5_add_double_attr( h5d_, name.c_str(), &val, 1 ) < 0 ) {
		throw std::runtime_error("scope_h5_add_double_attr() failed");
	}
}



void
H5Smpl::addAttribute(const std::string &name, const std::string &val)
{
	if ( scope_h5_add_string_attr( h5d_, name.c_str(), val.c_str() ) < 0 ) {
		throw std::runtime_error("scope_h5_add_string_attr() failed");
	}
}

void
H5Smpl::addHdrInfo(unsigned bufHdr, unsigned numChannels)
{
	if ( scope_h5_add_bufhdr( h5d_, bufHdr, numChannels ) < 0 ) {
		throw std::runtime_error("scope_h5_add_bufhdr() failed");
	}
}

void
H5Smpl::addDate(time_t when)
{
	if ( scope_h5_add_date( h5d_, when ) < 0 ) {
		throw std::runtime_error("scope_h5_add_date() failed");
	}
}

void
H5Smpl::addTriggerSource(TriggerSource src, int rising)
{
	if ( scope_h5_add_trigger_source( h5d_, src, rising ) < 0 ) {
		throw std::runtime_error("scope_h5_add_trigger_source() failed");
	}
}

H5Smpl::~H5Smpl()
{
	scope_h5_close( h5d_ );
}

H5DSpace::H5DSpace(std::vector<H5Smpl::Dimension> dims, ScopeH5SampleType typ, unsigned offset, unsigned precision)
: h5s_( scope_h5_space_create( typ, offset, precision, &dims[0], dims.size() ) )
{
	if ( ! h5s_ ) {
		throw std::runtime_error("H5DSpace::H5DSpace creation failed");
	}
}

H5DSpace::~H5DSpace()
{
	scope_h5_space_destroy( h5s_ );
}

void
H5DSpace::setHSlabSelection(std::vector<H5Smpl::Dimension> sel)
{
	if ( scope_h5_space_select( h5s_, &sel[0], sel.size() ) ) {
		throw std::runtime_error("H5DSpace::setHSlabSelection() failed");
	}
}
