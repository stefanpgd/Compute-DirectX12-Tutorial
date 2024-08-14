#pragma once
#include <random>

static unsigned int state = time(NULL);
inline unsigned int xorshift32()
{
	state ^= state << 13;
	state ^= state >> 17;
	state ^= state << 5;
	return state;
}

inline float Random01()
{
	return xorshift32() * 2.3283064365387e-10f;
}

inline float RandomInRange(float min, float max)
{
	return min + (max - min) * Random01();
}