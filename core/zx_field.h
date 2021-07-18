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
		field() : _data(nullptr) {}
		field(const field& other) : _data(other._data) {}
		field(const field&& other) noexcept : _data(other._data) {}
		~field() = default;
		field& operator=(const field& other);
		field& operator=(const field&& other);

		const type& get_owner_type() const { return _data->owner_type; }
		const type& get_type() const { return _data->type; }
		const std::string& get_name() const { return _data->name; }
		unsigned get_offset() const { return _data->offset; }
		unsigned get_size() const { return _data->size; }
		expose_type get_expose_type() const { return _data->expose_type; }
		const inject_type& get_inject_type() const { return _data->inject_type; }
		const inject_data& get_inject_data() const { return *_data->inject_data; }

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
