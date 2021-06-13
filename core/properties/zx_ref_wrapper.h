# pragma once
# include "../zx_defines.h"

namespace zx
{
	/// Raw pointer member specialization
	template <class Owner, class Data>
	class ref_wrapper
	{
		static inline write_rawptr_delegate<Data, Owner> _write_func;
		Owner* _owner;
		Data** _data;

	public:
		ref_wrapper(Owner* owner,
					Data** data,
					write_rawptr_delegate<Data, Owner> write_func) :
			_owner(owner),
			_data(data)
		{
			if (_write_func == nullptr)
			{
				_write_func = write_func;
			}
		}

		Data& operator*() const
		{
			return **_data;
		}

		Data* operator->() const
		{
			return *_data;
		}

		operator Data&() const
		{
			return **_data;
		}

		operator Data*() const
		{
			return *_data;
		}

		Data* ptr() const
		{
			return *_data;
		}

		ref_wrapper& operator=(Data* new_value)
		{
			_write_func(*_owner, new_value);
			return* this;
		}

	private:
		// prevent copy
		ref_wrapper(const ref_wrapper &) {}
		ref_wrapper& operator-(const ref_wrapper&) {}
	};
}