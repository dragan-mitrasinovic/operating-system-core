// File: kerSem.h
#ifndef _kerSem_h_
#define _kerSem_h_
#include "semaphor.h"
#include "PCB.h"

struct SemNode{
	KernelSem* info;
			SemNode* next;
			SemNode(KernelSem* inf, SemNode* n=0){
				info=inf;
				next=n;
			}
			~SemNode(){
				info=0;
				next=0;
			}
};

class KernelSem{
public:
	KernelSem(int init=1);
	virtual ~KernelSem();
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	int val () const;
	static void passTime();

	static SemNode* headSem; // List of all system Semaphores
	PCBNode* head1, *tail1;  //Threads blocked with wait(0)
	PCBNode* head2, *tail2;  //Threads blocked with wait(Time t)

private:
	int value;
	static void addToList(KernelSem* sem);
	static void removeFromList(KernelSem* sem);
	void addToBlockedList1(PCB* pcb);
	void addToBlockedList2(PCB* pcb,Time t);
	void block();
	void unblock();
	void timeBlock(Time t);

};

#endif
