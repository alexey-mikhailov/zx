# pragma once
# include "zx_di_structs.h"
# include "zx_rtti_functions.h"

namespace zx
{
	class singleton_container
	{
	public:
		explicit singleton_container(const class di_container* owner);
		virtual ~singleton_container() = default;

		template<class T>
		std::shared_ptr<T> get_or_add();

		zx::rtti::shared_ptr get_or_add(const type& type);

		template<class TImplementation, class... Args>
		bind_singleton_result bind_concrete_singleton(Args... args);

		template<class TInterface, class TImpl, class... Args>
		bind_singleton_result bind_abstract_singleton(Args... args);

	private:
		const class di_container*			_owner;
		std::vector<singleton_signature>	_signatures;
		std::map<singleton_identity, zx::rtti::shared_ptr> _singletons;
	};



	template <class T>
	std::shared_ptr<T> singleton_container::get_or_add()
	{
		auto result = get_or_add(type::i<T>());
		return zx::rtti::shared_ptr::to_shared_ptr<T>(result);
	}

	template <class TImplementation, class... Args>
	bind_singleton_result singleton_container::bind_concrete_singleton(Args... args)
	{
		std::function<zx::rtti::shared_ptr()> singleton_creator =
		[args...]()
		{
			return zx::rtti::make_shared<TImplementation>(args...);
		};

		const auto signature = singleton_signature(false,
												   type::null,
												   type::i<std::shared_ptr<TImplementation>>(),
												   singleton_creator);

		_signatures.push_back(signature);
		return bind_singleton_result(this, signature);
	}

	template <class TInterface, class TImpl, class... Args>
	bind_singleton_result singleton_container::bind_abstract_singleton(Args... args)
	{
		std::function<zx::rtti::shared_ptr()> singleton_creator =
		[args...]()
		{
			return zx::rtti::make_shared<TImpl>(args...);
		};

		const auto signature = singleton_signature(true,
												   type::i<std::shared_ptr<TInterface>>(),
												   type::i<std::shared_ptr<TImpl>>(), 
												   singleton_creator);

		_signatures.push_back(signature);
		return bind_singleton_result(this, signature);
	}
}