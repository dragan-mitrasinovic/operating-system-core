// File: KernelEv.cpp

#include "KernelEv.h"
#include "system.h"
#include "SCHEDULE.H"
#include "IVTEntry.h"

KernelEv::KernelEv(IVTNo ivtNo){
	lock();
	owner=(PCB*)PCB::running;
	entry=ivtNo;
	blocked=0;
	value=0;

	IVTEntry::IVT[ivtNo]->myKernelEv=this;

	unlock();
}

KernelEv::~KernelEv(){
lock();
IVTEntry::IVT[entry]->myKernelEv=0;
blocked=0;
unlock();
}

void KernelEv::wait(){
	lock();
	if(PCB::running!=owner){
		unlock();
		return;
	}
	if(value==1)value=0;
	else{
		PCB::running->status=BLOCKED;
		blocked=(PCB*)PCB::running;
		unlock();
		dispatch();
		return;
		}

	unlock();
}

void KernelEv::signal(){
lock();
if(blocked){
	blocked->status=READY;
	Scheduler::put(blocked);
	value=0;
	blocked=0;

}
else value=1;

unlock();
}