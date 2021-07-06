# pragma once
# include "zx.h"

class test_signal_pack
{
public:
	explicit test_signal_pack() = default;
	virtual ~test_signal_pack() = default;

	zx::multicast_delegate<double(double, double)> something_happened;
};

