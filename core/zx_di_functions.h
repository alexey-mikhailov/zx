# pragma once
# include "zx_type.h"

namespace zx
{
	namespace meta
	{
		class fieldpawn;
		class named_fieldpawn;
	}

	/// <summary>
	/// Injects dependencies for some instance of some type. 
	/// </summary>
	/// <param name="type">Reflected type of instance being injectable. </param>
	/// <param name="target">Address of instance to inject to. </param>
	ZX_API void inject_dependencies(const type& type, void* target);

	/// <summary>
	/// Injects dependencies for some instance of some type. 
	/// </summary>
	/// <typeparam name="Owner">Type of instance being injeectable. </typeparam>
	/// <param name="target">Instance being injectable. </param>
	template <class Owner>
	void inject_dependencies(Owner* target)
	{
		inject_dependencies(type::i<Owner>(), target);
	}

	/// <summary>
	/// Makes metadata for field pawn of singleton. 
	/// which may be used in zx::registry::add_field method overload. 
	/// </summary>
	/// <returns>Named instance wrapped into std::unique_ptr. </returns>
	ZX_API std::unique_ptr<meta::fieldpawn> make_fieldpawn_of_singleton();

	/// <summary>
	/// Makes metadata for field pawn of signal pack singleton. 
	/// which may be used in zx::registry::add_field method overload. 
	/// </summary>
	/// <returns>Named instance wrapped into std::unique_ptr. </returns>
	ZX_API std::unique_ptr<meta::fieldpawn> make_fieldpawn_of_signal_pack();

	/// <summary>
	/// Makes metadata for field pawn of named instance. 
	/// which may be used in zx::registry::add_field method overload. 
	/// </summary>
	/// <param name="name">Name of named singleton. </param>
	/// <returns>Named instance wrapped into std::unique_ptr. </returns>
	ZX_API std::unique_ptr<meta::named_fieldpawn> make_fieldpawn_of_nameleton(char const* name);

	/// <summary>
	/// Makes metadata for field pawn of named instance. 
	/// which may be used in zx::registry::add_field method overload. 
	/// </summary>
	/// <param name="name">Name of named singleton. </param>
	/// <returns>Named instance wrapped into std::unique_ptr. </returns>
	ZX_API std::unique_ptr<meta::named_fieldpawn> make_fieldpawn_of_nameleton(std::string name);

	/// <summary>
	/// Makes metadata for field pawn of named instance. 
	/// which may be used in zx::registry::add_field method overload. 
	/// </summary>
	/// <param name="name">Name of named singleton. </param>
	/// <returns>Named instance wrapped into std::unique_ptr. </returns>
	ZX_API std::unique_ptr<meta::named_fieldpawn> make_fieldpawn_of_nameleton(std::string&& name);
}
