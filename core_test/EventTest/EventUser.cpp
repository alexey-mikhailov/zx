# include "EventUser.h"


EventUser::EventUser()
{
	auto a = _somethingComplete;
}


EventUser::~EventUser()
{
}

void EventUser::DoSomething()
{
	_somethingComplete();
}

int EventUser::GetLastResult()
{
	return _readyToCalc();
}

void EventUser::Add(double a, double b)
{
	const auto result = a + b;
	_added(a, b, result);
}
