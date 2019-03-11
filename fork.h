#pragma once
#include <mutex>

class Fork {
	std::mutex _mutex;
	int _id;
public:
	Fork(int id) : _id(id) {}
	int id() const { return _id; }
	
	void take_me() {
		_mutex.lock();
	}
	void release_me() {
		_mutex.unlock();
	}
};
