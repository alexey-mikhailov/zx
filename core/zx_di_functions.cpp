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
					  iterable::imm::unordered_map<
							  std::string,
							  di_container*> di_containers)
	{
		if (field.get_inject_type() == inject_type::singleton || 
			field.get_inject_type() == inject_type::signal_pack)
		{
			for (auto&& [name, container] : di_containers)
			{
				auto instance =
					container->singleton_container->get_or_add(field.get_type());

				auto field_address = reinterpret_cast<void**>
				(
					static_cast<__int8*>(injectee) + 
					field.get_offset()
				);

				instance.write_to(field_address);
			}
		}
		else if (field.get_inject_type() == inject_type::named_instance)
		{
			const inject_data& inject_data = field.get_inject_data();
			auto named_instance = dynamic_cast<const zx::named_instance&>(inject_data);

			for (auto&& [name, container] : di_containers)
			{
				auto instance =
					container->nomination_container->get_or_add(
						field.get_type(),
						named_instance.get_name());

				auto field_address = reinterpret_cast<void**>
				(
					static_cast<__int8*>(injectee) + 
					field.get_offset()
				);

				instance.write_to(field_address);
			}
		}
	}

	void inject_dependencies(const type& type, void* target)
	{
		iterable::imm::unordered_map<std::string, di_container*>
			di_containers = di_container::__instances;

		const auto fields = metadata::get_fields(type);
		for (auto&& [name, field] : fields)
		{
			if (field.get_inject_type() != inject_type::none)
			{
				if (field.get_expose_type() == expose_type::shrptr)
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