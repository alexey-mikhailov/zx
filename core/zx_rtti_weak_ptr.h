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
				std::atomic<zx::ulong> riffs;
				std::atomic<zx::ulong> weaks;
			};

			struct stl_ref_cntr
			{
				void* vfptr;
				std::atomic<zx::ulong> riffs;
				std::atomic<zx::ulong> weaks;
			};

			ZX_API static weak_ptr empty;

		private:
			void*			_address;
			ref_cntr*		_ref_cntr;
			const zx::type*		_type;

		public:
			ZX_API weak_ptr();
			ZX_API weak_ptr(const weak_ptr& other);
			ZX_API weak_ptr(weak_ptr&& other);
			ZX_API ~weak_ptr();

			ZX_API weak_ptr& operator=(const weak_ptr& other);
			ZX_API weak_ptr& operator=(weak_ptr&& other);

			ZX_API const zx::type& get_type() const;
			ZX_API void* get() const;
			ZX_API unsigned long riff_count() const;
			ZX_API unsigned long weak_count() const;
			ZX_API explicit operator bool() const;

			template <typename T>
			static weak_ptr from(const std::shared_ptr<T>& shr)
			{
				weak_ptr result;
				result._type = &zx::type::i<T>();
				auto offset = (void**)const_cast<std::shared_ptr<T>*>(&shr);
				result._address = *offset++;

				// Offset one pointer right due to counter type vfptr. 
				offset = ((void**)*offset + 1);
				result._ref_cntr = (ref_cntr*)offset;

				// Increase weak counter as last as possible. 
				// It is non-existing weak reference in std::shared_ptr term. 
				// But we need it to keep reference counter data. 
				++result._ref_cntr->weaks;
				return result;
			}

			template <typename T>
			static std::shared_ptr<T> to_shared_ptr(const weak_ptr& ptr)
			{
				if (ptr._ref_cntr->riffs)
				{
					std::shared_ptr<T> result;
					auto offset = reinterpret_cast<void**>(&result);
					*offset = ptr._address;
					++offset;
					*offset = (void**)ptr._ref_cntr - 1;

					// Change ref counters as last as possible. 
					++ptr._ref_cntr->riffs;
					return result;
				}
				else
				{
					// We've lost data.
					// It's better to be honest and return it explicitly. 
					return std::shared_ptr<T>();
				}
			}

			ZX_API static void to_shared_ptr_unsafe(const weak_ptr& ptr,
													void** where_to);
		};
	}
}