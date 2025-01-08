#pragma once

#include <string>
#include <vector>
#include <hdf5Sup.h>

class H5Smpl {
	ScopeH5Data *h5d_ { nullptr };
public:
	// Data space covers indices 0..maxlen-1
    // The selected part stretches from off..actlen
	// actlen == 0 identidal with actlen == maxlen
	struct Dimension : ScopeDataDimension {
		Dimension()
		{
			maxlen = offset = actlen = 0;
		}
		Dimension &max(size_t arg)
		{
			maxlen = arg;
			return *this;
		}
		Dimension &off(size_t arg)
		{
			offset = arg;
			return *this;
		}
		Dimension &cnt(size_t arg)
		{
			actlen = arg;
			return *this;
		}
	};
	H5Smpl(const std::string &name, ScopeH5SampleType dsetType, ScopeH5SampleType memType, unsigned offset, unsigned precision, const std::vector<Dimension> &dims, const void *data);

	virtual void
	addAttributes(const std::string &name, const std::vector<unsigned> &vals);

	virtual void
	addAttributes(const std::string &name, const std::vector<int> &vals);

	virtual void
	addAttributes(const std::string &name, const std::vector<double> &vals);

	// Single string! (scalars can have per-channel arrays)
	virtual void
	addAttribute(const std::string &name, const std::string &val);

	virtual ~H5Smpl();
};
