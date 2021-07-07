# include "zx_rtti_args.h"

namespace zx
{
	namespace rtti
	{
		const args args::empty{ std::deque<std::type_index>() };

		bool args::operator==(const args& other) const
		{
			return std::hash<args>()(*this) ==
				std::hash<args>()(other);
		}
	}
}

