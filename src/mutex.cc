#include "mutex.h"

void Mutex::Lock(){
	this->std_mutex.lock();
}

void Mutex::Unlock(){
	this->std_mutex.unlock();
}

bool Mutex::TryLock(){
	return this->std_mutex.try_lock();
}
