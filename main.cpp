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
#include "randomizer.h"
#include "philosopher.h"

std::mutex mMutex;
std::condition_variable mCondVar;

std::vector<std::shared_ptr<Fork>> forks;

int main(int argc, char** argv)
{
	// cmd parsing
	int philCount, thinkingTime, eatingTime;
	std::tie(philCount, thinkingTime, eatingTime) = ParseCmdLine(argc, argv);

	// create as many forks as philosophers
	for (auto i = 0; i < philCount; i++)
	{
		forks.emplace_back(std::make_shared<Fork>(i));
	}

	const auto dinnerIsRunning = std::make_shared<std::atomic_bool>();
	auto threads = std::vector<std::thread>();

	// prepare the 'philosophers'
	std::vector<Philosopher> philosophers;
	auto randomizer = std::make_shared<Randomizer>();
	for (auto i = 0; i < philCount; i++)
	{
		philosophers.emplace_back(Philosopher{
			i,
			thinkingTime,
			eatingTime,
			forks[i],
			forks[(i + 1) % philCount],
			randomizer,
			dinnerIsRunning
		});
	}

	// Get the philosophers to the table
	for (auto &phil : philosophers)
	{
		threads.emplace_back(std::thread([&phil]() {
			phil.dine();
		}));
	}

	// signal the start of the dinner to the philosophers
	dinnerIsRunning->store(true);
	std::cout << "Press any key to end the dinner...\n";
	std::cin.ignore();

	// signal end of dinner to threads
	dinnerIsRunning->store(false);

	for (auto& t : threads) {
		t.join();
	}
}



