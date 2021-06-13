# pragma once
# include "zx.h"

class TestSignalPack
{
public:
	explicit TestSignalPack() = default;
	virtual ~TestSignalPack() = default;

	zx::multicast_delegate<double(double, double)> SomethingHappened;
};

