# include "../logger_stream.h"
# include "zx.h"

class some_class
{
public:
	int foo1(int a, int b) const
	{
		return a * b;
	}

	void foo2(int a, int b)
	{
		ls << "...mul2 called with args " << a << " " << b << zx::endl;
	}

	int foo3() const
	{
		return 42;
	}

	void foo4()
	{
		ls << "...mul4 prints hello. " << zx::endl;
	}
};