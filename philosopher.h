#include <iostream>
#include <mutex>
#include <thread>
#include <chrono> 
#include "fork.h"
#include <memory>
#include "randomizer.h"
#include <atomic>

class Philosopher {
	int mId;

	const int mMaxThinkingTime;
	const int mMaxEatingTime;

	const std::shared_ptr<Fork> mLeftFork;
	const std::shared_ptr<Fork> mRightFork;

	const std::shared_ptr<Randomizer> mRandomizer;
	const std::shared_ptr<std::atomic_bool> mDinnerIsRunning;

public:
	Philosopher(
		const int id,
		const int maxThinkingTime,
		const int maxEatingTime,
		const std::shared_ptr<Fork> leftFork,
		const std::shared_ptr<Fork> rightFork,
		const std::shared_ptr<Randomizer> randomizer,
		const std::shared_ptr<std::atomic_bool> dinnerIsRunning)
		:
		mId(id),
		mMaxThinkingTime(maxThinkingTime),
		mMaxEatingTime(maxEatingTime),
		mLeftFork(leftFork),
		mRightFork(rightFork),
		mRandomizer(randomizer),
		mDinnerIsRunning(dinnerIsRunning)
	{}

	void dine() {
		//std::unique_lock<std::mutex> lock(mMutex);
		//mCondVar.wait(lock);
		//lock.unlock();

		// Busy waiting
		while (!mDinnerIsRunning->load()) {}

		while (mDinnerIsRunning->load()) {
			auto t = mRandomizer->generateRandomInt(0, mMaxThinkingTime);
			std::this_thread::sleep_for(std::chrono::milliseconds(t));
			std::cout << "phil" << mId << " finished thinking\n";
			takeFork(mLeftFork);
			std::cout << "phil" << mId << " took first fork: " << mLeftFork->id() << "\n";
			takeFork(mRightFork);
			std::cout << "phil" << mId << " took second fork: " << mRightFork->id() << "\n";
			auto e = mRandomizer->generateRandomInt(0, mMaxEatingTime);
			std::this_thread::sleep_for(std::chrono::milliseconds(e));
			std::cout << "phil" << mId << " is done eating\n";
			putForksBack();
		}
	}

	void takeFork(std::shared_ptr<Fork> fork) const {
		fork->take_me();
	}

	void putForksBack() const {
		auto r = mRandomizer->generateRandomInt(0, 1);

		if (r % 2) {
			mLeftFork->release_me();
			mRightFork->release_me();
		}
		else {
			mRightFork->release_me();
			mLeftFork->release_me();
		}
	}
};