# include "zx_metadata.h"

namespace zx 
{
	namespace meta
	{
		std::unordered_map<
			std::type_index,
			std::unordered_map<
			std::string,
			field>> registry::__fields;

		void registry::add_field(field field)
		{
			const auto ownerTypeIndex = field.get_owner_type().get_index();
			__fields[ownerTypeIndex][field.get_name()] = field;
		}

		field registry::get_field(type type,
								  const std::string& name)
		{
			auto& fields = __fields[type.get_index()];
			const auto it = fields.find(name);
			if (it == fields.end())
			{
				throw exception(reason::field_not_found);
			}

			return it->second;
		}

		iterable::imm::unordered_map<std::string, field>
			registry::get_fields(type type)
		{
			auto& fields = __fields[type.get_index()];
			return fields;
		}

		void registry::for_each_field_of_type(
			type type,
			const std::function<void(field)>& routine)
		{
			auto& fields = __fields[type.get_index()];

			for (auto& field : fields)
			{
				routine(field.second);
			}
		}
	}
}
