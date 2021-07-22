# pragma once

namespace zx
{
	/// <summary>
	/// Unique pointer getter.
	/// Property which refers to data
	/// corresponding to given address. 
	/// </summary>
	/// <typeparam name="Data">Type of referenced data. </typeparam>
	template <class Data>
	struct unq_getter
	{
		unq_getter(std::unique_ptr<Data>* data) : _data(data) {}

		Data& operator*() const
		{
			return *(*_data);
		}

		Data* operator->() const
		{
			return (*_data).get();
		}

		operator Data* () const
		{
			return (*_data).get();
		}

		operator Data& () const
		{
			return *(*_data);
		}

	private:
		unq_getter(const unq_getter&) {}
		unq_getter& operator=(const unq_getter&) {}

		std::unique_ptr<Data>* _data;
	};
}