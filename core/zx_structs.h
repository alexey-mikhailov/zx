# pragma once
# include "zx_type.h"
# include "zx_rtti_shared_ptr.h"

namespace zx
{
	struct singleton_signature
	{
		bool is_mapped_to_interface;
		const type& interface_type;
		const type& implementation_type;
		std::function<zx::rtti::shared_ptr()> singleton_creator;

		ZX_API singleton_signature(bool is_mapped_to_interface,
								   const type& interface_type,
								   const type& implementation_type, 
								   std::function<zx::rtti::shared_ptr()> singleton_creator);
	};

	struct nomination_signature
	{
		bool is_mapped_to_interface;
		const type& interface_type;
		const type& implementation_type;
		std::string name;
		std::function<zx::rtti::shared_ptr()> singleton_creator;

		ZX_API nomination_signature(bool is_mapped_to_interface,
									const type& interface_type,
									const type& implementation_type, 
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

	struct bind_nomination_result
	{
		ZX_API bind_nomination_result(class nomination_container *owner,
									  nomination_signature signature);

		ZX_API void create();
	private:
		class nomination_container * _owner;
		nomination_signature _signature;
	};

	struct singleton_identity
	{
		const type& type;

		ZX_API singleton_identity(const zx::type &type);

		ZX_API bool operator <(const singleton_identity &other) const;
		ZX_API bool operator >(const singleton_identity &other) const;
	};

	struct nomination_identity
	{
		const type& type;
		std::string name;

		nomination_identity(const zx::type &type,
							std::string name);

		bool operator <(const nomination_identity &other) const;
		bool operator >(const nomination_identity &other) const;
	};
}