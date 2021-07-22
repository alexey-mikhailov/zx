# include "zx_meta_fldpwns.h"

namespace zx
{
	namespace meta
	{
		fieldpawn::fieldpawn(fieldpawn_type type) :
			_type(type)
		{
		}

		fieldpawn::fieldpawn(fieldpawn&& other) :
			_type(std::exchange(other._type, fieldpawn_type::none))
		{
		}

		fieldpawn_type fieldpawn::get_type() const
		{
			return _type;
		}

		named_fieldpawn::named_fieldpawn(std::string name) :
			fieldpawn(fieldpawn_type::nameleton),
			_name(std::move(name))
		{
		}

		named_fieldpawn::named_fieldpawn(named_fieldpawn && other) :
			fieldpawn(std::move(other)),
			_name(std::move(other._name))
		{
		}
	}
}