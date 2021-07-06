# pragma once
# include "test_types.h"
# include "test_signal_pack.h"

class some_injectee
{
	static struct ctor { ctor(); } __ctor;

	test_signal_pack* _signal_pack = nullptr;
	interface_to_inject* _interface_inst = nullptr;
	int							_value = 0;
	type_to_inject* _class_inst_1 = nullptr;
	type_to_inject* _class_inst_2 = nullptr;

public:
	some_injectee();
	virtual ~some_injectee() = default;

	interface_to_inject* get_interface_instance() const;
	type_to_inject* get_injected_1() const;
	type_to_inject* get_injected_2() const;

	double do_something();
};

