# include "zx_metadata.h"

namespace zx 
{
	std::unordered_map<
		std::type_index, 
		std::unordered_map<
			std::string, 
			field>> metadata::__fields;

	void metadata::add_field(const field& field)
	{
		const auto ownerTypeIndex = field.get_owner_type().index;
		__fields[ownerTypeIndex][field.get_name()] = field;
	}

	field metadata::get_field(const type& type, 
							  const std::string& name)
	{
		auto& fields = __fields[type.index];
		const auto it = fields.find(name);
		if (it == fields.end())
		{
			throw std::logic_error("Field not found. ");
		}

		return it->second;
	}

	iterable::imm::unordered_map<std::string, field>
	metadata::get_fields(const type& type)
	{
		auto& fields = __fields[type.index];
		return fields;
	}

	void metadata::for_each_field_of_type(
		const type& type, 
		const std::function<void(const field&)>& routine)
	{
		auto& fields = __fields[type.index];

		for (auto& field : fields)
		{
			routine(field.second);
		}
	}
}
