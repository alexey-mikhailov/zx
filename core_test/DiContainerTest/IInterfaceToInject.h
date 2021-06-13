#pragma once

struct IInterfaceToInject
{
	virtual ~IInterfaceToInject() = default;
	virtual int SomeMethod() = 0;
};

