# pragma once
# include "../zx_defines.h"

namespace zx
{
	/// <summary>
	/// Shared pointer computer. 
	/// Property which reads and writes data 
	/// over std::function. 
	/// </summary>
	/// <typeparam name="Owner">Type of owner. </typeparam>
	/// <typeparam name="Data">Type of referenced data. </typeparam>
	template <class Owner, class Data>
	class shr_computer
	{
		read_shrptr_delegate<Data, Owner> _read_func;
		write_shrptr_delegate<Data, Owner> _write_func;
		Owner* _owner;

	public:
		shr_computer(Owner* owner,
						read_shrptr_delegate<Data, Owner> read_func,
						write_shrptr_delegate<Data, Owner> write_func) :
			_owner(owner)
		{
			_read_func = read_func;
			_write_func = write_func;
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

		shr_computer& operator =(std::shared_ptr<Data> new_value)
		{
			_write_func(*_owner, new_value);
			return *this;
		}

	private:
		// prevent copy
		shr_computer(const shr_computer &){}
		shr_computer& operator==(const shr_computer&){}
	};
}