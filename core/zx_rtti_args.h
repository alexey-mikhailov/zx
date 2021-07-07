# pragma once
# include "zx_defines.h"

namespace zx
{
	namespace rtti
	{
		struct args final
		{
			size_t size;
			std::deque<std::type_index> data;

			ZX_API args(const std::deque<std::type_index>& args) :
				size(args.size()), data(args)
			{
			}

			ZX_API bool operator ==(const args& other) const;

			ZX_API static const args empty;
		};
	}
}

namespace std
{
	template<>
	struct hash<zx::rtti::args> final
	{
		size_t operator()(const zx::rtti::args& key) const noexcept
		{
			auto result = hash<size_t>()(key.size);

			for (auto arg : key.data)
			{
				result ^= arg.hash_code();
			}

			return result;
		}
	};
}



