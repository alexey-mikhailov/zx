# pragma once
# include "zx_defines.h"
# include "tmpl_args_converter.h"
# include "rtti_args.h"

namespace zx
{
	struct type_data
	{
		template <class T>
		static type_data* create()
		{
			auto result = new type_data();
			result->index = typeid(T);
			result->name = typeid(T).name();
			result->is_pointer = std::is_pointer<T>::value;
			result->is_abstract = std::is_abstract<T>::value;
			return result;
		}

		std::type_index index = typeid(nullptr);
		std::string name;
		std::unordered_map<rtti_args, fnptr> factory_methods;
		bool is_pointer;
		bool is_abstract;
	};


	class type
	{
		type_data *_data;
		ZX_API static std::map<std::type_index, zx::type *> __instances;

	public:
		ZX_API static zx::type null;

		// Properties
		const std::type_index& index = _data->index;
		const std::string& name = _data->name;
		const bool& is_pointer = _data->is_pointer;
		const bool& is_abstract = _data->is_abstract;
		
		ZX_API type(const zx::type &other);

		/// Gets instance of abstract type. 
		template <class T>
		static const zx::type& i()
		{
			const auto it = __instances.find(typeid(T));
			if (it == __instances.end())
			{
				auto value = new type(type_data::create<T>());
				__instances[typeid(T)] = value;

				if constexpr (std::is_default_constructible_v<T>)
				{
					const rtti_args arg_types = rtti_args::empty;

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
		static const type& ensure()
		{
			auto it = __instances.find(typeid(T));
			if (it == __instances.end())
			{
				auto value = new type(type_data::create<T>());
				__instances[typeid(T)] = value;
			}

			it = __instances.find(typeid(T));
			const auto& value = *it->second;

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
		void* instantiate(Args... args) const
		{
			const rtti_args arg_types = tmpl_args_converter<Args...>::to_rtti();
			for (auto& fm : _data->factory_methods)
			{
				if (arg_types == fm.first)
				{
					auto temp = reinterpret_cast<void* (*)(Args ...)>(fm.second);
					return temp(args...);
				}
			}

			throw std::logic_error("No factory method during instantiate call. ");
		}

		ZX_API bool operator ==(const type& other) const;
		ZX_API bool operator <(const type &other) const;
		ZX_API bool operator >(const type &other) const;

	private:
		ZX_API type(type_data *data);
	};
}

