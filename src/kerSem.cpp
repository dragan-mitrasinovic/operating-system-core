// File: kerSem.cpp
#include "kerSem.h"
#include "system.h"
#include "SCHEDULE.H"

SemNode* KernelSem::headSem=0;

KernelSem::KernelSem(int init){
	lock();
	value=init;
	head1=tail1=head2=tail2=0;
	addToList(this);
	unlock();
}

KernelSem::~KernelSem(){
	lock();
	removeFromList(this);
	unlock();
}

int KernelSem::wait(Time max){
	lock();
	value--;
	if(value<0){
		if(max==0){
			block();
			return 1;
		}
		else{
			timeBlock(max);
			lock();
			int ret=PCB::running->wokenUpBySignal;
			PCB::running->wokenUpBySignal=0;
			unlock();
			return ret;
		}

	}
	unlock();
	return 1;
}

void KernelSem::signal(){
	lock();
	value++;
	if(value<1){
		unblock();
	}
	unlock();
}

int KernelSem::val() const{
	return value;
}

void KernelSem::addToList(KernelSem* sem){
	SemNode* temp=new SemNode(sem,headSem);
	headSem=temp;
}

void KernelSem::removeFromList(KernelSem* sem){
	if(headSem==0)return;
	SemNode* temp,*prev;
	if(headSem->info==sem){
		temp=headSem;
		headSem=headSem->next;
		delete temp;
		return;
	}
	temp=headSem->next;
	prev=headSem;
	while(temp){
		if(temp->info==sem){
			prev->next=temp->next;
			delete temp;
			return;
		}
		prev=temp;
		temp=temp->next;
	}
	return;
}

void KernelSem::block(){
	PCB::running->status=BLOCKED;
	addToBlockedList1((PCB*)PCB::running);
	unlock();
	dispatch();
}

void KernelSem::addToBlockedList1(PCB* pcb){
	if(head1==0){
		head1=new PCBNode(pcb,0);
		tail1=head1;
	}
	else{
		PCBNode* temp=new PCBNode(pcb,0);
		tail1->next=temp;
		tail1=temp;
	}
}

void KernelSem::timeBlock(Time t){
	PCB::running->status=BLOCKED;
	addToBlockedList2((PCB*)PCB::running,t);
	unlock();
	dispatch();
}

void KernelSem::addToBlockedList2(PCB* pcb,Time t){
	if(head2==0){
		head2=new PCBNode(pcb,0,t);
		tail2=head2;
	}
	else{
		PCBNode* temp=new PCBNode(pcb,0,t);
		tail2->next=temp;
		tail2=temp;
	}
}

void KernelSem::unblock(){
	if(head1!=0){
		PCBNode* del=head1;
		PCB* temp=head1->info;
		head1=head1->next;
		temp->status=READY;
		Scheduler::put(temp);
		delete del;
		return;
	}

	if(head2!=0){
		PCBNode* del=head2;
		PCB* temp=head2->info;
		head2=head2->next;
		temp->status=READY;
		temp->wokenUpBySignal=1;
		Scheduler::put(temp);
		delete del;
		return;
	}
	return;
}

void KernelSem::passTime(){
	SemNode* curr=headSem;
	while(curr){
		PCBNode* prev=0;
		PCBNode* currPCBNode=curr->info->head2;
		while(currPCBNode){
			--currPCBNode->time;
			if(currPCBNode->time==0){
				if(currPCBNode==curr->info->head2){
					PCBNode* del=curr->info->head2;
					del->info->status=READY;
					Scheduler::put(del->info);
					if(curr->info->tail2==curr->info->head2)curr->info->tail2=0;
					curr->info->head2=curr->info->head2->next;
					currPCBNode=currPCBNode->next;
					delete del;
				}
				else{
					currPCBNode->info->status=READY;
					Scheduler::put(currPCBNode->info);
					PCBNode* next=currPCBNode->next;
					if(currPCBNode->next){
					currPCBNode->info=next->info;
					currPCBNode->time=next->time;
					currPCBNode->next=next->next;
					}
					if(next==curr->info->tail2)curr->info->tail2=currPCBNode;
						delete next;
				}

			}
			else{
				prev=currPCBNode;
				currPCBNode=currPCBNode->next;
			}
		}
		curr=curr->next;
	}
}




