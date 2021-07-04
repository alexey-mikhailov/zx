# pragma once

namespace zx
{
	inline const char* empty_charbuf = "";
	inline const wchar_t* empty_wcharbuf = L"";
	inline const std::string empty_string = empty_charbuf;
	inline const std::wstring empty_wstring = empty_wcharbuf;
	inline const char* whitespace = " ";
	inline const wchar_t* widespace = L" ";

	inline const char* endl = "\r\n";

	typedef ::int8_t   i08;
	typedef ::int16_t  i16;
	typedef ::int32_t  i32;
	typedef ::int64_t  i64;
	typedef ::uint8_t  u08;
	typedef ::uint16_t u16;
	typedef ::uint32_t u32;
	typedef ::uint64_t u64;
	typedef ::float_t  f32;
	typedef ::double_t f64;

	namespace zero
	{
		constexpr zx::i08 i08 = 0i8;
		constexpr zx::i16 i16 = 0i16;
		constexpr zx::i32 i32 = 0i32;
		constexpr zx::i64 i64 = 0i64;
		constexpr zx::u08 u08 = 0ui8;
		constexpr zx::u16 u16 = 0ui16;
		constexpr zx::u32 u32 = 0ui32;
		constexpr zx::u64 u64 = 0ui64;
		constexpr zx::f32 f32 = 0.0f;
		constexpr zx::f64 f64 = 0.0;
	}
	namespace one
	{
		constexpr zx::i08 i08 = 1i8;
		constexpr zx::i16 i16 = 1i16;
		constexpr zx::i32 i32 = 1i32;
		constexpr zx::i64 i64 = 1i64;
		constexpr zx::u08 u08 = 1ui8;
		constexpr zx::u16 u16 = 1ui16;
		constexpr zx::u32 u32 = 1ui32;
		constexpr zx::u64 u64 = 1ui64;
		constexpr zx::f32 f32 = 1.0f;
		constexpr zx::f64 f64 = 1.0;
	}
}
