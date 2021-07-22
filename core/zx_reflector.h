# pragma once

# include "zx_type.h"
# include "zx_rtti_args.h"

namespace zx
{
	/// <summary>
	/// Reflector of templated type to zx::rtti...
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	struct refl
	{
		template<typename... Sig>
		struct ctor;

		/// <summary>
		/// Constructor reflector. 
		/// </summary>
		/// <typeparam name="First">First argument type. </typeparam>
		/// <typeparam name="...Args">Remaining argument types from parameter pack. </typeparam>
		template<typename First, typename... Args>
		struct ctor <First, Args...>
		{
			static_assert(
				std::is_constructible_v<T, First, Args...>, 
				"Type is not constructible from these args. ");

			static rtti::args make_args()
			{
				return tmpl_args_converter<First, Args...>::to_rtti();
			}

			static fnptr make_fnptr()
			{
				void* (*temp)(First, Args...) =
				[](First first, Args... args) -> void*
				{
					return new T(first, args...);
				};

				return reinterpret_cast<fnptr>(temp);
			}

			static const zx::type& ensure()
			{
				return type::ensure<T, zx::refl<T>::template ctor<First, Args...>>();
			}

		private:
			ctor() = delete;
			ctor(const ctor&) = delete;
			ctor(ctor&&) = delete;
		};

		/// <summary>
		/// Constructor reflector. 
		/// </summary>
		template<>
		struct ctor <>
		{
			static_assert(
				std::is_default_constructible_v<T>, 
				"Type is not default constructible. "); 

			static rtti::args make_args()
			{
				return tmpl_args_converter<>::to_rtti();
			}

			static fnptr make_fnptr()
			{
				void* (*temp)() = []() -> void*
				{
					return new T();
				};

				return reinterpret_cast<fnptr>(temp);
			}

			static const zx::type& ensure()
			{
				return type::ensure<T, zx::refl<T>::template ctor<>>();
			}
		};
	};
}