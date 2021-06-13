# pragma once
# include "TestSignalPack.h"

class SignalUser
{
	static struct Ctor { Ctor(); } __ctor;
	TestSignalPack* _signalPack;

public:
	explicit SignalUser();
	virtual ~SignalUser() = default;

private:
	double OnSomethingHappened(double a, double b);
};

