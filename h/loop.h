// File: loop.h
#ifndef _loop_h_
#define _loop_h_

#include "thread.h"

class Loop: public Thread{

public:
	Loop();
	void run();
	void start();

};

#endif
