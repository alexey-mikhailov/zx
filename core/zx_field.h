# pragma once
# include "zx_type.h"
# include "zx_inject_data.h"
# include "properties/zx_ref_formula.h"
# include "zx_enums.h"

namespace zx
{
	struct field_data
	{
		const type&					owner_type;
		const type&					type;
		std::string					name;
		unsigned					offset;
		unsigned					size;
		expose_type					expose_type;
		inject_type					inject_type;
		std::unique_ptr<inject_data> inject_data;

		field_data(const zx::type& owner_type,
		           const zx::type& type,
				   std::string name,
				   unsigned offset,
				   unsigned size,
				   zx::expose_type expose_type,
				   zx::inject_type inject_type,
				   zx::inject_data* inject_data) :
			owner_type(owner_type),
			type(type),
			name(std::move(name)),
			offset(offset),
			size(size),
			expose_type(expose_type),
			inject_type(inject_type),
			inject_data(inject_data) {}
	};

	class field final
	{
		field_data* _data;

	public:
		ref_formula<field, const type> owner_type =
		{
			this, 
			[](field& field) -> const zx::type&
			{
				return field._data->owner_type;
			}
		};

		ref_formula<field, const type> type =
		{
			this, 
			[](field& field) -> const zx::type&
			{
				return field._data->type;
			}
		};

		const std::string& name = _data->name;
		const unsigned& offset = _data->offset;
		const unsigned& size = _data->size;
		const expose_type& expose_type = _data->expose_type;
		const inject_type& inject_type = _data->inject_type;
		
		ref_formula<field, inject_data> inject_data =
		{
			this,
			[](field& field) -> zx::inject_data&
			{
				return *field._data->inject_data;
			}
		};

		field() : _data(nullptr) {}
		field(const field& other) : _data(other._data) {}
		field(const field&& other) noexcept : _data(other._data) {}
		~field() = default;
		field& operator=(const field& other);
		field& operator=(const field&& other);

	private:
		ZX_API field(field_data* data) : _data(data) {}

		friend class metadata;
		template <class Owner, class Data>
		static field create(const std::string& name,
							Data Owner::* member,
							class named_instance* inject_data = nullptr);

		friend class metadata;
		template <class Owner, class Data>
		static field create(const std::string& name,
							Data Owner::* member,
							zx::inject_type inject_type);
	};

	template <class Owner, class Data>
	field field::create(const std::string &name,
						Data Owner::* member,
						named_instance* inject_data)
	{
		static_assert(
			std::is_default_constructible<Data>::value,
			"Only default constructible field type supported. ");

		static_assert(
			std::is_default_constructible<Owner>::value,
			"Only default constructible field owner supported. ");

		auto& owner_type = type::i<Owner>();

		auto expose_type = expose_type::shrptr;

		auto& type = type::i<typename std::remove_pointer<Data>::type>();

		auto offset = reinterpret_cast<size_t>(&(static_cast<Owner *>(nullptr)->*member));

		auto size = sizeof(typename std::remove_pointer<Data>::type);

		auto data = new field_data
		{
			owner_type,
			type,
			name,
			static_cast<unsigned>(offset),
			static_cast<unsigned>(size),
			expose_type,
			inject_type::named_instance,
			inject_data
		};
		
		return field(data);
	}

	template <class Owner, class Data>
	field field::create(const std::string &name,
						Data Owner::* member,
						zx::inject_type inject_type)
	{
		static_assert(
			std::is_default_constructible<Data>::value,
			"Only default constructible field type supported. ");

		static_assert(
			std::is_default_constructible<Owner>::value,
			"Only default constructible field owner supported. ");

		auto& owner_type = type::i<Owner>();

		auto expose_type = expose_type::value;
		if (inject_type == inject_type::none)
		{
			if (std::is_pointer<Data>::value)
			{
				expose_type = expose_type::rawptr;
			}
		}
		else
		{
			expose_type = expose_type::shrptr;
		}

		auto& type = type::i<typename std::remove_pointer<Data>::type>();

		auto offset = reinterpret_cast<size_t>(&(static_cast<Owner *>(nullptr)->*member));

		auto size = sizeof(typename std::remove_pointer<Data>::type);

		auto data = new field_data
		{
			owner_type,
			type,
			name,
			static_cast<unsigned>(offset),
			static_cast<unsigned>(size),
			expose_type,
			inject_type,
			nullptr
		};
		
		return field(data);
	}
}
