# pragma once
# include "singleton_container.h"
# include "nomination_container.h"
# include "properties/zx_ref_getter.h"

namespace zx
{
	/// Base abstract class for DI container
	class di_container
	{
		std::string					_name;

		class singleton_container* _singleton_container;
		class nomination_container* _nomination_container;

	public:
		ref_getter<singleton_container> singleton_container = &_singleton_container;
		ref_getter<nomination_container> nomination_container = &_nomination_container;

	protected:
		ZX_API explicit di_container(const std::string& name);
		ZX_API virtual ~di_container();

		ZX_API virtual void bind_all() = 0;

		template<class TImplementation>
		bind_singleton_result bind_singleton();

		template<class TInterface, class TImpl>
		bind_singleton_result bind_singleton();

		template<class TImplementation>
		bind_nomination_result bind_nominated(const std::string &name);

		template<class TInterface, class TImpl>
		bind_nomination_result bind_nominated(const std::string &name);

		template <class TSignalPack>
		void register_signal_pack();

	private:
		friend ZX_API void inject_dependencies(const type& type, void* target);
		static std::unordered_map<std::string, di_container *> __instances;
	};

	template <class TImplementation>
	bind_singleton_result di_container::bind_singleton()
	{
		return _singleton_container->bind_singleton<TImplementation>();
	}

	template <class TInterface, class TImpl>
	bind_singleton_result di_container::bind_singleton()
	{
		return _singleton_container->bind_singleton<TInterface, TImpl>();
	}

	template <class TImplementation>
	bind_nomination_result di_container::bind_nominated(const std::string& name)
	{
		return _nomination_container->bind_singleton<TImplementation>(name);
	}

	template <class TInterface, class TImpl>
	bind_nomination_result di_container::bind_nominated(const std::string& name)
	{
		return _nomination_container->bind_singleton<TInterface, TImpl>(name);
	}

	template <class TSignalPack>
	void di_container::register_signal_pack()
	{
		_singleton_container->bind_singleton<TSignalPack>();
	}
}
