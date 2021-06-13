# pragma once

namespace zx
{
	/// Iterator range
	template <class Iterator>
	struct it_range
	{
		Iterator begin;
		Iterator end;

		it_range(Iterator begin, Iterator end) : 
			begin(begin), end(end)
		{
		}
	};
}
