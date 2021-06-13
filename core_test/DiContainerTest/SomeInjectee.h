# pragma once
# include "TypeToInject.h"
#include "TestSignalPack.h"

class SomeInjectee
{
	static struct Ctor { Ctor(); } __ctor;

	TestSignalPack* _signalPack = nullptr;
	IInterfaceToInject* _interfaceInst = nullptr;
	int							_value = 0;
	TypeToInject* _classInst1 = nullptr;
	TypeToInject* _classInst2 = nullptr;

public:
	SomeInjectee();
	virtual ~SomeInjectee() = default;

	IInterfaceToInject* GetInterfaceInstance() const { return  _interfaceInst; }
	TypeToInject* GetInjected1() const { return _classInst1; }
	TypeToInject* GetInjected2() const { return _classInst2; }

	double DoSomething();
};

