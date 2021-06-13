# pragma once
# include "zx_structs.h"

namespace zx
{
	class singleton_container
	{
	public:
		explicit singleton_container(const class di_container *owner);
		virtual ~singleton_container() = default;

		template<class T>
		T *get_or_add();

		void *get_or_add(const type &type);

		template<class TImplementation>
		bind_singleton_result bind_singleton();

		template<class TInterface, class TImpl>
		bind_singleton_result bind_singleton();

	private:
		const class di_container*	_owner;

		std::vector<singleton_signature> _signatures;
		std::map<type, void *> _singletons;
	};



	template <class T>
	T* singleton_container::get_or_add()
	{
		return get_or_add(type::i<T>());
	}

	template <class TImplementation>
	bind_singleton_result singleton_container::bind_singleton()
	{
		const auto signature = singleton_signature(false,
												   type::null,
												   type::i<TImplementation>());

		_signatures.push_back(signature);
		return bind_singleton_result(this, signature);
	}

	template <class TInterface, class TImpl>
	bind_singleton_result singleton_container::bind_singleton()
	{
		const auto signature = singleton_signature(true,
												   type::i<TInterface>(),
												   type::i<TImpl>());

		_signatures.push_back(signature);
		return bind_singleton_result(this, signature);
	}
}