#pragma once

#include <string>
#include <vector>
#include <hdf5Sup.h>

class H5DSpace;

class H5Smpl {
	ScopeH5Data *h5d_ { nullptr };
	size_t       ndims_;
public:
	// Data space covers indices 0..maxlen-1
    // The selected part stretches from off..actlen.
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
	H5Smpl(const std::string &name, ScopeH5SampleType dsetType, unsigned offset, unsigned precision, const std::vector<Dimension> &dims);

	// fileSelector == nullptr => H5S_ALL
    // memSpace == nullptr => H5S_ALL, mem_type := dataset type

	virtual void
	addHSlab(const std::vector<Dimension> *fileSelector, const H5DSpace *memSpace, const void *data);

	virtual void
	addAttribute(const std::string &name, const std::vector<unsigned> &vals);

	virtual void
	addAttribute(const std::string &name, const std::vector<int> &vals);

	virtual void
	addAttribute(const std::string &name, const std::vector<double> &vals);

	virtual void
	addAttribute(const std::string &name, unsigned val);

	virtual void
	addAttribute(const std::string &name, int val);

	virtual void
	addAttribute(const std::string &name, double val);

	// Single string! (scalars can have per-channel arrays)
	virtual void
	addAttribute(const std::string &name, const std::string &val);

	virtual void
	addHdrInfo(unsigned bufHdr, unsigned numChannels);

	virtual void
	addTriggerSource(TriggerSource src, int rising);

	virtual void
	addDate(time_t when);

	virtual void
	addScopeParams(const ScopeParams *p);

	virtual void
	addComment(const std::string &);

	virtual ~H5Smpl();
};

class H5DSpace {
	ScopeH5DSpace *h5s_ { nullptr };
public:
	size_t getRank() const { return scope_h5_space_get_rank( h5s_ ); }

	H5DSpace(std::vector<H5Smpl::Dimension> dims, ScopeH5SampleType typ, unsigned offset = 0, unsigned precision = 0);

	ScopeH5DSpace *
	get() const
	{
		return h5s_;
	}

	void setHSlabSelection(std::vector<H5Smpl::Dimension> sel);

	~H5DSpace();
};
