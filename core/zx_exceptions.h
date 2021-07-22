# pragma once
# include "zx_defines.h"

namespace zx
{
	/// <summary>
	/// Reason of error. 
	/// Designed to be string representation of zx::exception::what(). 
	/// Will be mapped to string literals. 
	/// May be useful to be error code convention for dependent ABI's. 
	/// </summary>
	enum class reason
	{
		unknown,
		no_factory_method,
		field_not_found,
		field_is_not_shrptr,
		nameleton_expected,
	};

	/// <summary>
	/// Exception class for this module. 
	/// </summary>
	class exception : public std::exception
	{
	public:
		ZX_API explicit exception();
		ZX_API explicit exception(reason reason);
		ZX_API explicit exception(const char* const message);
		ZX_API explicit exception(const std::string& message);
	};
}