# pragma once

namespace zx
{
	enum struct expose_type
	{
		value,
		rawptr,
		shrptr
	};

	enum struct inject_type
	{
		none,
		singleton,
		named_instance,
		signal_pack
	};
}
