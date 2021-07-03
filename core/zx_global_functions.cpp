# include "zx_global_functions.h"
# include "zx_metadata.h"
# include "di_container.h"
# include "singleton_container.h"
# include "nomination_container.h"
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
				const auto instance =
					container->singleton_container->get_or_add(field.type);

				memcpy(static_cast<__int8*>(injectee) + field.offset,
					   &instance,
					   field.size);
			}
		}
		else if (field.inject_type == inject_type::named_instance)
		{
			const inject_data& inject_data = field.inject_data;
			auto named_instance = dynamic_cast<const zx::named_instance&>(inject_data);

			for (auto it = di_containers.begin; it != di_containers.end; ++it)
			{
				auto container = it->second;
				const auto instance =
					container->nomination_container->get_or_add(
						field.type,
						named_instance.get_name());

				memcpy(static_cast<__int8*>(injectee) + field.offset,
					   &instance,
					   field.size);
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
				if (field.expose_type == expose_type::rawptr)
				{
					inject_field(target, field, di_containers);
				}
				else
				{
					throw std::logic_error("Field to inject is not rawptr. ");
				}
			}
		}
	}
}