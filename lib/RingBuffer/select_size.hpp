#ifndef SELECT_SIZE_H
#define SELECT_SIZE_H

#include "static_if.hpp"
#include <stdint.h>

	template<unsigned sizeBits>
	struct SelectSize
	{
		static const bool LessOrEq8 = sizeBits <= 8;
		static const bool LessOrEq16 = sizeBits <= 16;

		typedef typename StaticIf<
				LessOrEq8,
				uint8_t,
				typename StaticIf<LessOrEq16, uint16_t, uint32_t>::Result>
				::Result Result;
	};


	template<unsigned size>
	struct SelectSizeForLength
	{
		static const bool LessOrEq8 = size <= 0xff;
		static const bool LessOrEq16 = size <= 0xffff;

		typedef typename StaticIf<
				LessOrEq8,
				uint8_t,
				typename StaticIf<LessOrEq16, uint16_t, uint32_t>::Result>
				::Result Result;
	};

#endif /* SELECT_SIZE_H */
