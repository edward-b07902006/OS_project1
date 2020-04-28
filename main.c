#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include "scheduler.h"

int cmp(const void *a, const void *b) {
	return ((Proc *)a)->t_ready - ((Proc *)b)->t_ready;
}


int main(int argc,char** argv){
	char policy[128]; 
	int algorithm;
	int N;
	scanf("%s%d",policy,&N);
	Proc p[N];
	for(int i = 0; i < N; i++){
		scanf("%s%d%d",p[i].name,&p[i].t_ready,&p[i].t_exec);
		p[i].pid = -1; // this process is not ready yet
	}

	qsort(p,N,sizeof(Proc),cmp); //sort these processes with ready time

	if(policy[0] == 'F') algorithm = FIFO;
	else if(policy[0] == 'R') algorithm = RR;
	else if(policy[0] == 'S') algorithm = SJF;
	else if(policy[0] == 'P') algorithm = PSJF;
	else{
		fprintf(stderr,"Wrong policy input.");
		exit(0);
	}

	cpu_scheduling(p,algorithm,N);
	return 0;
}
