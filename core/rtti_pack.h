# pragma once

namespace zx
{
	template <class... Args>
	struct rtti_pack;

	template<class First, class... Args>
	struct rtti_pack<First, Args...>
	{
		static std::deque<std::type_index> get()
		{
			auto result = rtti_pack<Args...>::get();
			result.push_front(typeid(First));
			return result;
		}
	};

	template<>
	struct rtti_pack<>
	{
		static std::deque<std::type_index> get()
		{
			return std::deque<std::type_index>();
		}
	};
}
