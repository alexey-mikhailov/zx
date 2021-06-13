# pragma once

struct logger_stream
{
	logger_stream& operator <<(const char* value);
	logger_stream& operator <<(void* value);
	logger_stream& operator <<(int value);
	logger_stream& operator <<(float value);
	logger_stream& operator <<(double value);

private:
	std::stringstream _ss;
};

inline logger_stream ls;
