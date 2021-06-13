# pragma once
# include "../zx_defines.h"

namespace zx
{
	/// Readwrite value computer
	template <class Owner, class Data>
	class val_computer
	{
		read_value_delegate<Data, Owner> _read_func;
		write_value_delegate<Data, Owner> _write_func;
		Owner* _owner;
		Data _cache;

	public:
		val_computer(Owner* owner, 
					 read_value_delegate<Data, Owner> read_func, 
					 write_value_delegate<Data, Owner> write_func) : 
			_owner(owner)
		{
			_read_func = read_func;
			_write_func = write_func;
		}

		Data operator*() const
		{
			return _read_func(*_owner);
		}

		Data* operator->()
		{
			_cache = _read_func(*_owner);
			return &_cache;
		}

		operator Data() const
		{
			return _read_func(*_owner);
		}

		val_computer& operator =(const Data& new_value)
		{
			_write_func(*_owner, new_value);
			return *this;
		}

	private:
		// prevent copy
		val_computer(const val_computer &){}
		val_computer& operator=(const val_computer&){}
	};
}
