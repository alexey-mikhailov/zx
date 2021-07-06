# pragma once

# include "IInterfaceToInject.h"
# include "TypeToInjectBase.h"

class type_to_inject : public type_to_inject_base, public interface_to_inject
{
public:
	type_to_inject() = default;

	int some_method() override;

	int public_field;
};

