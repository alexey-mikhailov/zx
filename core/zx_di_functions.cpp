# include "zx_di_functions.h"
# include "zx_metadata.h"
# include "zx_di_container.h"
# include "zx_inject_data.h"
# include "zx_singleton_container.h"
# include "zx_nomination_container.h"
# include "zx_it_range.h"

namespace zx
{
	using namespace meta;

	void inject_field(void* injectee,
					  field field,
					  iterable::imm::unordered_map<
							  std::string,
							  di_container*> di_containers)
	{
		if (field.get_pawn_type() == fieldpawn_type::singleton || 
			field.get_pawn_type() == fieldpawn_type::signal_pack)
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
		else if (field.get_pawn_type() == fieldpawn_type::named_fieldpawn)
		{
			const fieldpawn& pawn = field.get_pawn();
			auto named_fieldpawn = dynamic_cast<const meta::named_fieldpawn*>(&pawn);

			if (named_fieldpawn)
			{
				for (auto&& [name, container] : di_containers)
				{
					auto instance =
						container->nomination_container->get_or_add(
							field.get_type(),
							named_fieldpawn->get_name());

					auto field_address = reinterpret_cast<void**>
					(
						static_cast<__int8*>(injectee) +
						field.get_offset()
					);

					instance.write_to(field_address);
				}
			}
			else
			{
				throw zx::exception(reason::named_instance_expected);
			}
		}
	}

	void inject_dependencies(const type& type, void* target)
	{
		iterable::imm::unordered_map<std::string, di_container*>
			di_containers = di_container::__instances;

		const auto fields = meta::registry::get_fields(type);
		for (auto&& [name, field] : fields)
		{
			if (field.get_pawn_type() != fieldpawn_type::none)
			{
				if (field.get_expose_type() == expose_type::shrptr)
				{
					inject_field(target, field, di_containers);
				}
				else
				{
					throw exception(reason::field_is_not_shrptr);
				}
			}
		}
	}
	
	std::unique_ptr<named_fieldpawn> make_named_fieldpawn(char const* name)
	{
		return std::make_unique<named_fieldpawn>(name);
	}

	std::unique_ptr<named_fieldpawn> make_named_fieldpawn(std::string name)
	{
		return std::make_unique<named_fieldpawn>(std::move(name));
	}

	std::unique_ptr<named_fieldpawn> make_named_fieldpawn(std::string&& name)
	{
		return std::make_unique<named_fieldpawn>(std::move(name));
	}
}