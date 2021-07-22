# pragma once
# include "zx_di_structs.h"
# include "zx_rtti_functions.h"

namespace zx
{
	/// Contains named singletons
	class nameleton_container
	{
	public:
		explicit nameleton_container(const class di_container *owner);
		virtual ~nameleton_container() = default;

		template<class T>
		std::shared_ptr<T> get_or_add(const std::string& name);

		zx::rtti::shared_ptr get_or_add(const type& type, const std::string& name);

		template<class TImplementation, class... Args>
		bind_nameleton_result bind_concrete_nameleton(const std::string& name, Args... args);

		template<class TInterface, class TImpl, class... Args>
		bind_nameleton_result bind_abstract_nameleton(const std::string& name, Args... args);

	private:
		const class di_container*				_owner;
		std::vector<nameleton_signature>		_signatures;
		std::map<nameleton_identity, zx::rtti::shared_ptr>	_singletons;
	};

	template <class T>
	std::shared_ptr<T> nameleton_container::get_or_add(const std::string& name)
	{
		auto result = get_or_add(type::i<T>(), name);
		return zx::rtti::shared_ptr::to_shared_ptr<T>(result);
	}

	template <class TImplementation, class... Args>
	bind_nameleton_result nameleton_container::bind_concrete_nameleton(const std::string& name, Args... args)
	{
		std::function<zx::rtti::shared_ptr()> singleton_creator =
		[args...]()
		{
			return zx::rtti::make_shared<TImplementation>(args...);
		};

		const auto signature = nameleton_signature(false,
												   type::null,
												   type::i<std::shared_ptr<TImplementation>>(),
												   name,
												   singleton_creator);

		_signatures.push_back(signature);
		return bind_nameleton_result(this, signature);
	}

	template <class TInterface, class TImpl, class... Args>
	bind_nameleton_result nameleton_container::bind_abstract_nameleton(const std::string& name, Args... args)
	{
		std::function<zx::rtti::shared_ptr()> singleton_creator =
		[&args...]()
		{
			return zx::rtti::make_shared<TImpl>(args...);
		};

		const auto signature = nameleton_signature(true,
												   type::i<std::shared_ptr<TInterface>>(),
												   type::i<std::shared_ptr<TImpl>>(),
												   name,
												   singleton_creator);
		_signatures.push_back(signature);
		return bind_nameleton_result(this, signature);
	}
}

