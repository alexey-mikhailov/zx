# include "zx_type.h"

namespace zx
{
	std::map<std::type_index, type *> type::__instances;

	type type::null = type(type_data());

	bool type::operator==(const type& other) const
	{
		// Pointer comparison is ok
		return _data == other._data;
	}

	bool type::operator<(const type& other) const
	{
		// Pointer comparison is ok. 
		// Associative containers support. 
		return _data < other._data;
	}

	bool type::operator>(const type& other) const
	{
		// Pointer comparison is ok. 
		return _data > other._data;
	}

	type::type(type_data data) :
		_data(std::make_shared<type_data>(std::move(data)))
	{
	}
}
