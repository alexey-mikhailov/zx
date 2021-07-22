# pragma once
# include "zx.h"

// Magic number revelation :)
constexpr zx::i32 val_type_x = 10i32;
constexpr zx::i32 val_type_y = 15i32;
constexpr zx::i32 property_user_ro_i32 = 8i32;
constexpr zx::i32 property_user_ref_arg = 123i32;
constexpr zx::i32 property_user_unq_arg = 456i32;
constexpr zx::i32 property_user_shr_arg = 789i32;
constexpr zx::i32 property_user_valarg1 = 3i32;
constexpr zx::i32 property_user_valarg2 = 5i32;

struct val_type
{
	zx::i32 x = val_type_x;
	zx::i32 y = val_type_y;

	val_type operator+(const val_type& other) const
	{
		return val_type { x + other.x, y + other.y };
	}

	val_type operator-(const val_type& other) const
	{
		return val_type { x - other.x, y - other.y };
	}
};

class ref_type
{
public:
	zx::i32 a;
	ref_type(zx::i32 a) : a(a) { }

	static ref_type null;
};

ref_type ref_type::null(zx::zero::i32);

class property_user_2
{
	const zx::i32 _ro_i32 = property_user_ro_i32;
	const val_type _ro_val;
	val_type _rw_val;

	ref_type* _rw_ref1 = new ref_type(property_user_ref_arg);
	ref_type* _rw_ref2;

	std::unique_ptr<ref_type> _ro_unq = std::make_unique<ref_type>(property_user_unq_arg);

	std::shared_ptr<ref_type> _rw_shr1 = std::make_shared<ref_type>(property_user_shr_arg);
	std::shared_ptr<ref_type> _rw_shr2;

public:
	property_user_2() {}
	property_user_2(val_type ii) : _ro_val(ii) {}

	zx::val_getter<const int> ro_i32 = &_ro_i32;
	zx::val_getter<const val_type> ro_val = &_ro_val;

	zx::val_wrapper<property_user_2, val_type> rw_val =
	{
		this, &_rw_val,

		[](property_user_2& user, const val_type& value)
		{
			user._rw_val = value;

			// You can notify something here
		}
	};

	zx::val_computer <property_user_2, val_type> sum =
	{
		this,

		[](property_user_2& user)
		{
			return user._rw_val + user._ro_val;
		},

		[](property_user_2& user, const val_type& value)
		{
			user._rw_val = value - user._ro_val;
		}
	};

	zx::val_formula <property_user_2, val_type> diff =
	{
		this,

		[](property_user_2& user)
		{
			return user._rw_val - user._ro_val;
		}
	};

	zx::ref_getter<ref_type> ref_getter = &_rw_ref1;

	zx::ref_wrapper<property_user_2, ref_type> ref_wrapper =
	{
		this, &_rw_ref1,

		[](property_user_2& user, ref_type * value)
		{
			user._rw_ref1 = value;

			// Add notification behaviour here
		}
	};

	zx::ref_formula<property_user_2, ref_type> ref_formula
	{
		this,

		[](property_user_2& user) -> ref_type&
		{
			return user._rw_ref2 ? *user._rw_ref2 : ref_type::null;
		}
	};

	zx::ref_computer<property_user_2, ref_type> ref_computer
	{
		this,
		[](property_user_2 & user) -> ref_type&
		{
			return user._rw_ref2 ? *user._rw_ref2 : ref_type::null;
		},

		[](property_user_2& user, ref_type * value)
		{
			user._rw_ref2 = value;
		}
	};

	zx::unq_getter<ref_type> unq_getter = &_ro_unq;

	zx::shr_getter<ref_type> shr_getter = &_rw_shr1;

	zx::shr_wrapper<property_user_2, ref_type> shr_wrapper =
	{
		this, &_rw_shr1,

		[](property_user_2& user, std::shared_ptr<ref_type> value)
		{
			user._rw_shr1 = value;

			// Add notification behaviour here
		}
	};

	zx::shr_formula<property_user_2, ref_type> shr_formula
	{
		this,

		[](property_user_2& user) -> std::shared_ptr<ref_type>
		{
			return user._rw_shr1;
		}
	};

	zx::shr_computer<property_user_2, ref_type> shr_computer
	{
		this,

		[](property_user_2& user) -> std::shared_ptr<ref_type>
		{
			return user._rw_shr2;
		},

		[](property_user_2& user, std::shared_ptr<ref_type> value)
		{
			user._rw_shr2 = value;
		}
	};
};
