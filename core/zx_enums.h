# pragma once

namespace zx
{
	typedef enum
	{
		value,
		rawptr,
		unqptr,
		shrptr
	}
	expose_type;

	namespace inject_type 
	{
		enum val
		{
			none,
			singleton,
			named_instance,
			signal_pack
		};
	}
}
