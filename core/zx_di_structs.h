# pragma once
# include "zx_type.h"
# include "zx_rtti_shared_ptr.h"

namespace zx
{
	struct singleton_signature
	{
		bool is_mapped_to_interface;
		type interface_type;
		type implementation_type;
		std::function<zx::rtti::shared_ptr()> singleton_creator;

		ZX_API singleton_signature(bool is_mapped_to_interface,
								   type interface_type,
								   type implementation_type, 
								   std::function<zx::rtti::shared_ptr()> singleton_creator);
	};

	struct nameleton_signature
	{
		bool is_mapped_to_interface;
		type interface_type;
		type implementation_type;
		std::string name;
		std::function<zx::rtti::shared_ptr()> singleton_creator;

		ZX_API nameleton_signature(bool is_mapped_to_interface,
								   type interface_type,
								   type implementation_type,
								   std::string name,
								   std::function<zx::rtti::shared_ptr()> singleton_creator);
	};

	struct bind_singleton_result
	{
		ZX_API bind_singleton_result(class singleton_container* owner,
									 singleton_signature signature);

		ZX_API void create();

	private:
		class singleton_container * _owner;
		singleton_signature _signature;
	};

	struct bind_nameleton_result
	{
		ZX_API bind_nameleton_result(class nameleton_container *owner,
									  nameleton_signature signature);

		ZX_API void create();

	private:
		class nameleton_container * _owner;
		nameleton_signature _signature;
	};

	struct singleton_identity
	{
		type type;

		ZX_API singleton_identity(zx::type type);

		ZX_API bool operator <(const singleton_identity &other) const;
		ZX_API bool operator >(const singleton_identity &other) const;
	};

	struct nameleton_identity
	{
		type type;
		std::string name;

		nameleton_identity(zx::type type,
						   std::string name);

		bool operator <(const nameleton_identity &other) const;
		bool operator >(const nameleton_identity &other) const;
	};
}