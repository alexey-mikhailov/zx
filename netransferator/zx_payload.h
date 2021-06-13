# pragma once

namespace zx
{
	struct datarange final
	{
		int size;
		byte* data;

		datarange(int size, byte* data) :
			size(size),
			data(data)
		{}

		const byte* size_ptr() const { return reinterpret_cast<const byte*>(&size); }
	};


	class weak_payload final
	{
		int	_size = 0;
		byte* _data = nullptr;

	public:
		weak_payload() = default;

		explicit weak_payload(int size, byte* data) :
			_size(size),
			_data(data)
		{
		}

		weak_payload(const weak_payload& other) = default;

		weak_payload(weak_payload&& other) noexcept :
			_size(other._size),
			_data(other._data)
		{}

		~weak_payload() = default;

		weak_payload& operator=(const weak_payload& other) = default;
		weak_payload& operator=(weak_payload&& other) noexcept = default;

		int size() const { return _size; }
		const byte* size_ptr() const { return reinterpret_cast<const byte*>(&_size); }
		byte* data() const { return _data; }
	};


	class strong_payload final
	{
		int _size = 0;
		byte* _data = nullptr;

	public:
		static std::shared_ptr<strong_payload> create_shared(int size)
		{
			return std::make_shared<strong_payload>(size);
		}

		static std::shared_ptr<strong_payload> create_shared(int size, void* source_to_copy)
		{
			auto ptr = std::make_shared<strong_payload>(size);
			memcpy(ptr->data(), source_to_copy, ptr->size());
			return ptr;
		}

		strong_payload() = default;

		explicit strong_payload(int size) :
			_size(size),
			_data(new byte[size])
		{
		}

		~strong_payload()
		{
			delete[] _data;
		}

		strong_payload(const strong_payload& other) :
			_size(other._size)
		{
			_data = new byte[_size];
			memcpy(_data, other._data, _size);
		}

		strong_payload(strong_payload&& other) noexcept :
			_size(other._size)
		{
			delete[] _data;
			_data = other._data;

			other._data = nullptr;
			other._size = 0;
		}

		strong_payload& operator=(const strong_payload& other)
		{
			_size = other._size;

			delete[] _data;
			_data = new byte[_size];
			memcpy(_data, other._data, _size);

			return *this;
		}

		strong_payload& operator=(strong_payload&& other) noexcept
		{
			_size = other._size;
			other._size = 0;

			delete[] _data;
			_data = other._data;
			other._data = nullptr;

			return *this;
		}

		void resize(int size)
		{
			delete[] _data;

			_size = size;
			_data = new byte[size];
		}

		int size() const { return _size; }
		const byte* size_ptr() const { return reinterpret_cast<const byte*>(&_size); }
		byte* data() const { return _data; }
	};
}
