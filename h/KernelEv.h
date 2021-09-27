// File: KernelEv.h

#ifndef _KernelEv_h_
#define _KernelEv_h_

#include "PCB.h"
#include "event.h"

class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void wait();
	void signal();

private:
	PCB* owner;
	PCB* blocked;
	int value;
	IVTNo entry;

};

#endif
