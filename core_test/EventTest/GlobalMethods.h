# pragma once
# include "EventUser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


inline int Glob1(int a, int b)
{
	return a * b;
}

inline void Glob2(int a, int b)
{
	const auto result = a * b;
	std::wstringstream ss;
	ss << "...mul2 called with args " << a << " " << b << std::endl;
	Logger::WriteMessage(ss.str().c_str());
}

inline int Glob3()
{
	return 42;
}

inline void Glob4()
{
	std::wstringstream ss;
	ss << "...mul4 called" << std::endl;
	Logger::WriteMessage(ss.str().c_str());
}


inline void SubscribeInvokeUnsubscribe(
	zx::multicast_delegate<int(int, int)> del1,
	zx::multicast_delegate<void(int, int)> del2,
	zx::multicast_delegate<int()> del3,
	zx::multicast_delegate<void()> del4)
{
	try
	{
		SomeClass some;

		// del 1
		del1 += Glob1;

		del1.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << L"Del1: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  }, 1, 2);

		auto result = del1(3, 4);

		auto memfn = zx::mem_fn(&some, &SomeClass::Foo1);
		del1 += memfn;

		del1.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << L"Del1: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  }, 5, 6);

		result = del1(7, 8);

		del1 -= Glob1;

		del1.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << "Del1: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  }, 9, 10);

		result = del1(11, 12);

		del1 -= zx::mem_fn(&some, &SomeClass::Foo1);
		result = del1(13, 14);
		del1.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << "Del1: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  }, 15, 16);


		// del 2
		del2 += Glob2;
		del2(19, 20);

		del2 += zx::mem_fn(&some, &SomeClass::Foo2);
		del2(23, 24);

		del2 -= Glob2;
		del2(5, 7);

		del2 -= zx::mem_fn(&some, &SomeClass::Foo2);
		del2(5, 7);


		// del 3
		del3 += Glob3;
		del3.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << "Del3: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  });
		result = del3();

		del3 += zx::mem_fn(&some, &SomeClass::Foo3);
		del3.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << "Del3: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  });
		result = del3();

		del3 -= Glob3;
		del3.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << "Del3: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  });
		result = del3();

		del3 -= zx::mem_fn(&some, &SomeClass::Foo3);
		del3.handle_every([](int result)
						  {
							  std::wstringstream ss;
							  ss << "Del3: Result of current: " << result << std::endl;
							  Logger::WriteMessage(ss.str().c_str());
						  });
		result = del3();


		// del 4
		del4 += Glob4;
		del4();

		del4 += zx::mem_fn(&some, &SomeClass::Foo4);
		del4();

		del4 -= Glob4;
		del4();

		del4 -= zx::mem_fn(&some, &SomeClass::Foo4);
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