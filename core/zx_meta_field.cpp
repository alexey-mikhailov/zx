# include "zx_meta_field.h"

namespace zx
{
	namespace meta
	{
		type field::get_owner_type() const
		{
			return _data->owner_type;
		}
		
		type field::get_type() const
		{
			return _data->type;
		}

		const std::string& field::get_name() const
		{
			return _data->name;
		}

		unsigned field::get_offset() const
		{
			return _data->offset;
		}

		expose_type field::get_expose_type() const
		{
			return _data->expose_type;
		}

		fieldpawn_type field::get_pawn_type() const
		{
			return _data->pawn->get_type();
		}

		const fieldpawn& field::get_pawn() const
		{
			return *_data->pawn;
		}
		
		field::field(field_data data) :
			_data(std::make_shared<field_data>(std::move(data)))
		{
		}

		field field::create(std::string name,
							type owner_type,
							type type,
							zx::u32 offset,
							fieldpawn_unqptr pawn)
		{
			auto data = field_data
			{
				owner_type,
				type,
				std::move(name),
				static_cast<unsigned>(offset),
				expose_type::shrptr,
				std::move(pawn)
			};

			return field(std::move(data));
		}

		field field::create(std::string name,
							type owner_type,
							type type,
							zx::u32 offset,
							fieldpawn_type pawn_type)
		{
			auto data = field_data
			{
				owner_type,
				type,
				std::move(name),
				static_cast<unsigned>(offset),
				expose_type::shrptr,
				std::make_unique<fieldpawn>(pawn_type)
			};

			return field(std::move(data));
		}
	}
}
