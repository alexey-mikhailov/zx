# pragma once
# include "zx_type.h"

namespace zx
{
	struct singleton_signature
	{
		bool is_mapped_to_interface;
		type interface_type;
		type implementation_type;

		ZX_API singleton_signature(bool is_mapped_to_interface,
											type interface_type,
											type implementation_type);
	};

	struct nomination_signature
	{
		bool is_mapped_to_interface;
		type interface_type;
		type implementation_type;
		std::string name;

		ZX_API nomination_signature(bool is_mapped_to_interface,
											 type interface_type,
											 type implementation_type, 
											 std::string name);
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

	struct nomination_identity
	{
		type type;
		std::string name;

		nomination_identity(const zx::type &type,
		                    std::string name);

		bool operator <(const nomination_identity &other) const;
		bool operator >(const nomination_identity &other) const;
	};
}