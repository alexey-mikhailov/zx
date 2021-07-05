# pragma once

struct logger_stream
{
	logger_stream& operator <<(const char* value);
	logger_stream& operator <<(void* value);
	logger_stream& operator <<(int value);
	logger_stream& operator <<(long long value);
	logger_stream& operator <<(unsigned long long value);
	logger_stream& operator <<(float value);
	logger_stream& operator <<(double value);
	logger_stream& operator <<(const std::string& value);

	template<class T, class R>
	logger_stream& operator <<(std::chrono::duration<T, R> value);

private:
	std::stringstream _ss;
};

inline logger_stream ls;

template<class T, class R>
inline logger_stream& logger_stream::operator<<(std::chrono::duration<T, R> value)
{
	_ss.str("");
	_ss << std::dec << value.count();
	Logger::WriteMessage(_ss.str().c_str());
	return *this;
}
