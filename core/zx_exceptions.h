# pragma once
# include "zx_defines.h"

namespace zx
{
	enum struct reason
	{
		unknown,
		no_factory_method,
		field_not_found,
		field_is_not_shrptr,
	};

	class exception : public std::exception
	{
	public:
		ZX_API explicit exception();
		ZX_API explicit exception(reason reason);
		ZX_API explicit exception(const char* const message);
		ZX_API explicit exception(const std::string& message);
	};
}