# pragma once
# include "../zx_defines.h"

namespace zx
{
	/// Reference readonly computer
	template <class Owner, class Data>
	class ref_formula
	{
		read_rawptr_delegate<Data, Owner> _read_func;
		Owner* _owner;

	public:
		ref_formula(Owner* owner,
					read_rawptr_delegate<Data, Owner> read_func) :
			_owner(owner)
		{
			_read_func = read_func;
		}

		Data& operator*() const
		{
			return _read_func(*_owner);
		}

		Data* operator->() const
		{
			return &_read_func(*_owner);
		}

		operator Data& () const
		{
			return _read_func(*_owner);
		}

		operator Data* () const
		{
			return &_read_func(*_owner);
		}

		Data* ptr() const
		{
			return &_read_func(*_owner);
		}

	private:
		// prevent copy
		ref_formula(const ref_formula&) {};
		ref_formula& operator=(const ref_formula&) {}
	};
}

