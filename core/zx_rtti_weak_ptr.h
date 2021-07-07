# pragma once
# include "zx_defines.h"

namespace zx
{
	namespace rtti
	{
		/// <summary>
		/// Reflected weak pointer to shared pointer
		/// </summary>
		class weak_ptr final
		{
		public:
			struct ref_cntr
			{
				unsigned long strongs;
				unsigned long weaks;
			};

			ZX_API static weak_ptr empty;

		private:
			void* address;
			ref_cntr* ref_cntr;

		public:
			ZX_API weak_ptr();
			ZX_API weak_ptr(const weak_ptr& other);
			ZX_API weak_ptr(weak_ptr&& other);
			ZX_API ~weak_ptr();

			ZX_API weak_ptr& operator=(const weak_ptr& other);
			ZX_API weak_ptr& operator=(weak_ptr&& other);

			ZX_API void* get() const;
			ZX_API unsigned long strn_count() const;
			ZX_API unsigned long weak_count() const;
			ZX_API explicit operator bool() const;

			template <typename T>
			static weak_ptr from(const std::shared_ptr<T>& shr)
			{
				weak_ptr result;
				auto offset = (void**)const_cast<std::shared_ptr<T>*>(&shr);
				result.address = *offset++;

				// Offset one pointer right due to counter type vfptr. 
				offset = ((void**)*offset + 1);
				result.ref_cntr = (struct ref_cntr*)offset;

				// Increase weak counter as last as possible. 
				// It is non-existing weak reference in std::shared_ptr term. 
				// But we need it to keep reference counter data. 
				++result.ref_cntr->weaks;
				return result;
			}

			template <typename T>
			static std::shared_ptr<T> to_shared_ptr(const weak_ptr& ptr)
			{
				if (ptr.ref_cntr->strongs)
				{
					std::shared_ptr<T> result;
					auto offset = reinterpret_cast<void**>(&result);
					*offset = ptr.address;
					++offset;
					*offset = (void**)ptr.ref_cntr - 1;

					// Change ref counters as last as possible. 
					++ptr.ref_cntr->strongs;
					++ptr.ref_cntr->weaks;
					return result;
				}
				else
				{
					// We've lost data.
					// It's better to be honest and return it explicitly. 
					return std::shared_ptr<T>();
				}
			}
		};
	}
}