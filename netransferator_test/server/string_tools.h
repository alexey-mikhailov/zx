# pragma once

namespace zx
{
	std::wstring wstring(const std::string& string);

	std::wstring wstring(const wchar_t* buffer, size_t size);

	std::wstring wstring(const byte* buffer, int size);
}
