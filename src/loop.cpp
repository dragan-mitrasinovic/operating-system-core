// File: loop.cpp

#include "loop.h"
#include "PCB.h"

Loop::Loop():Thread(defaultStackSize,1){}

void Loop::run(){while(1){};}

void Loop::start(){myPCB->status=READY;}
