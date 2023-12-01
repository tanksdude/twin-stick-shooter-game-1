#include "rng.h"

#include <chrono>

std::default_random_engine RNG::generator;
std::uniform_real_distribution<double> RNG::distribution(0, 1);

void RNG::Initialize(long long seed) {
	generator = std::default_random_engine(seed);
}

void RNG::Initialize() {
	Initialize(std::chrono::high_resolution_clock::now().time_since_epoch().count());
}

double RNG::randFunc() {
	return distribution(generator);
}

int RNG::randIntInRange(int min, int max) {
	return static_cast<int>(randFunc() * (max - min)) + min;
}

double RNG::randNumInRange(double min, double max) {
	return randFunc() * (max - min) + min;
}
float RNG::randFloatInRange(float min, float max) {
	return static_cast<float>(randFunc()) * (max - min) + min;
}
