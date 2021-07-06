#pragma once

struct interface_to_inject
{
	virtual ~interface_to_inject() = default;
	virtual int some_method() = 0;
};

