# pragma once

namespace zx
{
	/// <summary>
	/// Shared pointer getter. 
	/// Property which refers to data
	/// corresponding to given address. 
	/// </summary>
	/// <typeparam name="Data">Type of referenced data. </typeparam>
	template <class Data>
	struct shr_getter
	{
		shr_getter(std::shared_ptr<Data>* data) : _data(data) {}
		
		std::shared_ptr<Data> operator*() const
		{
			return *_data;
		}

		Data* operator->() const
		{
			return _data->get();
		}

		operator std::shared_ptr<Data>() const
		{
			return *_data;
		}

	private:
		// prevent copy
		shr_getter(const shr_getter&){}
		shr_getter& operator=(const shr_getter&){}

		std::shared_ptr<Data>* _data;
	};

}
