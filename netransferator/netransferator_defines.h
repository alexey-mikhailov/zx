# pragma once

typedef unsigned __int8 u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
typedef __int8 i8;
typedef __int16 i16;
typedef __int32 i32;
typedef __int64 i64;

typedef unsigned __int8 byte;

# ifdef NETRANSFERATOR_EXPORTS
#	define NETRANSFERATOR_API __declspec(dllexport)
# else
#	define NETRANSFERATOR_API __declspec(dllimport)
# endif


#define TRACE_MAX_STACK_FRAMES 1024
#define TRACE_MAX_FUNCTION_NAME_LENGTH 1024