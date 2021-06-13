# include "netranexception.h"

namespace zx
{
	netranexception::netranexception() : logic_error("Netranexception"),
		_error(0)
	{
		capture_stack();
	}

	netranexception::netranexception(const std::string& msg) : logic_error(msg)
	{
		capture_stack();
	}

	netranexception::netranexception(const std::string& msg, int error) :
		logic_error(msg + std::to_string(error)), _error(error)
	{
		capture_stack();
	}

	std::string netranexception::stack_trace_str() const
	{
		std::stringstream ss;
		for (const auto& frame : _stack_trace)
		{
			ss << "\tin " << frame.file_name << " : line "
				<< frame.line_number << std::endl;
		}

		return ss.str();
	}

	struct CSymbolInfoPackage : public SYMBOL_INFO_PACKAGE
	{
		CSymbolInfoPackage()
		{
			si.SizeOfStruct = sizeof(SYMBOL_INFO);
			si.MaxNameLen = sizeof(name);
		}
	};

	void netranexception::capture_stack()
	{
		void* stack[TRACE_MAX_STACK_FRAMES];
		HANDLE process = GetCurrentProcess();
		auto result = SymInitialize(process, NULL, TRUE);
		WORD numberOfFrames = CaptureStackBackTrace(0, TRACE_MAX_STACK_FRAMES, stack, NULL);

		DWORD displacement = 0;
		IMAGEHLP_LINE64* line = (IMAGEHLP_LINE64*)malloc(sizeof(IMAGEHLP_LINE64));
		line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
		for (int i = 0; i < numberOfFrames; i++)
		{
			DWORD64 address = (DWORD64)(stack[i]);

			// It was nice idea but this is surprisingly blocking call. 
			//SymFromAddr(process, address, &displacement2, &symbol.si);
			if (SymGetLineFromAddr64(process, address, &displacement, line))
			{
				if (i > 1)
				{
					_stack_trace.push_back(
						{
							line->FileName,
							static_cast<int>(line->LineNumber)
						});
				}
			}
			//else
			//{
			//	_stack_trace.push_back(
			//		{
			//			std::string(),
			//			0
			//		});
			//}
		}
		free(line);
	}
}