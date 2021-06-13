# include "zx_structs.h"
# include "singleton_container.h"
# include "nomination_container.h"

namespace zx
{
	singleton_signature::singleton_signature(bool is_mapped_to_interface,
											 type interface_type,  // NOLINT(performance-unnecessary-value-param, performance-unnecessary-value-param)
											 type implementation_type) :  // NOLINT(performance-unnecessary-value-param)
		is_mapped_to_interface(is_mapped_to_interface),
		interface_type(interface_type),
		implementation_type(implementation_type)
	{
	}

	nomination_signature::nomination_signature(bool is_mapped_to_interface,
											   type interface_type,  // NOLINT(performance-unnecessary-value-param)
											   type implementation_type,   // NOLINT(performance-unnecessary-value-param)
											   std::string name) :
		is_mapped_to_interface(is_mapped_to_interface),
		interface_type(interface_type),
		implementation_type(implementation_type),
		name(std::move(name))
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

	bind_nomination_result::bind_nomination_result(nomination_container* owner, 
												   nomination_signature signature) :
		_owner(owner),
		_signature(std::move(signature))
	{
	}

	void bind_nomination_result::create()
	{
		_owner->get_or_add(
			_signature.is_mapped_to_interface
				? _signature.interface_type
				: _signature.implementation_type,
			_signature.name);
	}

	nomination_identity::nomination_identity(const zx::type &type,   
											 std::string name):
		type(type),
		name(std::move(name))
	{
	}

	bool nomination_identity::operator<(const nomination_identity& other) const
	{
		return type < other.type || name < other.name;
	}

	bool nomination_identity::operator>(const nomination_identity& other) const
	{
		return type > other.type || name > other.name;
	}
}
