# pragma once
# include "zx_type.h"

namespace zx
{
	namespace meta
	{
		class fieldpawn;
		class named_fieldpawn;
	}

	ZX_API void inject_dependencies(const type& type, void* target);

	template <class Owner>
	void inject_dependencies(Owner* target)
	{
		inject_dependencies(type::i<Owner>(), target);
	}

	ZX_API std::unique_ptr<meta::fieldpawn> make_fieldpawn_of_singleton();
	ZX_API std::unique_ptr<meta::fieldpawn> make_fieldpawn_of_signal_pack();
	ZX_API std::unique_ptr<meta::named_fieldpawn> make_fieldpawn_of_nameleton(char const* name);
	ZX_API std::unique_ptr<meta::named_fieldpawn> make_fieldpawn_of_nameleton(std::string name);
	ZX_API std::unique_ptr<meta::named_fieldpawn> make_fieldpawn_of_nameleton(std::string&& name);
}
