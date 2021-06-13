# include "SignalUser.h"
# include "zx.h"

SignalUser::Ctor SignalUser::__ctor;
SignalUser::Ctor::Ctor()
{
	zx::metadata::add_field(
		nameof(_testSignalPack), 
		&SignalUser::_signalPack, 
		zx::inject_type::signal_pack);
}

SignalUser::SignalUser()
{
	zx::inject_dependencies(this);

	_signalPack->SomethingHappened += zx::mem_fn(this, &SignalUser::OnSomethingHappened);
}

double SignalUser::OnSomethingHappened(double a, double b)
{
	return a + b;
}

