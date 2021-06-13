# include "string_tools.h"

namespace zx
{
	std::wstring wstring(const std::string& string)
	{
		return std::wstring(string.begin(), string.end());
	}

	std::wstring wstring(const wchar_t* buffer, size_t size)
	{
		return std::wstring(buffer, size / 2);
	}

	std::wstring wstring(const byte * buffer, int size)
	{
		return std::wstring(reinterpret_cast<const wchar_t*>(buffer), size / 2);
	}
}