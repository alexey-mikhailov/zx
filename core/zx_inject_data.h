# pragma once
# include "zx_defines.h"

namespace zx
{
	class inject_data
	{
	public:
		ZX_API explicit inject_data() = default;
		ZX_API virtual ~inject_data() = default;

		ZX_API virtual bool operator==(const inject_data& other) const;
		ZX_API virtual bool operator!=(const inject_data& other) const;
	};

	class named_instance final : public inject_data
	{
	public:
		ZX_API explicit named_instance(std::string name);

		std::string get_name() const { return _name; }

	private:
		std::string				_name;
	};
}