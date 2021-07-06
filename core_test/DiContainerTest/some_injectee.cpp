# include "some_injectee.h"
# include "../logger_stream.h"
# include "zx.h"
# include <CppUnitTestLogger.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


some_injectee::ctor some_injectee::__ctor;
some_injectee::ctor::ctor()
{
	zx::metadata::add_field(nameof(_signal_pack),
							&some_injectee::_signal_pack,
							zx::inject_type::signal_pack);

	zx::metadata::add_field(nameof(_interface_inst),
							&some_injectee::_interface_inst,
							zx::inject_type::singleton);

	zx::metadata::add_field(nameof(_class_inst_1),
							&some_injectee::_class_inst_1,
							new zx::named_instance("instance 1"));

	zx::metadata::add_field(nameof(_class_inst_2),
							&some_injectee::_class_inst_2,
							new zx::named_instance("instance 2"));

	zx::metadata::add_field(nameof(_value),
							&some_injectee::_value);
}



some_injectee::some_injectee()
{
	zx::inject_dependencies(this);
}

interface_to_inject* some_injectee::get_interface_instance() const
{
	return  _interface_inst;
}

type_to_inject* some_injectee::get_injected_1() const
{
	return _class_inst_1;
}

type_to_inject* some_injectee::get_injected_2() const 
{
	return _class_inst_2; 
}

double some_injectee::do_something()
{
	const double calculated1 = 1.5;
	const double calculated2 = 2.5;

	const auto delegates = _signal_pack->something_happened.get_delegates();

	double lastResult = {};
	for (auto it = delegates.begin; it != delegates.end; ++it)
	{
		lastResult = it->second->invoke(calculated1, calculated2);

		ls << "something_happened returns result: ";
		ls << lastResult << zx::endl;
	}

	return lastResult;
}


