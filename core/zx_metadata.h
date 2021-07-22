# pragma once
# include "zx_type.h"
# include "zx_it_range.h"
# include "zx_inject_data.h"
# include "zx_field.h"

namespace zx
{
	namespace meta
	{
		class registry final
		{
			static std::unordered_map<
				std::type_index,
				std::unordered_map<
				std::string,
				field>> __fields;

		public:
			template <class Data, class Owner>
			static void add_field(std::string name,
								  Data Owner::* member,
								  fieldpawn_type pawn_type = fieldpawn_type::none)
			{
				auto& owner_type = type::i<Owner>();
				auto& type = type::i<typename std::remove_pointer_t<Data>>();
				auto offset = reinterpret_cast<zx::u64>(&(static_cast<Owner *>(nullptr)->*member));
				add_field(field::create(std::move(name),
										owner_type,
										type,
										zx::u32(offset),
										pawn_type));
			}

			template <class Data, class Owner>
			static void add_field(std::string name,
								  Data Owner::* member,
								  fieldpawn_unqptr pawn)
			{
				auto& owner_type = type::i<Owner>();
				auto& type = type::i<typename std::remove_pointer_t<Data>>();
				auto offset = reinterpret_cast<zx::u64>(&(static_cast<Owner *>(nullptr)->*member));
				add_field(field::create(std::move(name),
										owner_type,
										type,
										zx::u32(offset),
										std::move(pawn)));
			}

			ZX_API static field get_field(type type,
										  const std::string& name);

			ZX_API static iterable::imm::unordered_map<std::string, field>
				get_fields(type type);

			ZX_API static void for_each_field_of_type(
				type type,
				const std::function<void(field)>& routine);

		private:
			ZX_API static void add_field(field field);
		};
	}
}
