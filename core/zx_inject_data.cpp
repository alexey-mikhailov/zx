#include "PCH.h"
#include "zx_inject_data.h"

namespace zx
{
	bool inject_data::operator==(const inject_data& other) const
	{
		return this == &other;
	}

	bool inject_data::operator!=(const inject_data& other) const
	{
		return this != &other;
	}

	named_instance::named_instance(std::string name) :
		_name(std::move(name))
	{
	}
}