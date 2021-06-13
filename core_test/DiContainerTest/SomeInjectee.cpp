# include "SomeInjectee.h"
# include "../logger_stream.h"
# include "zx.h"
# include <CppUnitTestLogger.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


SomeInjectee::Ctor SomeInjectee::__ctor;
SomeInjectee::Ctor::Ctor()
{
	zx::metadata::add_field(nameof(_signalPack),
							&SomeInjectee::_signalPack,
							zx::inject_type::signal_pack);

	zx::metadata::add_field(nameof(_interfaceInst),
							&SomeInjectee::_interfaceInst,
							zx::inject_type::singleton);

	zx::metadata::add_field(nameof(_classInst1),
							&SomeInjectee::_classInst1,
							new zx::named_instance("instance 1"));

	zx::metadata::add_field(nameof(_classInst2),
							&SomeInjectee::_classInst2,
							new zx::named_instance("instance 2"));

	zx::metadata::add_field(nameof(_value),
							&SomeInjectee::_value);
}



SomeInjectee::SomeInjectee()
{
	zx::inject_dependencies(this);
}

double SomeInjectee::DoSomething()
{
	const double calculated1 = 1.5;
	const double calculated2 = 2.5;

	const auto delegates = _signalPack->SomethingHappened.get_delegates();

	double lastResult = {};
	for (auto it = delegates.begin; it != delegates.end; ++it)
	{
		lastResult = it->second->invoke(calculated1, calculated2);

		ls << "SomethingHappened returns result: ";
		ls << lastResult << zx::endl;
	}

	return lastResult;
}


