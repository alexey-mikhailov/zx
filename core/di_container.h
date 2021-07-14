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

		/// <summary>
		/// Binds named (nominated) singleton to container.
		/// Does not create instance of it. 
		/// </summary>
		/// <typeparam name="TImplementation">Raw type of implementation</typeparam>
		/// <typeparam name="...Args">
		/// Constructor arguments which will be propagated
		/// during creation time. 
		/// </typeparam>
		/// <returns>Functional-style result of binding</returns>
		/// <remarks>
		/// Since singletons are std::shared_ptr instances,
		/// it's important to keep in mind
		/// that binding uses raw types, not of shared_ptr's. 
		/// </remarks>
		template<class TImplementation, class... Args>
		bind_singleton_result bind_singleton(Args... args);

		/// <summary>
		/// Binds singleton to container.
		/// Does not create instance of it. 
		/// </summary>
		/// <typeparam name="TInterface">Raw type of interface</typeparam>
		/// <typeparam name="TImpl">Raw type of implementation</typeparam>
		/// <typeparam name="...Args">
		/// Constructor arguments which will be propagated
		/// during creation time. 
		/// </typeparam>
		/// <returns>Functional-style result of binding</returns>
		/// <remarks>
		/// Since singletons are std::shared_ptr instances,
		/// it's important to keep in mind
		/// that binding uses raw types, not of shared_ptr's. 
		/// </remarks>
		template<class TInterface, class TImpl, class... Args>
		bind_singleton_result bind_singleton(Args... args);

		/// <summary>
		/// Binds named (nominated) singleton to container.
		/// Does not create instance of it. 
		/// </summary>
		/// <typeparam name="TImplementation">Raw type of implementation</typeparam>
		/// <typeparam name="...Args">
		/// Constructor arguments which will be propagated
		/// during creation time. 
		/// </typeparam>
		/// <returns>Functional-style result of binding</returns>
		/// <remarks>
		/// Since singletons are std::shared_ptr instances,
		/// it's important to keep in mind
		/// that binding uses raw types, not of shared_ptr's. 
		/// </remarks>
		template<class TImplementation, class... Args>
		bind_nomination_result bind_nominated(const std::string &name, Args... args);

		/// <summary>
		/// Binds singleton to container.
		/// Does not create instance of it. 
		/// </summary>
		/// <typeparam name="TInterface">Raw type of interface</typeparam>
		/// <typeparam name="TImpl">Raw type of implementation</typeparam>
		/// <typeparam name="...Args">
		/// Constructor arguments which will be propagated
		/// during creation time. 
		/// </typeparam>
		/// <returns>Functional-style result of binding</returns>
		/// <remarks>
		/// Since singletons are std::shared_ptr instances,
		/// it's important to keep in mind
		/// that binding uses raw types, not of shared_ptr's. 
		/// </remarks>
		template<class TInterface, class TImpl, class... Args>
		bind_nomination_result bind_nominated(const std::string &name, Args... args);

		/// <summary>
		/// Registers bunch of signals 
		/// which may be used as singleton 
		/// to subscribe methods, functors
		/// and fire them. 
		/// </summary>
		/// <typeparam name="TSignalPack">Raw type of signal pack</typeparam>
		/// <remarks>
		/// Since signal packs are std::shared_ptr instances,
		/// it's important to keep in mind
		/// that binding uses raw types, not of shared_ptr's. 
		/// </remarks>
		template <class TSignalPack>
		void register_signal_pack();

	private:
		friend ZX_API void inject_dependencies(const type& type, void* target);
		static std::unordered_map<std::string, di_container *> __instances;
	};

	template <class TImplementation, class... Args>
	bind_singleton_result di_container::bind_singleton(Args... args)
	{
		return _singleton_container->bind_concrete_singleton<TImplementation, Args...>(args...);
	}

	template <class TInterface, class TImpl, class... Args>
	bind_singleton_result di_container::bind_singleton(Args... args)
	{
		return _singleton_container->bind_abstract_singleton<TInterface, TImpl, Args...>(args...);
	}

	template <class TImplementation, class... Args>
	bind_nomination_result di_container::bind_nominated(const std::string& name, Args... args)
	{
		return _nomination_container->bind_concrete_singleton<TImplementation, Args...>(name, args...);
	}

	template <class TInterface, class TImpl, class... Args>
	bind_nomination_result di_container::bind_nominated(const std::string& name, Args... args)
	{
		return _nomination_container->bind_abstract_singleton<TInterface, TImpl, Args...>(name, args...);
	}

	template <class TSignalPack>
	void di_container::register_signal_pack()
	{
		_singleton_container->bind_concrete_singleton<TSignalPack>();
	}
}
