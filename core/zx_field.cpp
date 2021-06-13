# include "zx_field.h"

namespace zx
{
	field& field::operator=(const field& other)
	{
		_data = other._data;
		return *this;
	}

	field& field::operator=(const field&& other)
	{
		if (&other != this)
		{
			delete _data;
			_data = other._data;
		}

		return *this;
	}
}
