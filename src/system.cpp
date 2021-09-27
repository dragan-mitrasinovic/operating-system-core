// File: system.cpp
#include <dos.h>
#include "system.h"
#include "loop.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "kerSem.h"

extern void tick();
volatile int System::contextSwitchNeeded=0;
pInterrupt System::oldTimer=0;
volatile unsigned System::counter=1;
volatile unsigned tsp, tss, tbp;

void System::initialize(){
	lock();

	#ifndef BCC_BLOCK_IGNORE
	oldTimer=getvect(0x08);
	setvect(0x60,oldTimer);
	setvect(0x08,timer);
	#endif


	PCB::running=new PCB(defaultStackSize,0,0);
	PCB::running->status=READY;
	PCB::loop=new Loop();
	PCB::loop->start();

	unlock();
}

void System::restore(){
	lock();

	#ifndef BCC_BLOCK_IGNORE
	setvect(0x08,oldTimer);
	#endif

	delete PCB::running;
	delete PCB::loop;

	unlock();
}

void interrupt System::timer(...){

	if(contextSwitchNeeded==0){
		tick();
		KernelSem::passTime();
		counter--;

	#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
	#endif

}

	if(contextSwitchNeeded||(counter==0 && PCB::running->timeSlice!=0)){

	#ifndef BCC_BLOCK_IGNORE
		asm{
			mov tsp,sp
			mov tss,ss
			mov tbp,bp
		}
	#endif

	PCB::running->sp=tsp;
	PCB::running->ss=tss;
	PCB::running->bp=tbp;

	if(PCB::running->status==READY && PCB::running->myThread!=PCB::loop)
		Scheduler::put((PCB*)PCB::running);

	PCB::running=Scheduler::get();
	if(PCB::running==0)PCB::running=PCB::loop->myPCB;

	tsp=PCB::running->sp;
	tss=PCB::running->ss;
	tbp=PCB::running->bp;


	#ifndef BCC_BLOCK_IGNORE
		asm{
			mov sp,tsp
			mov ss,tss
			mov bp,tbp
		}
	#endif

	contextSwitchNeeded=0;
	counter=PCB::running->timeSlice;
	}
}
