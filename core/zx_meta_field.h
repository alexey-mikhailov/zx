# pragma once
# include "zx_enums.h"
# include "zx_constants.h"
# include "zx_type.h"
# include "zx_meta_fldpwns.h"
# include "properties/zx_ref_formula.h"

namespace zx
{
	namespace meta
	{
		struct field_data final
		{
			type						owner_type;
			type						type;
			std::string					name;
			zx::u32						offset;
			expose_type					expose_type;
			std::unique_ptr<fieldpawn>  pawn;

			field_data(zx::type owner_type,
					   zx::type type,
					   std::string name,
					   unsigned offset,
					   zx::meta::expose_type expose_type,
					   std::unique_ptr<zx::meta::fieldpawn> pawn) :
				owner_type(owner_type),
				type(type),
				name(std::move(name)),
				offset(offset),
				expose_type(expose_type),
				pawn(std::move(pawn))
			{
			}
		};

		class field final
		{
			std::shared_ptr<field_data> _data;

		public:
			field() = default;
			field(const field& other) = default;
			field(field&& other) noexcept = default;
			~field() = default;

			field& operator =(const field&) = default;
			field& operator =(field&&) = default;

			ZX_API type get_owner_type() const;
			ZX_API type get_type() const;
			ZX_API const std::string& get_name() const;
			ZX_API unsigned get_offset() const;
			ZX_API expose_type get_expose_type() const;
			ZX_API fieldpawn_type get_pawn_type() const;
			ZX_API const fieldpawn& get_pawn() const;

		private:
			field(field_data data);

			friend class registry;
			ZX_API static field create_value(std::string name,
											 type owner_type,
											 type type,
											 zx::u32 offset);

			friend class registry;
			ZX_API static field create_shrptr(std::string name,
											  type owner_type,
											  type type,
											  zx::u32 offset,
											  fieldpawn_unqptr pawn);
		};
	}
}
