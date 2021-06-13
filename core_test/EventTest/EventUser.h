# pragma once
# include "../logger_stream.h"
# include "zx.h"
# include <CppUnitTestLogger.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

typedef void (*AddedDelegate)(double a, double b, double result);

struct SomeClass
{
	int Foo1(int a, int b) const
	{
		return a * b;
	}

	void Foo2(int a, int b)
	{
		ls << "...mul2 called with args " << a << " " << b << zx::endl;
	}

	int Foo3() const
	{
		return 42;
	}

	void Foo4()
	{
		ls << "...mul4 prints hello. " << zx::endl;
	}
};

class EventUser
{
	zx::multicast_delegate<void()> _somethingComplete;
	zx::multicast_delegate<int()> _readyToCalc;
	zx::multicast_delegate<void(double, double, double)> _added;

public:
	zx::event<void()> SomethingComplete = _somethingComplete;
	zx::event<int()> ReadyToCalc = _readyToCalc;
	zx::event<void(double, double, double)> Added = _added;

	explicit EventUser();
	virtual ~EventUser();

	void DoSomething();
	int GetLastResult();
	void Add(double a, double b);

};

