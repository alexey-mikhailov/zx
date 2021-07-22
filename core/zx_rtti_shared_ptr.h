# pragma once
# include "zx_defines.h"
# include "zx_constants.h"
# include "zx_type.h"

namespace zx
{
	namespace rtti
	{
		/// <summary>
		/// Strong RTTI handle to STL shared pointer
		/// </summary>
		class shared_ptr final
		{
		public:
			struct ref_cntr
			{
				void* vfptr;
				std::atomic<zx::ulong> riffs;
				std::atomic<zx::ulong> weaks;
			};

			ZX_API static shared_ptr empty;

		private:
			void*			_address;
			ref_cntr*		_ref_cntr;
			const zx::type*	_type;
			ZX_API static std::map<const std::type_index, void(*)(void*)> __deleters;

		public:
			ZX_API shared_ptr();
			ZX_API shared_ptr(const shared_ptr& other);
			ZX_API shared_ptr(shared_ptr&& other);
			ZX_API ~shared_ptr();

			template <class T>
			shared_ptr(const std::shared_ptr<T>&);

			template <class T>
			shared_ptr(const std::shared_ptr<T>&&);

			template <class T>
			operator std::shared_ptr<T>() const;

			ZX_API shared_ptr& operator=(const shared_ptr& other);
			ZX_API shared_ptr& operator=(shared_ptr&& other);

			ZX_API zx::type get_type() const;
			ZX_API void* get() const;
			ZX_API unsigned long riff_count() const;
			ZX_API unsigned long weak_count() const;
			ZX_API explicit operator bool() const;

			ZX_API void write_to(void** where_to);

		private:
			/// <summary>
			/// Decrements ref counter. 
			/// </summary>
			/// <param name="type">Type of data inside. May be nullptr. </param>
			/// <param name="ref_cntr">Reference counter. May be nullptr. </param>
			/// <param name="address">Address to data. May be nullptr. </param>
			static void decrement(const type* type, 
								  ref_cntr* ref_cntr, 
								  void* address);
		};
		
		template<class T>
		shared_ptr::shared_ptr(const std::shared_ptr<T>& tmpl)
		{
			_type = &zx::type::i<T>();

			auto offset = (void**)const_cast<std::shared_ptr<T>*>(&tmpl);
			_address = *offset++;
			_ref_cntr = (ref_cntr*)*offset;

			__deleters[_type->get_index()] = [](void* address)
			{
				// Delete in place,
				// because address allocated in stack. 
				auto casted = reinterpret_cast<T*>(address);
				casted->~T();
			};

			// Change ref counters as last as possible. 
			++_ref_cntr->riffs;
		}

		template<class T>
		shared_ptr::shared_ptr(const std::shared_ptr<T>&& tmpl)
		{
			_type = &zx::type::i<T>();

			auto offset = (void**)const_cast<std::shared_ptr<T>*>(&tmpl);
			_address = *offset++;
			_ref_cntr = (ref_cntr*)*offset;

			__deleters[_type->get_index()] = [](void* address)
			{
				// Delete in place,
				// because address allocated in stack. 
				auto casted = reinterpret_cast<T*>(address);
				casted->~T();
			};

			// Change ref counters as last as possible. 
			++_ref_cntr->riffs;
		}

		template<class T>
		shared_ptr::operator std::shared_ptr<T>() const
		{
			if (_ref_cntr && _ref_cntr->riffs)
			{
				std::shared_ptr<T> result;
				auto offset = reinterpret_cast<void**>(&result);
				*offset = _address;
				++offset;
				*offset = _ref_cntr;

				// Change ref counters as last as possible. 
				++_ref_cntr->riffs;
				return result;
			}
			else
			{
				// We've lost data.
				// It's better to be honest and return it explicitly. 
				return std::shared_ptr<T>();
			}
		}
	}
}