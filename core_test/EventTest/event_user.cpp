# include "event_user.h"


event_user::event_user()
{
	_copyable_resource = static_cast<zx::u64>(rand());
}

event_user::event_user(const event_user& other) : 
	_copyable_resource(other._copyable_resource)
{
}

event_user::~event_user()
{
}

void event_user::do_something()
{
	_somethingComplete();
}

int event_user::get_last_result()
{
	return _readyToCalc();
}

void event_user::add(double a, double b)
{
	const auto result = a + b;
	_added(a, b, result);
}

std::tuple<const event_user&, zx::u32> event_user::request_identity()
{
	size_t counter = 0ui64;
	const event_user* result = nullptr;
	auto& delegates = _identity_requested.get_delegates();
	for (auto&& [hash, del] : delegates)
	{
		result = &del->invoke(*this);
		++counter;
	}

	// Ill-formed return value for simplification. 
	return { *result, counter };
}
