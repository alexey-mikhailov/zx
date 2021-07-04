# pragma once
# include "zx.h"

constexpr zx::i32 property_user_order = 1234;
constexpr float   property_user_balance = 5.0f;
constexpr double  property_user_inflation_index = 1.0;

class property_user_1
{
	zx::i32						_order;
	float						_balance;
	double						_inflation_index;

public:
	explicit property_user_1();
	virtual ~property_user_1();

	zx::val_getter<int> order = &_order;
	zx::val_wrapper<property_user_1, float> balance =
	{
		this, &_balance,

		[](property_user_1& user, const float& value)
		{
			user._balance = value;
		}
	};

	zx::val_wrapper<property_user_1, double> inflation_index =
	{
		this, &_inflation_index,

		[](property_user_1& user, const double& value)
		{
			user._inflation_index = 2.0 * value;
		}
	};
};

