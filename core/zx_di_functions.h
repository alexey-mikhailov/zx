# pragma once
# include "zx_type.h"

namespace zx
{
	ZX_API 
	void inject_dependencies(const type& type, void* target);

	template <class Owner>
	void inject_dependencies(Owner* target)
	{
		inject_dependencies(type::i<Owner>(), target);
	}
}
