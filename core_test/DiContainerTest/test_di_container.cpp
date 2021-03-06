# include "test_di_container.h"
# include "test_types.h"
# include "test_signal_pack.h"



test_di_container::test_di_container() :
	di_container(nameof(test_di_container))
{
	test_di_container::bind_all();
}


void test_di_container::bind_all()
{
	bind_singleton<interface_to_inject, type_to_inject>(singleton_arg);
	bind_nameleton<type_to_inject>("instance 1", nameleton1_arg);
	bind_nameleton<type_to_inject>("instance 2", nameleton2_arg);

	register_signal_pack<test_signal_pack>();
}
