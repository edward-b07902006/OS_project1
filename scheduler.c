#define _GNU_SOURCE
#include "scheduler.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sched.h>

static int now_time,index,count,last_time,nextprocess;

void assign_cpu(int pid,int core){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core,&mask);
	if(sched_setaffinity(pid,sizeof(mask),&mask) < 0){
		perror("sched_affinity");
		exit(1);
	}
	return;
}

int p_exec(Proc p){
	int pid = fork();
	if(pid == 0){
		unsigned long begin_t, begin_nt, end_t, end_nt;
		syscall(334,&begin_t,&begin_nt);
		for(int i = 0; i < p.t_exec; i++) UNIT_TIME();
		syscall(334,&end_t,&end_nt);
		syscall(335,getpid(),begin_t,begin_nt,end_t,end_nt);
		exit(0);
	}
	else assign_cpu(pid,1);
	return pid;
}

int priority(int pid,int mode){
	struct sched_param param;
	if(mode) param.sched_priority = 99;
	else param.sched_priority = 1;
	int ret = sched_setscheduler(pid,SCHED_FIFO,&param);
	if(ret < 0){
		perror("sched_setscheduler");
		return -1;
	}
	return ret;
}
int cpu_scheduling(Proc *p,int policy,int N){
	assign_cpu(getpid(),0);
	priority(getpid(),1);
	// Initial scheduler
	now_time = count = nextprocess = 0;
	index = -1; //no one is running
	while(1){
		/*check if process finish*/
		if(index != -1 && p[index].t_exec <= 0){
			waitpid(p[index].pid,NULL,0);
			printf("%s %d\n",p[index].name,p[index].pid);
			fflush(0);
			index = -1;
			count++;
			if(count == N) break; //finish all process
		}
		/*check any process ready*/
		while(p[nextprocess].t_ready == now_time && nextprocess < N){
			p[nextprocess].pid = p_exec(p[nextprocess]);
			priority(p[nextprocess].pid,0); //block first
			nextprocess++;
		}
		/*select next process to run*/
		int next = -1;
		if(index != -1 && ((policy == FIFO) || (policy == SJF))) next = index;
		else{
			if(policy == PSJF || policy == SJF){
				for(int i = 0; i < N; i++){
					if(p[i].pid == -1 || p[i].t_exec <= 0) continue;
					if(next == -1) next = i;
					if(p[i].t_exec < p[next].t_exec) next = i;
				}
			}
			else if(policy == RR){
				if(index == -1){
					for(int i = 0; i < N; i++){
						if(p[i].pid != -1 && p[i].t_exec > 0){
							next = i;
							break;
						}
					}
				}
				else if((now_time - last_time) % time_slice == 0){
					next = (index + 1) % N;
					while(p[next].pid == -1 || p[next].t_exec <= 0) next = (next + 1) % N;
				}
				else next = index;
			}
			else if(policy == FIFO){
				for(int i = 0; i < N; i++){
					if(p[i].pid == -1 || p[i].t_exec <= 0) continue;
					if(next == -1) next = i;
					if(p[i].t_ready < p[next].t_ready) next = i;
				}
			}
			else fprintf(stderr,"policy error.\n");
		}
		if(next != -1){
			/*context switch*/
			if(index != next){
				priority(p[next].pid,1);
				priority(p[index].pid,0);
				index = next;
				last_time = now_time;
			}
		}
		//fprintf(stderr,"sec: %d, %s\n",now_time,p[next].name);
		UNIT_TIME();
		if(index > -1) p[index].t_exec--;
		now_time++;
	}	
	return 0;
}





































