
#include <H5Smpl.hpp>
#include <stdexcept>

H5Smpl::H5Smpl(const std::string &name, ScopeH5SampleType dtype, const std::vector<Dimension> &dims, const void *data)
{
	unsigned bitShift = 0;
	if ( DOUBLE_T == dtype ) {
		bitShift = 32; // store double as float
	}
	// TODO set bit shift for INT16 based on sample size from the scope
	if ( ! (h5d_ = scope_h5_create_from_hslab( name.c_str(), dtype, bitShift, &dims[0], dims.size(), data )) ) {
		throw std::runtime_error("scope_h5_create_from_hslab() failed");
	}
}

void
H5Smpl::addAttributes(const std::string &name, const std::vector<unsigned> &vals)
{
	if ( scope_h5_add_uint_attrs( h5d_, name.c_str(), &vals[0], vals.size() ) < 0 ) {
		throw std::runtime_error("scope_h5_add_uint_attrs() failed");
	}
}

void
H5Smpl::addAttributes(const std::string &name, const std::vector<int> &vals)
{
	if ( scope_h5_add_int_attrs( h5d_, name.c_str(), &vals[0], vals.size() ) < 0 ) {
		throw std::runtime_error("scope_h5_add_int_attrs() failed");
	}
}

void
H5Smpl::addAttributes(const std::string &name, const std::vector<double> &vals)
{
	if ( scope_h5_add_double_attrs( h5d_, name.c_str(), &vals[0], vals.size() ) < 0 ) {
		throw std::runtime_error("scope_h5_add_double_attrs() failed");
	}
}


void
H5Smpl::addAttribute(const std::string &name, const std::string &val)
{
	if ( scope_h5_add_string_attr( h5d_, name.c_str(), val.c_str() ) < 0 ) {
		throw std::runtime_error("scope_h5_add_string_attr() failed");
	}
}

H5Smpl::~H5Smpl()
{
	scope_h5_close( h5d_ );
}

template <>
H5Smpl::H5Smpl(const std::string &name, const int16_t *data, const std::vector<Dimension> &dims)
 : H5Smpl( name, INT16_T, dims, data )
{
}

template <>
H5Smpl::H5Smpl(const std::string &name, const double *data, const std::vector<Dimension> &dims)
 : H5Smpl( name, DOUBLE_T, dims, data )
{
}
