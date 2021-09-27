// File: system.h
#ifndef _system_h_
#define _system_h_

typedef void interrupt (*pInterrupt)(...);

static void lock() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
#endif
}

static void unlock() {
#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif
}


class System{
public:
	static void initialize();
	static void restore();
	static interrupt void timer(...);
	static pInterrupt oldTimer;
	static volatile unsigned counter;
	static volatile int contextSwitchNeeded;

};

#endif
