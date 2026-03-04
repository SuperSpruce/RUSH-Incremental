// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)

#include "pcg_random.h"


uint32_t pcg32_random_r(pcg32_random_t* rng)
{
    uint64_t oldstate = rng->state;
    // Advance internal state
    rng->state = oldstate * 6364136223846793005ULL + (rng->inc|1);
    // Calculate output function (XSH RR), uses old state for max ILP
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}


// SuperSpruce's part to get a random float
// Random float between 0 and 1
float pcg32_random_f(pcg32_random_t* rng)
{
	uint32_t bits = pcg32_random_r(rng);
	bits &= 0x007FFFFF;
	bits |= 0x3F800000;
	float f;
	*((uint32_t*)&f) = bits;
	return f-1;
}

/*
// Random float between -1 and 1
float pcg32_random_fn(pcg32_random_t* rng)
{
	uint32_t bits = pcg32_random_r(rng);
	bits &= 0x007FFFFF;
	bits |= 0x3F800000;
	float f;
	*((uint32_t*)&f) = bits;
	return f*2.0 - 3.0;
}

// Random float between -1/4 and 1/4
float pcg32_random_fm02(pcg32_random_t* rng)
{
	uint32_t bits = pcg32_random_r(rng);
	bits &= 0x007FFFFF;
	bits |= 0x3F800000;
	float f;
	*((uint32_t*)&f) = bits;
	return f/2.0 - 3.0/4.0;
}

// Random float between -1/8 and 1/8
float pcg32_random_fm03(pcg32_random_t* rng)
{
	uint32_t bits = pcg32_random_r(rng);
	bits &= 0x007FFFFF;
	bits |= 0x3F800000;
	float f;
	*((uint32_t*)&f) = bits;
	return f/4.0 - 3.0/8.0;
}
*/
