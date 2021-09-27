// File: thread.cpp
#include "thread.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "system.h"

Thread::Thread(StackSize stackSize, Time timeSlice){
	if(stackSize>65536)stackSize=65536;
	myPCB=new PCB(stackSize,timeSlice,this);
}

void Thread::start(){
	lock();
	if(myPCB->status==NEW){
	myPCB->status=READY;
	Scheduler::put(myPCB);
	}
	unlock();
}

void Thread::waitToComplete(){
	lock();
	if(myPCB!=PCB::running && myPCB->status!=FINISHED &&
		myPCB->status!=NEW && this!=PCB::loop){
		PCB::running->status=BLOCKED;
		myPCB->addToBlockedList((PCB*)PCB::running);
		unlock();
		dispatch();
		return;
		}
	unlock();
	}

Thread::~Thread(){
	lock();
	waitToComplete();
	delete myPCB;
	unlock();
}

ID Thread::getId(){
	return myPCB->id;
}

ID Thread::getRunningId(){
	return PCB::running->id;
}

Thread* Thread::getThreadById(ID id){
	PCB* pcb=PCB::getPCBById(id);
	if(pcb==0)return 0;
	else return pcb->myThread;
}

void dispatch(){
	lock();
	System::contextSwitchNeeded=1;
	System::timer();
	unlock();
}
