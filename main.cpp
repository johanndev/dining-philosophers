#include <iostream>
#include <string>
#include <tuple>
#include <memory>
#include <vector>
#include <algorithm>
#include <tclap/CmdLine.h>
#include "main.h"
#include "fork.h"
#include <chrono>         
#include <thread>
#include <random>
#include <atomic>

static std::atomic_bool dinnerIsRunning;
int intRand(const int& min, const int& max) {
	thread_local std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<int> dist(min, max);
	return dist(mt);
}


std::vector<std::unique_ptr<Fork>> forks;
void take_fork(int index) {
	forks.at(index)->take_me();
}

void put_back_forks(int index1, int index2) {
	auto r = intRand(0, 1);

	if (r % 2) {
		std::swap(index1, index2);
	}

	forks.at(index1)->release_me();
	forks.at(index2)->release_me();
}

void dine(int pCount, int index, int maxThinkingTime, int maxEatingTime) {

	while (!dinnerIsRunning.load()) {}

	while (dinnerIsRunning)
	{
		auto t = intRand(0, maxThinkingTime);
		std::this_thread::sleep_for(std::chrono::milliseconds(t));
		std::cout << "phil" << index << " finished thinking\n";
		take_fork(index);
		std::cout << "phil" << index << " took first fork: " << index << "\n";
		take_fork((index + 1) % pCount);
		std::cout << "phil" << index << " took second fork: " << (index + 1) % pCount << "\n";
		auto e = intRand(0, maxEatingTime);
		std::this_thread::sleep_for(std::chrono::milliseconds(e));
		std::cout << "phil" << index << " is done eating\n";
		put_back_forks(index, (index + 1) % pCount);
	}
}

int main(int argc, char** argv)
{
		// cmd parsing
		int philCount, thinkingTime, eatingTime;
		std::tie(philCount, thinkingTime, eatingTime) = ParseCmdLine(argc, argv);

		// create as many forks as philosophers
		for (auto i = 0; i < philCount; i++)
		{
			forks.emplace_back(std::make_unique<Fork>(i));
		}

		// prepare the 'philosophers'
		std::vector<std::thread> philosophers;
		for (auto i = 0; i < philCount; i++)
		{
			philosophers.emplace_back(std::thread(dine, philCount, i, thinkingTime, eatingTime));
		}

		// signal the start of the dinner to the threads
		dinnerIsRunning.store(true);
		std::cout << "Press any key to end the dinner...\n";
		std::cin.ignore();

		// signal end of dinner to threads
		dinnerIsRunning.store(false);

		for (auto& p : philosophers) {
			p.join();
		}
	}



