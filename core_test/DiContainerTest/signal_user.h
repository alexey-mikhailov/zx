# pragma once
# include "test_signal_pack.h"

class signal_user
{
	static struct ctor { ctor(); } __ctor;
	std::shared_ptr<test_signal_pack> _signal_pack;

public:
	explicit signal_user();
	virtual ~signal_user() = default;

private:
	double on_something_happened(double a, double b);
};

