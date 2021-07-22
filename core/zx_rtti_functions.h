# pragma once
# include "zx_rtti_shared_ptr.h"

namespace zx
{
	namespace rtti
	{
		/// <summary>
		/// Makes std::shared_ptr from zx::rtti::shared_ptr. 
		/// </summary>
		/// <typeparam name="T">Type to wrap to smart pointer. </typeparam>
		/// <typeparam name="...Args">Constructor argument types. </typeparam>
		/// <param name="...args">Constructor arguments. </param>
		/// <returns>New std::shared_ptr of template type. </returns>
		template <typename T, typename... Args>
		shared_ptr make_shared(Args... args)
		{
			return std::make_shared<T>(args...);
		}
	}
}