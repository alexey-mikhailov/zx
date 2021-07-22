# pragma once
# include "../zx_defines.h"

namespace zx
{
	/// <summary>
	/// Value formula.
	/// Property which reads data over std::function. 
	/// </summary>
	/// <typeparam name="Owner">Type of owner. </typeparam>
	/// <typeparam name="Data">Type of referenced data. </typeparam>
	template <class Owner, class Data>
	class val_formula
	{
		read_value_delegate<Data, Owner> _read_func;
		Owner* _owner;
		Data _cache;

	public:
		val_formula(Owner* owner,
					read_value_delegate<Data, Owner> read_func) :
			_owner(owner)
		{
			_read_func = read_func;
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

	private:
		// prevent copy
		val_formula(const val_formula &){}
		val_formula& operator=(const val_formula&) {}
	};
}