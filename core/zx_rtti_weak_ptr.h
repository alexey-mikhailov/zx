# pragma once
# include "zx_defines.h"
# include "zx_constants.h"
# include "zx_type.h"

namespace zx
{
	namespace rtti
	{
		/// <summary>
		/// Weak RTTI handle to STL shared pointer
		/// </summary>
		class weak_ptr final
		{
		public:
			struct ref_cntr
			{
				void* vfptr;
				std::atomic<zx::ulong> riffs;
				std::atomic<zx::ulong> weaks;
			};

			ZX_API static weak_ptr empty;

		private:
			void*			_address;
			ref_cntr*		_ref_cntr;
			const zx::type*	_type;

		public:
			ZX_API weak_ptr();
			ZX_API weak_ptr(const weak_ptr& other);
			ZX_API weak_ptr(weak_ptr&& other);
			ZX_API ~weak_ptr();

			template <class T>
			weak_ptr(const std::shared_ptr<T>& tmpl);

			template <class T>
			weak_ptr(std::shared_ptr<T>&& tmpl);

			template <class T>
			operator std::shared_ptr<T>() const;

			ZX_API weak_ptr& operator=(const weak_ptr& other);
			ZX_API weak_ptr& operator=(weak_ptr&& other);

			ZX_API const zx::type& get_type() const;
			ZX_API void* get() const;
			ZX_API unsigned long riff_count() const;
			ZX_API unsigned long weak_count() const;
			ZX_API explicit operator bool() const;

			ZX_API void write_to(void** tmpl_shr_ptr_addr);

		private:
			/// <summary>
			/// Decrements ref counter. 
			/// </summary>
			/// <param name="ref_cntr">Reference counter. May be nullptr. </param>
			/// <param name="address">Address to data. May be nullptr. </param>
			static void decrement(ref_cntr* ref_cntr,
								  void* address);
		};
		
		template<class T>
		weak_ptr::weak_ptr(const std::shared_ptr<T>& tmpl)
		{
			_type = &zx::type::i<T>();
			auto offset = (void**)const_cast<std::shared_ptr<T>*>(&tmpl);
			_address = *offset++;
			_ref_cntr = (ref_cntr*)*offset;

			// Increase weak counter as last as possible. 
			++_ref_cntr->weaks;
		}
		
		template<class T>
		weak_ptr::weak_ptr(std::shared_ptr<T>&& tmpl)
		{
			_type = &zx::type::i<T>();
			auto offset = (void**)const_cast<std::shared_ptr<T>*>(&tmpl);
			_address = *offset++;
			_ref_cntr = (ref_cntr*)*offset;

			// Increase weak counter as last as possible. 
			++_ref_cntr->weaks;
		}
		
		template<class T>
		weak_ptr::operator std::shared_ptr<T>() const
		{
			if (_ref_cntr->riffs)
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