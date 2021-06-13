# pragma once

namespace zx
{
	template <class Data>
	struct shr_getter
	{
		shr_getter(std::shared_ptr<Data> data) : _data(data) {}
		
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

	private:
		// prevent copy
		shr_getter(const shr_getter &){}
		shr_getter& operator=(const shr_getter&){}

		std::shared_ptr<Data> _data;
	};

}
