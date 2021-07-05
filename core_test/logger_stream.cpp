# include "logger_stream.h"
# include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

logger_stream& logger_stream::operator<<(const char* value)
{
	Logger::WriteMessage(value);
	return *this;
}

logger_stream& logger_stream::operator<<(void* value)
{
	_ss.str("");
	_ss << std::hex << value;
	Logger::WriteMessage(_ss.str().c_str());
	return *this;
}

logger_stream& logger_stream::operator<<(int value)
{
	_ss.str("");
	_ss << std::dec << value;
	Logger::WriteMessage(_ss.str().c_str());
	return *this;
}

logger_stream& logger_stream::operator<<(long long value)
{
	_ss.str("");
	_ss << std::dec << value;
	Logger::WriteMessage(_ss.str().c_str());
	return *this;
}

logger_stream& logger_stream::operator<<(unsigned long long value)
{
	_ss.str("");
	_ss << std::dec << value;
	Logger::WriteMessage(_ss.str().c_str());
	return *this;
}

logger_stream& logger_stream::operator<<(float value)
{
	_ss.str("");
	_ss << std::dec << value;
	Logger::WriteMessage(_ss.str().c_str());
	return *this;
}

logger_stream& logger_stream::operator<<(double value)
{
	_ss.str("");
	_ss << std::dec << value;
	Logger::WriteMessage(_ss.str().c_str());
	return *this;
}

logger_stream& logger_stream::operator<<(const std::string& value)
{
	Logger::WriteMessage(value.c_str());
	return *this;
}

