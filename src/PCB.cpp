// File: PCB.h
#include"PCB.h"
#include <dos.h>
#include "SCHEDULE.H"
#include "system.h"

ID PCB::lastID=0;
volatile PCB* PCB::running=0;
PCBNode* PCB::headAll=0;
Loop* PCB::loop=0;


PCB::PCB(StackSize stackSize, Time timeSlice,Thread* myThread){
	lock();
	id=++lastID;
	this->myThread=myThread;
	this->timeSlice=timeSlice;
	status=NEW;
	sp=ss=bp=0;
	headBlocked=0;
	wokenUpBySignal=0;


	stackSize/=sizeof(unsigned);
	stack=new unsigned[stackSize];


	stack[stackSize-1]=0x200;
	#ifndef BCC_BLOCK_IGNORE
		stack[stackSize-2]=FP_SEG(&(wrapper));
		stack[stackSize-3]=FP_OFF(&(wrapper));

		sp = FP_OFF(stack+stackSize-12);
		ss = FP_SEG(stack+stackSize-12);
		bp = sp;
	#endif

	addToList(this);
	unlock();
}

PCB::~PCB(){
	lock();
	removeFromList(this);
	delete stack;
	unlock();
}

PCB* PCB::getPCBById(ID id){
	lock();
	PCBNode* temp=headAll;
	while(temp){
		if(temp->info->id==id){
			unlock();
			return temp->info;
		}
		temp=temp->next;
	}
	unlock();
	return 0;
}

void PCB::wrapper(){
	running->myThread->run();
	lock();
	running->status=FINISHED;
	running->unblockAll();
	unlock();
	dispatch();
}

void PCB::addToList(PCB* pcb){
	PCBNode* temp=new PCBNode(pcb,headAll);
	headAll=temp;
}

void PCB::removeFromList(PCB* pcb){
	if(headAll==0)return;
	PCBNode* temp,*prev;
	if(headAll->info==pcb){
		temp=headAll;
		headAll=headAll->next;
		delete temp;
		return;
	}
	temp=headAll->next;
	prev=headAll;
	while(temp){
		if(temp->info==pcb){
			prev->next=temp->next;
			delete temp;
			return;
		}
		prev=temp;
		temp=temp->next;
	}
	return;
}

void PCB::addToBlockedList(PCB* pcb){
	PCBNode* temp=new PCBNode(pcb,headBlocked);
	headBlocked=temp;
}

void PCB::unblockAll() volatile{
PCBNode* temp=headBlocked;PCBNode* last;
	while(temp){
		last=temp;
		temp=temp->next;
		last->info->status=READY;
		Scheduler::put(last->info);
		delete last;
		}
	headBlocked=0;
}


