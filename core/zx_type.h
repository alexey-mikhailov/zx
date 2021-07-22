# pragma once
# include "zx_defines.h"
# include "zx_exceptions.h"
# include "zx_tmpl_args_converter.h"
# include "zx_rtti_args.h"

namespace zx
{
	struct type_data
	{
		template <class T>
		static type_data create()
		{
			auto result = type_data();
			result.index = typeid(T);
			result.name = typeid(T).name();
			result.is_pointer = std::is_pointer<T>::value;
			result.is_abstract = std::is_abstract<T>::value;
			result.is_default_constructible = std::is_default_constructible<T>::value;
			return result;
		}

		std::type_index index = typeid(nullptr);
		std::string name;
		std::unordered_map<rtti::args, fnptr> factory_methods;
		bool is_pointer;
		bool is_abstract;
		bool is_default_constructible;
	};


	class type final
	{
		std::shared_ptr<type_data> _data;
		ZX_API static std::map<std::type_index, zx::type *> __instances;

	public:
		ZX_API static zx::type null;

		ZX_API type(const zx::type& other);

		// Properties
		std::type_index get_index() const { return _data->index; }
		const std::string& get_name() const { return _data->name; }
		bool is_pointer() const {return _data->is_pointer; }
		bool is_abstract() const { return _data->is_abstract; }
		bool is_default_constructible() const { return _data->is_default_constructible; }
		
		template <class T>
		static const type& i();

		template <typename T, typename ReflCtor>
		static const type& ensure();

		template <class... Args>
		void* instantiate(Args... args) const;

		ZX_API bool operator ==(const type& other) const;
		ZX_API bool operator <(const type& other) const;
		ZX_API bool operator >(const type& other) const;

	private:
		ZX_API type(type_data data);
	};

	template <class T>
	static const type& type::i()
	{
		const auto it = __instances.find(typeid(T));
		if (it == __instances.end())
		{
			auto value = new type(type_data::create<T>());
			__instances[typeid(T)] = value;

			if constexpr (std::is_default_constructible_v<T>)
			{
				const rtti::args arg_types = rtti::args::empty;

				auto& fms = value->_data->factory_methods;
				const auto fit = fms.find(arg_types);

				if (fit == fms.end())
				{
					void* (*temp)() =
						[]() -> void*
					{
						return new T();
					};

					fms[arg_types] = reinterpret_cast<fnptr>(temp);
				}
			}

			return *value;
		}

		return *it->second;
	}

	template <typename T, typename ReflCtor>
	static const type& type::ensure()
	{
		auto it = __instances.find(typeid(T));
		if (it == __instances.end())
		{
			auto value = new type(type_data::create<T>());
			__instances[typeid(T)] = value;
		}

		it = __instances.find(typeid(T));
		auto& value = *it->second;

		auto arg_types = ReflCtor::make_args();

		auto& fms = value._data->factory_methods;
		const auto fit = fms.find(arg_types);

		if (fit == fms.end())
		{
			fms[arg_types] = ReflCtor::make_fnptr();
		}
		return value;
	}

	template <class... Args>
	void* type::instantiate(Args... args) const
	{
		const rtti::args arg_types = tmpl_args_converter<Args...>::to_rtti();
		for (auto& fm : _data->factory_methods)
		{
			if (arg_types == fm.first)
			{
				auto temp = reinterpret_cast<void* (*)(Args ...)>(fm.second);
				return temp(args...);
			}
		}

		throw zx::exception(zx::reason::no_factory_method);
	}
}

