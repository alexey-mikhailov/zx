# pragma once

struct interface_to_inject
{
	virtual ~interface_to_inject() = default;
	virtual int some_method() = 0;
};

class type_to_inject_base
{
public:
	type_to_inject_base() = default;

private:
	int _field1 = 1234;
};

class type_to_inject : public type_to_inject_base, public interface_to_inject
{
public:
	type_to_inject() = default;

	int some_method() override;

	int public_field;
};


