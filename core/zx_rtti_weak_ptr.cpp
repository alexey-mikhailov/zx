# include "zx_rtti_weak_ptr.h"

namespace zx
{
	namespace rtti
	{
		weak_ptr weak_ptr::empty;

		weak_ptr::weak_ptr() :
			_address(nullptr),
			_ref_cntr(nullptr),
			_type(nullptr)
		{
		}

		weak_ptr::weak_ptr(const weak_ptr& other) :
			_address(other._address),
			_ref_cntr(other._ref_cntr),
			_type(other._type)
		{
			if (_ref_cntr)
			{
				++_ref_cntr->weaks;
			}
		}

		weak_ptr::weak_ptr(weak_ptr&& other) :
			_address(std::exchange(other._address, nullptr)),
			_ref_cntr(std::exchange(other._ref_cntr, nullptr)),
			_type(std::exchange(other._type, nullptr))
		{
		}

		weak_ptr& weak_ptr::operator=(const weak_ptr& other)
		{
			if (this == &other)
			{
				return *this;
			}

			if (_address != other._address &&
				_ref_cntr != other._ref_cntr)
			{
				_address = other._address;
				_ref_cntr = other._ref_cntr;
				_type = other._type;

				if (_ref_cntr)
				{
					++_ref_cntr->weaks;
				}
			}

			return *this;
		}

		weak_ptr& weak_ptr::operator=(weak_ptr&& other)
		{
			if (this == &other)
			{
				return *this;
			}

			_address =  std::exchange(other._address, nullptr);
			_ref_cntr = std::exchange(other._ref_cntr, nullptr);
			_type = std::exchange(other._type, nullptr);

			return *this;
		}

		weak_ptr::~weak_ptr()
		{
			if (_ref_cntr)
			{
				--_ref_cntr->weaks;

				if (_ref_cntr->weaks == zx::zero::ulong)
				{
					// Zero weak count is state 
					// when resource already
					// called delete in place. 
					// Because shared pointers
					// keep weak pointer equals at least one. 
					//
					// Vfptr have to call virtual dtor
					// of inherited ref counter type. 
					// So, resource at _address will be deallocated here. 
					auto stl_ref_cntr = reinterpret_cast<weak_ptr::stl_ref_cntr*>((void**)_ref_cntr - 1);
					delete stl_ref_cntr;
				}
			}
		}

		const zx::type& weak_ptr::get_type() const
		{
			return *_type;
		}

		void* weak_ptr::get() const
		{
			return _address;
		}

		unsigned long weak_ptr::riff_count() const
		{
			if (_ref_cntr)
			{
				return _ref_cntr->riffs;
			}
			else
			{
				return 0ul;
			}
		}

		unsigned long weak_ptr::weak_count() const
		{
			if (_ref_cntr)
			{
				return _ref_cntr->weaks;
			}
			else
			{
				return 0ul;
			}
		}

		weak_ptr::operator bool() const
		{
			if (_ref_cntr)
			{
				return _ref_cntr->riffs;
			}
			else
			{
				return false;
			}
		}

		void weak_ptr::to_shared_ptr_unsafe(const weak_ptr & ptr, void ** where_to)
		{
			if (ptr._ref_cntr->riffs)
			{
				auto offset = reinterpret_cast<void**>(where_to);
				*offset = ptr._address;
				++offset;
				*offset = (void**)ptr._ref_cntr - 1;

				// Change ref counters as last as possible. 
				++ptr._ref_cntr->riffs;
			}
			else
			{
				// We've lost data.
				// It's better to be honest and return it explicitly. 
				auto offset = reinterpret_cast<void**>(where_to);
				*offset = nullptr;
				++offset;
				*offset = nullptr;
			}
		}
	}
}
