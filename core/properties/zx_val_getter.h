# pragma once

namespace zx
{
	template <class Data>
	struct val_getter
	{
		val_getter(Data* data) : _data(data) {}

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

	private:
		// prevent copy
		val_getter(const val_getter &) {}
		val_getter& operator=(const val_getter&){}

		Data* _data;
	};
}
