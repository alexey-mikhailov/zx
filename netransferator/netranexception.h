# pragma once
# include "netransferator_defines.h"

namespace zx
{
	struct stack_frame
	{
		std::string file_name;
		int line_number;
	};

	struct netranexception : std::logic_error
	{
		NETRANSFERATOR_API explicit netranexception();

		NETRANSFERATOR_API explicit netranexception(const std::string& msg);

		NETRANSFERATOR_API explicit netranexception(const std::string& msg, int error);

		NETRANSFERATOR_API std::vector<stack_frame> stack_trace() const { return _stack_trace; }

		NETRANSFERATOR_API int error() const { return _error; }

		NETRANSFERATOR_API std::string stack_trace_str() const;

	private:
		int _error;
		std::vector<stack_frame> _stack_trace;

		void capture_stack();
	};
}