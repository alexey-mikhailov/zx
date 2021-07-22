# pragma once

namespace zx
{
	inline const char* empty_charbuf = "";
	inline const wchar_t* empty_wcharbuf = L"";
	inline const std::string empty_string = empty_charbuf;
	inline const std::wstring empty_wstring = empty_wcharbuf;
	inline const char* whitespace = " ";
	inline const wchar_t* widespace = L" ";

	# ifdef _WIN32
	inline const char* endl = "\r\n";
	# endif

	using i08 = ::int8_t   ;
	using i16 = ::int16_t  ;
	using i32 = ::int32_t  ;
	using i64 = ::int64_t  ;
	using u08 = ::uint8_t  ;
	using u16 = ::uint16_t ;
	using u32 = ::uint32_t ;
	using u64 = ::uint64_t ;
	using f32 = ::float_t  ;
	using f64 = ::double_t ;

	// Cursed long
	using ilong =		 long;
	using ulong =		 unsigned long;
	
	# ifdef _WIN32
	using double_ilong = long long;
	using double_ulong = unsigned long long;
	# endif

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
		constexpr zx::ilong ilong = 0l;
		constexpr zx::ulong ulong = 0ul;
		
		# ifdef _WIN32
		constexpr zx::double_ilong double_ilong = 0i64;
		constexpr zx::double_ulong double_ulong = 0ui64;
		# endif
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
		constexpr zx::ilong ilong = 1l;
		constexpr zx::ulong ulong = 1ul;

		# ifdef _WIN32
		constexpr zx::double_ilong double_ilong = 1i64;
		constexpr zx::double_ulong double_ulong = 1ui64;
		# endif
	}
}
