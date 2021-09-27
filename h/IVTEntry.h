// File: IVTEntry.h
#ifndef _IVTEntry_h_
#define _IVTEntry_h_

#include "event.h"
#include "system.h"


//A macro for initializing an IVT Entry
#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...);\
IVTEntry newEntry##numEntry(numEntry, inter##numEntry);\
void interrupt inter##numEntry(...){\
	newEntry##numEntry.signal();\
	if (callOld == 1)\
	newEntry##numEntry.callOldR();\
}

class IVTEntry{
public:
	IVTEntry(IVTNo ivtNo, pInterrupt newRoutine);
	~IVTEntry();
	static IVTEntry** IVT;
	void signal();
	void callOldR();

private:
	 pInterrupt oldRoutine;
	 KernelEv* myKernelEv;
	 IVTNo entry;

protected:
	 friend class KernelEv;
	 friend class Event;

};

#endif
