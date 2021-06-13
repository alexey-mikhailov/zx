# include "TestDiContainer.h"
# include "IInterfaceToInject.h"
# include "TypeToInject.h"
# include "TestSignalPack.h"



TestDiContainer::TestDiContainer() :
	di_container(nameof(TestDiContainer))
{
	TestDiContainer::bind_all();
}


void TestDiContainer::bind_all()
{
	bind_singleton<IInterfaceToInject, TypeToInject>();
	bind_nominated<TypeToInject>("instance 1");
	bind_nominated<TypeToInject>("instance 2");

	register_signal_pack<TestSignalPack>();
}
