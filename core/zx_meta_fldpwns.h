# pragma once
# include "zx_defines.h"
# include "zx_enums.h"

namespace zx
{
	namespace meta
	{
		class fieldpawn
		{
			fieldpawn_type _type;

		public:
			ZX_API explicit fieldpawn(fieldpawn_type type);
			ZX_API explicit fieldpawn(const fieldpawn&) = default;
			ZX_API explicit fieldpawn(fieldpawn&& other);
			ZX_API virtual ~fieldpawn() = default;

			ZX_API fieldpawn_type get_type() const;
		};

		class named_fieldpawn final : public fieldpawn
		{
		public:
			ZX_API explicit named_fieldpawn(std::string name);
			ZX_API explicit named_fieldpawn(const named_fieldpawn& other) = default;
			ZX_API explicit named_fieldpawn(named_fieldpawn&& other);
			ZX_API std::string get_name() const
			{
				return _name;
			}

		private:
			std::string				_name;
		};

		using fieldpawn_unqptr = std::unique_ptr<fieldpawn>;
		using named_fieldpawn_unqptr = std::unique_ptr<named_fieldpawn>;
	}
}