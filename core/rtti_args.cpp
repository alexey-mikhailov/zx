# include "rtti_args.h"

namespace zx
{
	bool rtti_args::operator==(const rtti_args& other) const
	{
		return std::hash<rtti_args>()(*this) ==
			std::hash<rtti_args>()(other);
	}
}

