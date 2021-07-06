# include "zx.h"
# include "signal_user.h"

signal_user::ctor signal_user::__ctor;
signal_user::ctor::ctor()
{
	zx::metadata::add_field(
		nameof(_testSignalPack), 
		&signal_user::_signal_pack, 
		zx::inject_type::signal_pack);
}

signal_user::signal_user()
{
	zx::inject_dependencies(this);

	_signal_pack->something_happened += 
		zx::mem_fn(this, &signal_user::on_something_happened);
}

double signal_user::on_something_happened(double a, double b)
{
	return a + b;
}

