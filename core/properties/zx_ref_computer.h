# pragma once
# include "../zx_defines.h"


namespace zx
{
	/// Raw pointer member specialization
	template <class Owner, class Data>
	class ref_computer
	{
		read_rawptr_delegate<Data, Owner> _read_func;
		write_rawptr_delegate<Data, Owner> _write_func;
		Owner* _owner;

	public:
		ref_computer(Owner* owner,
						read_rawptr_delegate<Data, Owner> read_func,
						write_rawptr_delegate<Data, Owner> write_func) :
			_owner(owner)
		{
			_read_func = read_func;
			_write_func = write_func;
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

		ref_computer& operator =(Data* new_value)
		{
			_write_func(*_owner, new_value);
			return *this;
		}
	private:
		// prevent copy
		ref_computer(const ref_computer &) {}
		ref_computer& operator=(const ref_computer&){}
	};
}