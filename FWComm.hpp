#pragma once

#include <mutex>
#include <stdio.h>
#include <memory>
#include <fwComm.h>
#include <system_error>
#include <string>

class FWCommIOError : std::system_error {
public:
	FWCommIOError(const std::string &what)
	: std::system_error( EIO, std::generic_category(), what )
	{
	}
};

class FWComm;

typedef std::shared_ptr<FWComm> FWPtr;

class FWComm {
private:
	std::recursive_mutex  mtx_;
	unsigned              boardVersion_;
	unsigned              APIVersion_;
	unsigned              FWVersion_;

	FWComm(const FWComm &)    = delete;

	FWComm&
	operator=(const FWComm &) = delete;

	class Key {};

public:

	FWInfo     *const fw_;

	FWComm(Key k, const char *);
	~FWComm();

	void lock();
	void unlock();

	unsigned
	getBoardVersion()
	{
		return boardVersion_;
	}

	unsigned
	getAPIVersion()
	{
		return APIVersion_;
	}

	unsigned
	getFWVersion()
	{
		return FWVersion_;
	}

	FWInfo *
	operator*()
	{
		return fw_;
	}

	static FWPtr
	create(const char *nm)
	{
		return std::make_shared<FWComm>( Key(), nm );
	}

	static std::string
	what(int fwErr);
};

template <typename PT>
class LockedRef {
private:
	PT ptr_;

public:

	LockedRef(PT ptr)
	: ptr_(ptr)
	{
	}

	class Guard {
	private:
		LockedRef<PT> *r_;
	
		Guard(const Guard &)             = delete;
		Guard & operator=(const Guard &) = delete;

	public:

		Guard(LockedRef<PT> *r)
		: r_(r)
		{
			r_->ptr_->lock();
		}
	
		PT
		operator->()
		{
			return r_->ptr_;//.get();
		}

		~Guard()
		{
			r_->ptr_->unlock();
		}

		friend class LockedRef<PT>;
	};

	PT
	unlockedPtr()
	{
		return ptr_;
	}

	Guard
	operator*()
	{
		return Guard(this);
	}

	Guard
	operator->()
	{
		return Guard(this);
	}
};

typedef LockedRef<std::shared_ptr<FWComm>> FWRef;
