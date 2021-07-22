# pragma once

namespace zx
{
	namespace meta
	{
		enum struct expose_type
		{
			value,

			rawptr,
			shrptr
		};

		enum struct fieldpawn_type
		{
			none,
			singleton,
			named_fieldpawn,
			signal_pack
		};
	}
}
