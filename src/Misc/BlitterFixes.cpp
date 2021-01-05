#include <Helpers/Macro.h>
#include "../Utilities/Debug.h"

#define ushort unsigned __int16

// Half of an RGB range in 16 bit
// (equivalent of (127, 127, 127))
#define HALF_RANGE_MASK 0x7BEFu

// Blending mask for 16-bit pixels
// https://medium.com/@luc.trudeau/fast-averaging-of-high-color-16-bit-pixels-cb4ac7fd1488
#define BLENDING_MASK 0xF7DEu

inline ushort Blit50TranslucencyFix(ushort dst, ushort src)
{
	return (((((src) ^ (dst)) & BLENDING_MASK) >> 1) + ((src) & (dst)));
}

inline ushort Blit75TranslucencyFix(ushort dst, ushort src)
{
	ushort div = (((((src) ^ (dst)) & BLENDING_MASK) >> 1) + ((src) & (dst)));
	return (div >> 1 & HALF_RANGE_MASK) + (dst >> 1 & HALF_RANGE_MASK);
}

//same as 75, just reversed order of args
inline ushort Blit25TranslucencyFix(ushort dst, ushort src)
{
	return Blit75TranslucencyFix(src, dst);
}


DEFINE_HOOK(492866, BlitTransLucent50_Fix, 12)
{
	GET(ushort, srcv, EAX);
	GET(ushort*, dest, EDI);

	*dest = Blit50TranslucencyFix(*dest, srcv);

	return 0x492878;
}