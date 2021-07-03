# pragma once
# include "zx_type.h"
# include "inject_data.h"
# include "zx_field.h"
# include "zx_it_range.h"

namespace zx
{
	class metadata final
	{
		static std::unordered_map<
			std::type_index, 
			std::unordered_map<
				std::string, 
				field>> __fields;

	public:
		template <class Data, class Owner>
		static void add_field(const std::string& name,
							  Data Owner::* member,
							  inject_type inject_type = inject_type::none)
		{
			add_field(field::create(name, member, inject_type));
		}

		template <class Data, class Owner>
		static void add_field(const std::string& name,
							  Data Owner::* member,
							  named_instance* inject_data)
		{
			add_field(field::create(name, member, inject_data));
		}

		ZX_API static field get_field(const type& type,
							   const std::string& name);

		ZX_API 
		static it_range<std::unordered_map<std::string, field>::iterator> 
		get_fields(const type& type);

		ZX_API
		static void for_each_field_of_type(
			const type& type,
			const std::function<void(const field&)>& routine);

	private:
		ZX_API static void add_field(const field& field);
	};
}
