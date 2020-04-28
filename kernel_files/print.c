#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage void sys_print(int pid,unsigned long st,unsigned long snt,unsigned et,unsigned ent){
	printk("[Project1] %d %lu.09%lu %lu.09%lu\n",pid,st,snt,et,ent);
	return;
}
