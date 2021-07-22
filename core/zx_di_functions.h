# pragma once
# include "zx_type.h"

namespace zx
{
	namespace meta
	{
		class named_fieldpawn;
	}

	ZX_API void inject_dependencies(const type& type, void* target);

	template <class Owner>
	void inject_dependencies(Owner* target)
	{
		inject_dependencies(type::i<Owner>(), target);
	}

	ZX_API std::unique_ptr<meta::named_fieldpawn> make_named_fieldpawn(char const* name);
	ZX_API std::unique_ptr<meta::named_fieldpawn> make_named_fieldpawn(std::string name);
	ZX_API std::unique_ptr<meta::named_fieldpawn> make_named_fieldpawn(std::string&& name);
}
