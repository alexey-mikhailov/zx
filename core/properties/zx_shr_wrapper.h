# pragma once
# include "../zx_defines.h"


namespace zx
{
	/// Shared ptr member specialization
	template <class Owner, class Data>
	class shr_wrapper
	{
		static inline write_shrptr_delegate<Data, Owner> _write_func;
		Owner* _owner;
		std::shared_ptr<Data> _data;

	public:
		shr_wrapper(Owner* owner,
					std::shared_ptr<Data> data,
					write_shrptr_delegate<Data, Owner> write_func) :
			_owner(owner),
			_data(data)
		{
			if (_write_func == nullptr)
			{
				_write_func = write_func;
			}
		}

		std::shared_ptr<Data> operator*() const
		{
			return _data;
		}

		Data* operator->() const
		{
			return _data.get();
		}

		operator std::shared_ptr<Data>()
		{
			return _data;
		}

		shr_wrapper& operator=(std::shared_ptr<Data> new_value)
		{
			_write_func(*_owner, new_value);
			return*this;
		}

	private:
		// prevent copy
		shr_wrapper(const shr_wrapper&){}
		shr_wrapper& operator=(const shr_wrapper&){}
	};
}