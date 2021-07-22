# include "zx_rtti_shared_ptr.h"

namespace zx
{
	namespace rtti
	{
		shared_ptr shared_ptr::empty;
		std::map<const std::type_index, void(*)(void*)> shared_ptr::__deleters;

		shared_ptr::shared_ptr() :
			_address(nullptr),
			_ref_cntr(nullptr),
			_type(nullptr)
		{
		}

		shared_ptr::shared_ptr(const shared_ptr& other) :
			_address(other._address),
			_ref_cntr(other._ref_cntr),
			_type(other._type)
		{
			if (_ref_cntr)
			{
				++_ref_cntr->riffs;
			}
		}

		shared_ptr::shared_ptr(shared_ptr&& other) :
			_address(std::exchange(other._address, nullptr)),
			_ref_cntr(std::exchange(other._ref_cntr, nullptr)),
			_type(std::exchange(other._type, nullptr))
		{
		}

		shared_ptr& shared_ptr::operator=(const shared_ptr& other)
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
					++_ref_cntr->riffs;
				}
			}

			return *this;
		}

		shared_ptr& shared_ptr::operator=(shared_ptr&& other)
		{
			if (this == &other)
			{
				return *this;
			}

			_address = std::exchange(other._address, nullptr);
			_ref_cntr = std::exchange(other._ref_cntr, nullptr);
			_type = std::exchange(other._type, nullptr);

			return *this;
		}

		shared_ptr::~shared_ptr()
		{
			decrement(_type, _ref_cntr, _address);
		}

        zx::type shared_ptr::get_type() const
        {
			return *_type;
        }

        void* shared_ptr::get() const
		{
			return _address;
		}

		unsigned long shared_ptr::riff_count() const
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

		unsigned long shared_ptr::weak_count() const
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

		shared_ptr::operator bool() const
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

		void shared_ptr::write_to(void** where_to)
		{
			auto target_address = where_to;
			auto target_ref_cntr = reinterpret_cast<ref_cntr**>(where_to + 1);
			decrement(nullptr, *target_ref_cntr, *target_address);

			if (_ref_cntr && _ref_cntr->riffs)
			{
				*target_address = _address;
				*target_ref_cntr = _ref_cntr;

				// Change ref counters as last as possible. 
				++_ref_cntr->riffs;
			}
			else
			{
				// We've lost data.
				// It's better to be honest and return it explicitly. 
				*target_address = nullptr;
				*target_ref_cntr = nullptr;
			}
		}

		void shared_ptr::decrement(const type* type, 
								   ref_cntr* ref_cntr, 
								   void* address)
		{
			if (ref_cntr)
			{
				--ref_cntr->riffs;

				if (ref_cntr->riffs == zx::zero::ulong)
				{
					// Must perform delete in place. 
					// type = 0 when target is STL shared pointer. 
					// In this case delete in place will be deferred 
					// till reference counter still exist. 
					if (type)
					{
						__deleters[type->get_index()](address);
					}

					--ref_cntr->weaks;

					if (ref_cntr->weaks == zx::zero::ulong)
					{
						// Vfptr have to call virtual dtor
						// of inherited ref counter type. 
						// So, resource at _address will be deallocated here. 
						delete ref_cntr;
					}
				}
			}
		}
	}
}
