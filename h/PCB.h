// File: PCB.h
#ifndef _PCB_h_
#define _PCB_h_
#include "thread.h"
#include "semaphor.h"

enum Status{NEW,READY,BLOCKED,FINISHED};

struct PCBNode{
		PCB* info;
		PCBNode* next;
		Time time;
		PCBNode(PCB* inf, PCBNode* n=0){
			info=inf;
			next=n;
			time=0;
		}
		PCBNode(PCB* inf, PCBNode* n,Time t ){
					info=inf;
					next=n;
					time=t;
				}
		~PCBNode(){
			info=0;
			next=0;
		}
	};

class PCB{
private:
	unsigned *stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned timeSlice;
	Status status;
	Thread* myThread;
	ID id;
	static ID lastID;
	int wokenUpBySignal;

	static volatile PCB* running;
	static PCB* getPCBById(ID id);
	static void wrapper();
	static PCBNode* headAll;  //List of all system PCB-s
	PCBNode* headBlocked;     //List of all PCB-s (Threads) waiting for this one to finish
	static void addToList(PCB* pcb);
	static void removeFromList(PCB* pcb);
	void addToBlockedList(PCB* pcb);
	void unblockAll() volatile;
	static Loop* loop;

public:
	PCB(StackSize stackSize = defaultStackSize,Time timeSlice =defaultTimeSlice,Thread* myThread=0 );
	virtual ~PCB();

protected:
	friend class Thread;
	friend class System;
	friend class Loop;
	friend class KernelSem;
	friend class KernelEv;

};

#endif
