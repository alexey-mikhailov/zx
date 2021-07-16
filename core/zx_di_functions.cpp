# include "zx_di_functions.h"
# include "zx_metadata.h"
# include "zx_di_container.h"
# include "zx_singleton_container.h"
# include "zx_nomination_container.h"
# include "zx_it_range.h"

namespace zx
{
	void inject_field(void* injectee,
					  const field& field,
					  it_range<
						  std::unordered_map<
							  std::string,
							  di_container*>::iterator> di_containers)
	{
		if (field.inject_type == inject_type::singleton || 
			field.inject_type == inject_type::signal_pack)
		{
			for (auto it = di_containers.begin; it != di_containers.end; ++it)
			{
				auto container = it->second;
				auto instance =
					container->singleton_container->get_or_add(field.type);

				auto field_address = 
					reinterpret_cast<void**>(
						static_cast<__int8*>(injectee) + field.offset);

				instance.write_to(field_address);
			}
		}
		else if (field.inject_type == inject_type::named_instance)
		{
			const inject_data& inject_data = field.inject_data;
			auto named_instance = dynamic_cast<const zx::named_instance&>(inject_data);

			for (auto it = di_containers.begin; it != di_containers.end; ++it)
			{
				auto container = it->second;
				auto instance =
					container->nomination_container->get_or_add(
						field.type,
						named_instance.get_name());

				auto field_address =
					reinterpret_cast<void**>(
						static_cast<__int8*>(injectee) + field.offset);

				instance.write_to(field_address);
			}
		}
	}

	void inject_dependencies(const type& type, void* target)
	{
		const it_range<
			std::unordered_map<
			std::string,
			di_container*>::iterator> di_containers =
		{
			di_container::__instances.begin(),
			di_container::__instances.end()
		};

		const auto fields = metadata::get_fields(type);
		for (auto it = fields.begin; it != fields.end; ++it)
		{
			auto field = it->second;
			if (field.inject_type != inject_type::none)
			{
				if (field.expose_type == expose_type::shrptr)
				{
					inject_field(target, field, di_containers);
				}
				else
				{
					throw std::logic_error("Field to inject is not shrptr. ");
				}
			}
		}
	}
}