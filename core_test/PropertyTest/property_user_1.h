# pragma once
# include "zx.h"

class property_user_1
{
	int							_order = 1234;
	float						_balance = 5.0f;
	double						_inflation_index = 1.0;

public:
	explicit property_user_1();
	virtual ~property_user_1();

	zx::val_getter<int> Order = &_order;
	zx::val_wrapper<property_user_1, float> Balance =
	{
		this, &_balance,

		[](property_user_1& user, const float& value)
		{
			user._balance = value;
		}
	};

	zx::val_wrapper<property_user_1, double> InflationIndex =
	{
		this, &_inflation_index,

		[](property_user_1& user, const double& value)
		{
			user._inflation_index = 2 * value;
		}
	};
};

