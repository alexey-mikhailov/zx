# pragma once

# include "zx.h"

class test_di_container : public zx::di_container
{
public:
	test_di_container();

protected:
	void bind_all() override;
};

