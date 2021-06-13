# pragma once
# include "zx_structs.h"

namespace zx
{
	class nomination_container
	{
	public:
		explicit nomination_container(const class di_container *owner);
		virtual ~nomination_container() = default;

		template<class T>
		T *get_or_add(const std::string &name);

		void *get_or_add(const type &type, const std::string& name);

		template<class TImplementation>
		bind_nomination_result bind_singleton(const std::string &name);

		template<class TInterface, class TImpl>
		bind_nomination_result bind_singleton(const std::string &name);

	private:
		const class di_container *	_owner;

		std::vector<nomination_signature> _signatures;
		std::map<nomination_identity, void *> _singletons;
	};

	template <class T>
	T* nomination_container::get_or_add(const std::string& name)
	{
		return get_or_add(type::i<T>(), name);
	}

	template <class TImplementation>
	bind_nomination_result nomination_container::bind_singleton(const std::string &name)
	{
		const auto signature = nomination_signature(false,
													type::null,
													type::i<TImplementation>(),
													name);

		_signatures.push_back(signature);
		return bind_nomination_result(this, signature);
	}

	template <class TInterface, class TImpl>
	bind_nomination_result nomination_container::bind_singleton(const std::string& name)
	{
		const auto signature = nomination_signature(true,
													type::i<TInterface>(),
													type::i<TImpl>(),
													name);

		_signatures.push_back(signature);
		return bind_nomination_result(this, signature);
	}
}

