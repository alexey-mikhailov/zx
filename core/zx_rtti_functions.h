# pragma once
# include "zx_rtti_shared_ptr.h"

namespace zx
{
	namespace rtti
	{
		template <typename T, typename... Args>
		shared_ptr make_shared(Args... args)
		{
			auto value = std::make_shared<T>(args...);
			return zx::rtti::shared_ptr::from(value);
		}
	}
}