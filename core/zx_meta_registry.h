# pragma once
# include "zx_type.h"
# include "zx_iterables.h"
# include "zx_meta_field.h"
# include "zx_meta_fldpwns.h"

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
			/// <summary>
			/// Adds field with value type inside. 
			/// Will not be affected by DI. 
			/// </summary>
			/// <typeparam name="Data">Field value type. </typeparam>
			/// <typeparam name="Owner">Owner type. </typeparam>
			/// <param name="name">Field name. </param>
			/// <param name="member">Pointer to member. </param>
			template <class Data, class Owner>
			static void add_field(std::string name,
								  Data Owner::* member)
			{
				auto& owner_type = type::i<Owner>();
				auto& type = type::i<typename std::remove_pointer_t<Data>>();
				auto offset = reinterpret_cast<zx::u64>(&(static_cast<Owner *>(nullptr)->*member));
				add_field(field::create_value(std::move(name),
											  owner_type,
											  type,
											  zx::u32(offset)));
			}

			/// <summary>
			/// Adds field with shared pointer pawn type inside. 
			/// Will be affected by DI. 
			/// </summary>
			/// <typeparam name="Data">Field value type. </typeparam>
			/// <typeparam name="Owner">Owner type. </typeparam>
			/// <param name="name">Field name. </param>
			/// <param name="member">Pointer to member. </param>
			/// <param name="pawn">Fieldpawn metadata. </param>
			template <class Data, class Owner>
			static void add_field(std::string name,
								  Data Owner::* member,
								  fieldpawn_unqptr pawn)
			{
				auto& owner_type = type::i<Owner>();
				auto& type = type::i<typename std::remove_pointer_t<Data>>();
				auto offset = reinterpret_cast<zx::u64>(&(static_cast<Owner *>(nullptr)->*member));
				add_field(field::create_shrptr(std::move(name),
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
