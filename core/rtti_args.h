# pragma once
# include "zx_defines.h"

namespace zx
{
	struct rtti_args final
	{
		size_t size;
		std::deque<std::type_index> args;

		ZX_API rtti_args(const std::deque<std::type_index>& args) :
			size(args.size()), args(args)
		{
		}

		ZX_API bool operator ==(const rtti_args& other) const;

		ZX_API static const rtti_args empty;
	};
}

namespace std
{
	template<>
	struct hash<zx::rtti_args> final
	{
		size_t operator()(const zx::rtti_args& key) const noexcept
		{
			auto result = hash<size_t>()(key.size);

			for (auto arg : key.args)
			{
				result ^= arg.hash_code();
			}

			return result;
		}
	};
}



