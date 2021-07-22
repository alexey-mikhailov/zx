# pragma once

namespace zx
{
	namespace meta
	{
		/// <summary>
		/// Field exposure type. 
		/// </summary>
		enum struct expose_type
		{
			/// <summary>
			/// Field exposed by value. 
			/// </summary>
			value,

			/// <summary>
			/// Field exposed by STL shared pointer. 
			/// </summary>
			shrptr
		};

		/// <summary>
		/// Type of fieldpawn. 
		/// </summary>
		enum struct fieldpawn_type
		{
			/// <summary>
			/// Field is not changed by DI. 
			/// </summary>
			none,

			/// <summary>
			/// Field will be injected as pawn of singleton. 
			/// </summary>
			singleton,

			/// <summary>
			/// Field will be injected as pawn of named singleton. 
			/// </summary>
			nameleton,
			
			/// <summary>
			/// Field will be injected as pawn of signal pack singleton. 
			/// </summary>
			signal_pack
		};
	}
}
