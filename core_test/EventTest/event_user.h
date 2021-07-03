# pragma once
# include "zx.h"

class event_user
{
	zx::multicast_delegate<void()> _somethingComplete;
	zx::multicast_delegate<int()> _readyToCalc;
	zx::multicast_delegate<void(double, double, double)> _added;

public:
	zx::event<void()> something_complete = _somethingComplete;
	zx::event<int()> readly_to_calc = _readyToCalc;
	zx::event<void(double, double, double)> added = _added;

	explicit event_user();
	virtual ~event_user();

	void do_something();
	int get_last_result();
	void add(double a, double b);
};

