// File: semaphor.h
#include "semaphor.h"
#include "kerSem.h"

Semaphore::Semaphore(int init){
	myImpl=new KernelSem(init);
}

Semaphore::~Semaphore(){
	delete myImpl;
	myImpl=0;
}

int Semaphore::wait(Time max){
	return myImpl->wait(max);
}

void Semaphore::signal(){
	myImpl->signal();
}

int Semaphore::val()const{
	return myImpl->val();
}
