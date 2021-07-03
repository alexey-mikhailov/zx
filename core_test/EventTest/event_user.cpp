# include "event_user.h"


event_user::event_user()
{
	auto a = _somethingComplete;
}


event_user::~event_user()
{
}

void event_user::do_something()
{
	_somethingComplete();
}

int event_user::get_last_result()
{
	return _readyToCalc();
}

void event_user::add(double a, double b)
{
	const auto result = a + b;
	_added(a, b, result);
}
