# pragma once

namespace zx
{
	/// <summary>
	/// Value getter. 
	/// Property which refers to data
	/// corresponding to given address. 
	/// </summary>
	/// <typeparam name="Data">Type of referenced data. </typeparam>
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
