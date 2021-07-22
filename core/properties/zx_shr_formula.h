# pragma once
# include "../zx_defines.h"

namespace zx
{
	/// <summary>
	/// Shared pointer formula. 
	/// Property which reads data over std::function. 
	/// </summary>
	/// <typeparam name="Owner">Type of owner. </typeparam>
	/// <typeparam name="Data">Type of referenced data. </typeparam>
	template <class Owner, class Data>
	class shr_formula
	{
		read_shrptr_delegate<Data, Owner> _read_func;
		Owner* _owner;

	public:
		shr_formula(Owner* owner,
						read_shrptr_delegate<Data, Owner> read_func) :
			_owner(owner)
		{
			_read_func = read_func;
		}

		std::shared_ptr<Data> operator*() const
		{
			return _read_func(*_owner);
		}

		Data* operator->() const
		{
			return _read_func(*_owner).get();
		}

		operator std::shared_ptr<Data>() const
		{
			return _read_func(*_owner);
		}

	private:
		// prevent copy
		shr_formula(const shr_formula&){}
		shr_formula& operator=(const shr_formula&) {}
	};
}

