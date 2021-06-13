# pragma once

# ifdef ZX_EXPORTS
#	define ZX_API __declspec(dllexport)
# else
#	define ZX_API __declspec(dllimport)
# endif

# define nameof(x) (#x)

namespace zx
{
	typedef void(*fnptr)();
	typedef void(*action)();

	template <class Ret>
	using	func = Ret(*)();


	template<class Data, class Owner>
	using read_value_delegate = Data(*)(Owner&);

	template<class Data, class Owner>
	using write_value_delegate = void(*)(Owner&, const Data&);

	template<class Data, class Owner>
	using read_rawptr_delegate = Data & (*)(Owner&);

	template<class Data, class Owner>
	using write_rawptr_delegate = void(*)(Owner&, Data*);

	template<class Data, class Owner>
	using read_shrptr_delegate = std::shared_ptr<Data>(*)(Owner&);

	template<class Data, class Owner>
	using write_shrptr_delegate = void(*)(Owner&, std::shared_ptr<Data>);
}
