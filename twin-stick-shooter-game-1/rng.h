#pragma once
#include <random>

class RNG {
private:
	static std::default_random_engine generator;
	static std::uniform_real_distribution<double> distribution;

public:
	static void Initialize(long long seed);
	static void Initialize();

	static double randFunc(); //[0,1)
	//static double randFunc2(); //[0,1]
	static int randNumInRange(int min, int max); //[min, max)
};
