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
		ZX_API static std::map<std::type_index, type *> __instances;

	public:
		ZX_API static type null;

		// Properties
		const std::type_index& index = _data->index;
		const std::string& name = _data->name;
		const bool& is_pointer = _data->is_pointer;
		const bool& is_abstract = _data->is_abstract;
		
		ZX_API type(const type &other);

		/// Gets instance of abstract type. 
		template <class T, class... Args>
		static typename std::enable_if<
			std::is_abstract<T>::value, const type &>::type
		i()
		{
			const auto it = __instances.find(typeid(T));
			if (it == __instances.end())
			{
				auto value = new type(type_data::create<T>());
				__instances[typeid(T)] = value;
				return *value;
			}

			return *it->second;
		}

		/// Gets instance of concrete type. 
		template <class T, class... Args>
		static typename	std::enable_if<
			!std::is_abstract<T>::value, const type &>::type
		i()
		{
			const auto it = __instances.find(typeid(T));
			if (it == __instances.end())
			{
				auto data = type_data::create<T>();
				auto& fms = data->factory_methods;
				const rtti_args arg_types = rtti_pack<Args...>::get();
				const auto fit = fms.find(arg_types);

				if (fit == fms.end())
				{
					void* (*temp)(Args...) = 
						[](Args... args) -> void*
					{
						return new T(args...);
					};
					fms[arg_types] = reinterpret_cast<fnptr>(temp);
				}

				auto value = new type(data);

				__instances[typeid(T)] = value;
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

			throw std::logic_error("No factory method during create_instance. ");
		}

		ZX_API bool operator ==(const type& other) const;
		ZX_API bool operator <(const type &other) const;
		ZX_API bool operator >(const type &other) const;

	private:
		ZX_API type(type_data *data);
	};

	template <class Signature>
	class reflect;

	template <class Ret, class... Args>
	class reflect<Ret(Args...)>
	{

	public:
		type operator()() const
		{
			return type::i<Ret, Args...>();
		}

		operator type() const
		{
			return type::i<Ret, Args...>();
		}
	};
}