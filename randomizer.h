#pragma once
#include <random>

class Randomizer {
public:
	int generateRandomInt(const int min, const int max) {
		std::mt19937 mt{ std::random_device{}() };
		static thread_local std::uniform_int_distribution<int> dist(min, max);
		return dist(mt);
	}
};