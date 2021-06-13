# pragma once
# include "zx.h"

class PropertyUser
{
	int							_order = 1234;
	float						_balance = 5.0f;
	double						_inflation_index = 1.0;

public:
	explicit PropertyUser();
	virtual ~PropertyUser();

	zx::val_getter<int> Order = &_order;
	zx::val_wrapper<PropertyUser, float> Balance =
	{
		this, &_balance,

		[](PropertyUser& user, const float& value)
		{
			user._balance = value;
		}
	};

	zx::val_wrapper<PropertyUser, double> InflationIndex =
	{
		this, &_inflation_index,

		[](PropertyUser& user, const double& value)
		{
			user._inflation_index = 2 * value;
		}
	};
};

