#pragma once
#include <mutex>

class Fork {
	std::mutex mMutex;
	int mId;
public:
	Fork(int id) : mId(id) {}
	int id() const { return mId; }
	
	void take_me() {
		mMutex.lock();
	}
	void release_me() {
		mMutex.unlock();
	}
};
