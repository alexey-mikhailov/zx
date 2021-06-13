# pragma once
# include "../zx_defines.h"

namespace zx
{
	template <class Owner, class Data>
	class val_wrapper
	{
		static inline write_value_delegate<Data, Owner> _write_func;
		Owner* _owner;
		Data* _data;

	public:
		val_wrapper(Owner* owner,
					Data* data,
					write_value_delegate<Data, Owner> write_func) :
			_data(data),
			_owner(owner)
		{
			if (_write_func == nullptr)
			{
				_write_func = write_func;
			}
		}

		Data operator*() const
		{
			return *_data;
		}

		Data* operator->() const
		{
			return _data;
		}

		operator Data() const
		{
			return *_data;
		}

		val_wrapper& operator =(const Data& new_value)
		{
			_write_func(*_owner, new_value);
			return *this;
		}

	private:
		// prevent copy
		val_wrapper(const val_wrapper &) {}
		val_wrapper& operator=(const val_wrapper&) {}
	};
}
