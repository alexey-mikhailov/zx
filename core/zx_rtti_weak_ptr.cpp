# include "zx_rtti_weak_ptr.h"

namespace zx
{
	namespace rtti
	{
		weak_ptr weak_ptr::empty;

		weak_ptr::weak_ptr() :
			address(nullptr), 
			ref_cntr(nullptr)
		{
		}

		weak_ptr::weak_ptr(const weak_ptr& other) :
			address(other.address),
			ref_cntr(other.ref_cntr)
		{
			if (ref_cntr)
			{
				++ref_cntr->weaks;
			}
		}

		weak_ptr::weak_ptr(weak_ptr&& other) :
			address(std::exchange(other.address, nullptr)),
			ref_cntr(std::exchange(other.ref_cntr, nullptr))
		{
		}

		weak_ptr& weak_ptr::operator=(const weak_ptr& other)
		{
			if (this == &other)
			{
				return *this;
			}

			address = other.address;
			ref_cntr = other.ref_cntr;
			return *this;
		}

		weak_ptr& weak_ptr::operator=(weak_ptr&& other)
		{
			if (this == &other)
			{
				return *this;
			}

			address =  std::exchange(other.address, nullptr);
			ref_cntr = std::exchange(other.ref_cntr, nullptr);

			return *this;
		}

		weak_ptr::~weak_ptr()
		{
			if (ref_cntr)
			{
				--ref_cntr->weaks;
			}
		}

		void * weak_ptr::get() const
		{
			return address;
		}

		unsigned long weak_ptr::riff_count() const
		{
			if (ref_cntr)
			{
				return ref_cntr->riffs;
			}
			else
			{
				return 0ul;
			}
		}

		unsigned long weak_ptr::weak_count() const
		{
			if (ref_cntr)
			{
				return ref_cntr->weaks;
			}
			else
			{
				return 0ul;
			}
		}

		weak_ptr::operator bool() const
		{
			if (ref_cntr)
			{
				return ref_cntr->riffs;
			}
			else
			{
				return false;
			}
		}
	}
}
