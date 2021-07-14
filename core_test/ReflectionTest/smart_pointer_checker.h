#pragma once

struct smart_pointer_checker
{
	struct ref_cntr
	{
		void* vfptr;
		unsigned long riffs;
		unsigned long weaks;
	};

	ref_cntr* counter{};
	void* address{};

	template <typename T, typename S>
	void look_at(const S& smart_ptr)
	{
		static_assert(false, "No template implementation");
	}

	template<typename T>
	void look_at(const std::shared_ptr<T>& smart_ptr)
	{
		auto offset = (void**)const_cast<std::shared_ptr<T>*>(&smart_ptr);
		address = *offset++;
		counter = (ref_cntr*)*offset;
	}
};