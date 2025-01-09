
#include <H5Smpl.hpp>
#include <stdexcept>

H5Smpl::H5Smpl(const std::string &name, ScopeH5SampleType dsetType, ScopeH5SampleType memType, unsigned offset, unsigned precision, const std::vector<Dimension> &dims, const void *data)
{
	if ( ! (h5d_ = scope_h5_create_from_hslab( name.c_str(), dsetType, precision, offset, memType, &dims[0], dims.size(), data )) ) {
		throw std::runtime_error("scope_h5_create_from_hslab() failed");
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

H5Smpl::~H5Smpl()
{
	scope_h5_close( h5d_ );
}
