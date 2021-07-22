# pragma once

namespace zx
{
	namespace meta
	{
		enum struct expose_type
		{
			value,

			shrptr
		};

		enum struct fieldpawn_type
		{
			none,
			singleton,
			nameleton,
			signal_pack
		};
	}
}
