#include "rng.h"
//#include <limits>
#include <cstdlib> //pretty sure this is auto-included in C++
#include <chrono>

std::default_random_engine RNG::generator;
std::uniform_real_distribution<double> RNG::distribution;

void RNG::Initialize(long long seed) {
	//srand(seed);
	generator = std::default_random_engine(seed);
	std::uniform_int_distribution<int> distribution(0, 1);
}

void RNG::Initialize() {
	RNG::Initialize(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

double RNG::randFunc() {
	return distribution(generator);
}

int RNG::randNumInRange(int min, int max) {
	return int(RNG::randFunc() * (max - min)) + min;
}
