# include "zx_di_structs.h"
# include "zx_singleton_container.h"
# include "zx_nameleton_container.h"

namespace zx
{
	singleton_signature::singleton_signature(bool is_mapped_to_interface,
											 type interface_type,
											 type implementation_type,
											 std::function<zx::rtti::shared_ptr()> singleton_creator) :
		is_mapped_to_interface(is_mapped_to_interface),
		interface_type(interface_type),
		implementation_type(implementation_type),
		singleton_creator(std::move(singleton_creator))
	{
	}

	nameleton_signature::nameleton_signature(bool is_mapped_to_interface,
											   type interface_type,
											   type implementation_type,
											   std::string name, 
											   std::function<zx::rtti::shared_ptr()> singleton_creator) :
		is_mapped_to_interface(is_mapped_to_interface),
		interface_type(interface_type),
		implementation_type(implementation_type),
		name(std::move(name)), 
		singleton_creator(std::move(singleton_creator))
	{
	}

	bind_singleton_result::bind_singleton_result(singleton_container* owner,
												 singleton_signature signature) :
		_owner(owner),
		_signature(std::move(signature))
	{
	}

	void bind_singleton_result::create()
	{
		_owner->get_or_add(_signature.is_mapped_to_interface
						 ? _signature.interface_type
						 : _signature.implementation_type);
	}

	bind_nameleton_result::bind_nameleton_result(nameleton_container* owner,
												 nameleton_signature signature) :
		_owner(owner),
		_signature(std::move(signature))
	{
	}

	void bind_nameleton_result::create()
	{
		_owner->get_or_add(
			_signature.is_mapped_to_interface
				? _signature.interface_type
				: _signature.implementation_type,
			_signature.name);
	}

	singleton_identity::singleton_identity(zx::type type) :
		type(type)
	{
	}

	bool singleton_identity::operator<(const singleton_identity& other) const
	{
		return type < other.type;
	}

	bool singleton_identity::operator>(const singleton_identity& other) const
	{
		return type > other.type;
	}

	nameleton_identity::nameleton_identity(zx::type type,
										   std::string name) :
		type(type),
		name(std::move(name))
	{
	}

	bool nameleton_identity::operator<(const nameleton_identity& other) const
	{
		return type < other.type || name < other.name;
	}

	bool nameleton_identity::operator>(const nameleton_identity& other) const
	{
		return type > other.type || name > other.name;
	}
}
