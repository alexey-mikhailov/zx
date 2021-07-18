# pragma once
# include "zx.h"

class event_user
{
	zx::u64							_copyable_resource;
	zx::multicast_delegate<void()> _somethingComplete;
	zx::multicast_delegate<int()> _readyToCalc;
	zx::multicast_delegate<void(double, double, double)> _added;
	zx::multicast_delegate<const event_user&(const event_user&)> _identity_requested;

public:
	zx::event<void()> something_complete = _somethingComplete;
	zx::event<int()> readly_to_calc = _readyToCalc;
	zx::event<void(double, double, double)> added = _added;
	zx::event<const event_user&(const event_user&)> identity_requested = _identity_requested;

	explicit event_user();
	explicit event_user(const event_user& other);
	virtual ~event_user();

	zx::u64 get_copyable_resource() const { return _copyable_resource; }

	void do_something();
	int get_last_result();
	void add(double a, double b);
	std::tuple<const event_user&, size_t> request_identity();
};

