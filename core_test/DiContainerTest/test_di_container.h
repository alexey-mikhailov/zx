# pragma once

# include "zx.h"

constexpr int singleton_arg = 78;
constexpr int nameleton1_arg = 356;
constexpr int nameleton2_arg = 127;

class test_di_container : public zx::di_container
{
public:
	test_di_container();

protected:
	void bind_all() override;
};

