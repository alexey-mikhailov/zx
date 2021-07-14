#include "PCH.h"
#include "nomination_container.h"

namespace zx
{
	nomination_container::nomination_container(const di_container* owner) :
		_owner(owner)
	{
	}

	zx::rtti::shared_ptr nomination_container::get_or_add(const type& type,
														  const std::string& name)
	{
		zx::rtti::shared_ptr result;

		for (const auto& signature : _signatures)
		{
			auto& type_to_map = signature.is_mapped_to_interface
				? signature.interface_type
				: signature.implementation_type;

			if (type == type_to_map && name == signature.name)
			{
				auto id = nomination_identity(type_to_map, name);
				auto search_result = _singletons.find(id);

				if (search_result != _singletons.end())
				{
					return search_result->second;
				}
				else
				{
					result = signature.singleton_creator();

					// Check one more time due to possible recursion
					search_result = _singletons.find(id);
					if (search_result == _singletons.end())
					{
						_singletons.insert(std::make_pair(id, result));
						break;
					}
				}
			}
		}

		return result;
	}
}
