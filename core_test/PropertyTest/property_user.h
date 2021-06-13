# pragma once
# include "zx.h"

struct val_type
{
	int x = 0;
	int y = 0;

	val_type operator+(const val_type& other) const
	{
		return val_type{ x + other.x, y + other.y };
	}

	val_type operator-(const val_type& other) const
	{
		return val_type{ x - other.x, y - other.y };
	}
};

class ref_type
{
public:
	int a;
	ref_type(int a) : a(a) { }

	static ref_type null;
};

ref_type ref_type::null(0);

class property_user
{
	const int _ro_val1 = 8;
	const val_type _ro_val2;
	val_type _rw_val;

	ref_type* _rw_ref1 = new ref_type(123);
	ref_type* _rw_ref2;

	std::shared_ptr<ref_type> _rw_shr1 = std::make_shared<ref_type>(123);;
	std::shared_ptr<ref_type> _rw_shr2;

public:
	property_user(val_type ii) : _ro_val2(ii) {}

	zx::val_getter<const int> ro_val1 = &_ro_val1;
	zx::val_getter<const val_type> ro_val2 = &_ro_val2;

	zx::val_wrapper<property_user, val_type> rw_val =
	{
		this, &_rw_val,

		[](property_user & user, const val_type & value)
		{
			user._rw_val = value;

			// You can notify something here
		}
	};

	zx::val_computer <property_user, val_type> sum =
	{
		this,

		[](property_user & user)
		{
			return user._rw_val + user._ro_val2;
		},

		[](property_user & user, const val_type & value)
		{
			user._rw_val = value - user._ro_val2;
		}
	};

	zx::val_formula <property_user, val_type> diff =
	{
		this,

		[](property_user & user)
		{
			return user._rw_val - user._ro_val2;
		}
	};

	zx::ref_getter<ref_type> ro_ref1 = &_rw_ref1;

	zx::ref_wrapper<property_user, ref_type> rw_ref1 =
	{
		this, &_rw_ref1,

		[](property_user & user, ref_type * value)
		{
			user._rw_ref1 = value;

			// Add notification behaviour here
		}
	};

	zx::ref_formula<property_user, ref_type> ro_ref2
	{
		this,

		[](property_user & user) -> ref_type &
		{
			return user._rw_ref1 ? *user._rw_ref1 : ref_type::null;
		}
	};

	zx::ref_computer<property_user, ref_type> rw_ref2
	{
		this,
		[](property_user & user) -> ref_type &
		{
			return user._rw_ref2 ? *user._rw_ref2 : ref_type::null;
		},

		[](property_user & user, ref_type * value)
		{
			user._rw_ref2 = value;
		}
	};

	zx::shr_getter<ref_type> ro_shr1 = _rw_shr1;

	zx::shr_wrapper<property_user, ref_type> rw_shr1 =
	{
		this, _rw_shr1,

		[](property_user & user, std::shared_ptr<ref_type> value)
		{
			user._rw_shr1 = value;

			// Add notification behaviour here
		}
	};

	zx::shr_formula<property_user, ref_type> ro_shr2
	{
		this,

		[](property_user & user) -> std::shared_ptr<ref_type>
		{
			return user._rw_shr1;
		}
	};

	zx::shr_computer<property_user, ref_type> rw_shr2
	{
		this,

		[](property_user & user) -> std::shared_ptr<ref_type>
		{
			return user._rw_shr2;
		},

		[](property_user & user, std::shared_ptr<ref_type> value)
		{
			user._rw_shr2 = value;
		}
	};
};
