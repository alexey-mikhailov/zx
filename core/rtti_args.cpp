# include "rtti_args.h"

namespace zx
{
	const rtti_args rtti_args::empty{ std::deque<std::type_index>() };

	bool rtti_args::operator==(const rtti_args& other) const
	{
		return std::hash<rtti_args>()(*this) ==
			std::hash<rtti_args>()(other);
	}
}

