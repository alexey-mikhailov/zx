# include "zx_exceptions.h"

namespace zx
{
	constexpr char const* const get_message_from_reason(reason reason)
	{
		switch (reason)
		{
			case reason::no_factory_method:
			{
				return "No factory method during instantiate call. ";
			}
			case reason::field_not_found:
			{
				return "Field not found. ";
			}
			case reason::field_is_not_shrptr:
			{
				return "Field to inject is not shared pointer";
			}
			case reason::named_instance_expected:
			{
				return "Named instance expected. Bad downcast. ";
			}
			default:
			{
				return "Unspecified error in module zx. ";
			}
		}
	}

	exception::exception() :
		std::exception()
	{
	}

	exception::exception(reason reason) :
		std::exception(get_message_from_reason(reason))
	{
	}

	exception::exception(const char * const message) :
		std::exception(message)
	{
	}

	exception::exception(const std::string & message) :
		std::exception(message.c_str())
	{
	}
}