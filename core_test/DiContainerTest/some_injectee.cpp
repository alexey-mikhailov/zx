# include "some_injectee.h"
# include "../logger_stream.h"
# include "zx.h"
# include <CppUnitTestLogger.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


some_injectee::ctor some_injectee::__ctor;

some_injectee::ctor::ctor()
{
	zx::meta::registry::add_field(
		nameof(_signal_pack),
		&some_injectee::_signal_pack,
		zx::make_fieldpawn_of_signal_pack());

	zx::meta::registry::add_field(
		nameof(_interface_inst),
		&some_injectee::_interface_inst,
		zx::make_fieldpawn_of_singleton());

	zx::meta::registry::add_field(
		nameof(_class_inst_1),
		&some_injectee::_class_inst_1,
		zx::make_fieldpawn_of_nameleton("instance 1"));

	zx::meta::registry::add_field(
		nameof(_class_inst_2),
		&some_injectee::_class_inst_2,
		zx::make_fieldpawn_of_nameleton("instance 2"));

	zx::meta::registry::add_field(
		nameof(_value),
		&some_injectee::_value);
}



some_injectee::some_injectee()
{
	zx::inject_dependencies(this);
}

std::shared_ptr<interface_to_inject> some_injectee::get_interface_instance() const
{
	return  _interface_inst;
}

std::shared_ptr<type_to_inject> some_injectee::get_class_instance_1() const
{
	return _class_inst_1;
}

std::shared_ptr<type_to_inject> some_injectee::get_class_instance_2() const 
{
	return _class_inst_2; 
}

double some_injectee::do_something()
{
	const double calculated1 = 1.5;
	const double calculated2 = 2.5;

	const auto delegates = _signal_pack->something_happened.get_delegates();

	double lastResult {};
	for (auto&& [hash, dlgt] : delegates)
	{
		lastResult = dlgt->invoke(calculated1, calculated2);

		ls << "something_happened returns result: ";
		ls << lastResult << zx::endl;
	}

	return lastResult;
}


