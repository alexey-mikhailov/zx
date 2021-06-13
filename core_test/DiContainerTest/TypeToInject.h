# pragma once

# include "IInterfaceToInject.h"
# include "TypeToInjectBase.h"

class TypeToInject : public TypeToInjectBase, public IInterfaceToInject
{
public:
	TypeToInject() = default;

	int SomeMethod() override;

	int PublicField;
};

