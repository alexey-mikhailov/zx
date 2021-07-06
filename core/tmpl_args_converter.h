# pragma once

namespace zx
{
	template <class... Args>
	struct tmpl_args_converter;

	template<class First, class... Args>
	struct tmpl_args_converter<First, Args...>
	{
		static std::deque<std::type_index> to_rtti()
		{
			auto result = tmpl_args_converter<Args...>::to_rtti();
			result.push_front(typeid(First));
			return result;
		}
	};

	template<>
	struct tmpl_args_converter<>
	{
		static std::deque<std::type_index> to_rtti()
		{
			return std::deque<std::type_index>();
		}
	};
}
