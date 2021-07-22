# pragma once

namespace zx
{
	/// <summary>
	/// Raw reference getter. 
	/// Property which refers to data 
	/// corresponding to given address. 
	/// </summary>
	/// <typeparam name="Data">Type of referenced data. </typeparam>
	template <class Data>
	struct ref_getter
	{
		ref_getter(Data** data) : _data(data) {}
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
		
	private:
		// prevent copy
		ref_getter(const ref_getter &){}
		ref_getter& operator=(const ref_getter&){}

		Data** _data;
	};
}
