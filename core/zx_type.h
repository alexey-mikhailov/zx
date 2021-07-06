# pragma once
# include "zx_defines.h"
# include "rtti_pack.h"
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
					const rtti_args arg_types = rtti_pack<>::get();

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
			const auto it = __instances.find(typeid(T));
			if (it == __instances.end())
			{
				auto value = new type(type_data::create<T>());
				__instances[typeid(T)] = value;

				if constexpr (std::is_default_constructible_v<T>)
				{
					const rtti_args arg_types = rtti_pack<>::get();

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

		template <class... Args>
		void* instantiate(Args... args) const
		{
			const rtti_args arg_types = rtti_pack<Args...>::get();
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

	template<typename T>
	struct refl
	{
		template<typename... Sig>
		struct ctor;

		template<typename First, typename... Args>
		struct ctor <First, Args...>
		{
			static_assert(std::is_constructible_v<T, First, Args...>, "Type is not constructible from these args. ");

			static rtti_args make_args()
			{
				return rtti_pack<First, Args...>::get();
			}

			static fnptr make_fnptr()
			{
				void* (*temp)(First, Args...) =
					[](First first, Args... args) -> void*
				{
					return new T(first, args...);
				};

				return reinterpret_cast<fnptr>(temp);
			}

			static const zx::type& ensure()
			{
				return type::ensure<T, zx::refl<T>::template ctor<First, Args...>>();
			}
		};

		template<>
		struct ctor <>
		{
			static_assert(std::is_default_constructible_v<T>, "Type is not default constructible. ");

			static rtti_args make_args()
			{
				return rtti_pack<>::get();
			}

			static fnptr make_fnptr()
			{
				void* (*temp)() =
					[]() -> void*
				{
					return new T();
				};

				return reinterpret_cast<fnptr>(temp);
			}

			static const zx::type& ensure()
			{
				return type::ensure<T, zx::refl<T>::template ctor<>>();
			}
		};
	};
}

