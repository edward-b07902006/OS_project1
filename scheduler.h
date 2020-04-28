#ifndef _SCHEDULING_H_
#define _SCHEDULING_H_
#include <sys/types.h>
#define FIFO 0
#define RR 1
#define SJF	2
#define PSJF 3
#define time_slice 500
#define UNIT_TIME() {volatile unsigned long i; for(int i = 0; i < 1000000UL; i++); }
typedef struct process{
	char name[30];
	int t_ready, t_exec;
	pid_t pid;
 }Proc;

/* Running scheduler */
int cpu_scheduling(Proc *p,int policy,int N);

#endif
