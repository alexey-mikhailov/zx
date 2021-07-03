# pragma once
# include "some_class.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


inline int glob1(int a, int b)
{
	return a * b;
}

inline void glob2(int a, int b)
{
	const auto result = a * b;
	std::wstringstream ss;
	ss << "...mul2 called with args " << a << " " << b << std::endl;
	Logger::WriteMessage(ss.str().c_str());
}

inline int glob3()
{
	return 42;
}

inline void glob4()
{
	std::wstringstream ss;
	ss << "...mul4 called" << std::endl;
	Logger::WriteMessage(ss.str().c_str());
}


inline void sbscribe_invoke_unsubscribe(
	zx::multicast_delegate<int(int, int)> del1,
	zx::multicast_delegate<void(int, int)> del2,
	zx::multicast_delegate<int()> del3,
	zx::multicast_delegate<void()> del4)
{
	try
	{
		some_class some;

		// del 1
		del1 += glob1;

		del1.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << L"Del1: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  }, 1, 2);

		auto result = del1(3, 4);

		auto memfn = zx::mem_fn(&some, &some_class::foo1);
		del1 += memfn;

		del1.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << L"Del1: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  }, 5, 6);

		result = del1(7, 8);

		del1 -= glob1;

		del1.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << "Del1: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  }, 9, 10);

		result = del1(11, 12);

		del1 -= zx::mem_fn(&some, &some_class::foo1);
		result = del1(13, 14);
		del1.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << "Del1: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  }, 15, 16);


		// del 2
		del2 += glob2;
		del2(19, 20);

		del2 += zx::mem_fn(&some, &some_class::foo2);
		del2(23, 24);

		del2 -= glob2;
		del2(5, 7);

		del2 -= zx::mem_fn(&some, &some_class::foo2);
		del2(5, 7);


		// del 3
		del3 += glob3;
		del3.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << "Del3: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  });
		result = del3();

		del3 += zx::mem_fn(&some, &some_class::foo3);
		del3.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << "Del3: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  });
		result = del3();

		del3 -= glob3;
		del3.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << "Del3: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  });
		result = del3();

		del3 -= zx::mem_fn(&some, &some_class::foo3);
		del3.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << "Del3: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  });
		result = del3();


		// del 4
		del4 += glob4;
		del4();

		del4 += zx::mem_fn(&some, &some_class::foo4);
		del4();

		del4 -= glob4;
		del4();

		del4 -= zx::mem_fn(&some, &some_class::foo4);
		del4();
	}
	catch (std::exception& ex)
	{
		std::wstringstream ss;
		ss << ex.what() << std::endl;
		Logger::WriteMessage(ss.str().c_str());
		throw;
	}
}