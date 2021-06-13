# include "singleton_container.h"
# include "di_container.h"

namespace zx
{
	singleton_container::singleton_container(const di_container *owner) :
		_owner(owner)
	{
	}

	void* singleton_container::get_or_add(const type &type)
	{
		void *result = nullptr;

		for (const auto& signature : _signatures)
		{
			auto type_to_map = signature.is_mapped_to_interface
				? signature.interface_type
				: signature.implementation_type;

			if (type == type_to_map)
			{
				auto search_result = _singletons.find(type_to_map);

				if (search_result != _singletons.end())
				{
					return search_result->second;
				}
				else
				{
					result = signature.implementation_type.instantiate();

					// Check one more time due to possible recursion
					search_result = _singletons.find(type_to_map);
					if (search_result == _singletons.end())
					{
						_singletons.insert(std::make_pair(type_to_map, result));
						break;
					}
				}
			}
		}

		return result;
	}
}