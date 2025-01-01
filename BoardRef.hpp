#pragma once

#include <FWComm.hpp>

struct ScopePvt;

class BoardInterface {
protected:
	ScopePvt *scope_;
	FWPtr     fwp_;
public:
	BoardInterface(FWPtr fwp, ScopePvt *scope)
	: scope_( scope ),
	  fwp_  ( fwp   )
	{
	}

	ScopePvt *
	scope()
	{
		return scope_;
	}

	FWPtr
	fwp()
	{
		return fwp_;
	}

	FWInfo *
	fw()
	{
		return fwp_->fw_;
	}


	void lock()
	{
		fwp_->lock();
	}

	void unlock()
	{
		fwp_->unlock();
	}
};

typedef LockedRef<BoardInterface *> BoardRef;
