// File: IVTEntry.cpp
#include "IVTEntry.h"
#include "KernelEv.h"
#include <dos.h>

IVTEntry** IVTEntry::IVT=new IVTEntry*[256];

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRoutine){
	lock();
	oldRoutine=0;
	entry=ivtNo;
	myKernelEv=0;

#ifndef BCC_BLOCK_IGNORE
	oldRoutine=getvect(ivtNo);
	setvect(ivtNo,newRoutine);
#endif

	IVT[ivtNo]=this;
	unlock();
}

IVTEntry::~IVTEntry(){
	callOldR();

	lock();
#ifndef BCC_BLOCK_IGNORE
	setvect(entry,oldRoutine);
#endif

	unlock();
}

void IVTEntry::signal(){
	if(myKernelEv)myKernelEv->signal();
}

void IVTEntry::callOldR(){
	oldRoutine();
}
