# pragma once

# include "di_container.h"

class TestDiContainer : public zx::di_container
{
public:
	TestDiContainer();

protected:
	void bind_all() override;
};

