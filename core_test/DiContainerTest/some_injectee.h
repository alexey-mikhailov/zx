# pragma once
# include "test_types.h"
# include "test_signal_pack.h"

class some_injectee
{
	static struct ctor { ctor(); } __ctor;

	std::shared_ptr<test_signal_pack>	_signal_pack = nullptr;
	std::shared_ptr<interface_to_inject> _interface_inst = nullptr;
	int									_value = 0;
	std::shared_ptr<type_to_inject>		_class_inst_1 = nullptr;
	std::shared_ptr<type_to_inject>		_class_inst_2 = nullptr;

public:
	some_injectee();
	virtual ~some_injectee() = default;

	std::shared_ptr<interface_to_inject> get_interface_instance() const;
	std::shared_ptr<type_to_inject> get_class_instance_1() const;
	std::shared_ptr<type_to_inject> get_class_instance_2() const;

	double do_something();
};

