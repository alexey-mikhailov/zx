# pragma once

namespace zx
{
	inline const char* empty_charbuf = "";
	inline const wchar_t* empty_wcharbuf = L"";
	inline const std::string empty_string = empty_charbuf;
	inline const std::wstring empty_wstring = empty_wcharbuf;
	inline const char* whitespace = " ";
	inline const wchar_t* widespace = L" ";
	inline const char* endl = "\n";
}
