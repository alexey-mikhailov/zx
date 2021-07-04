# pragma once

namespace zx
{
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
