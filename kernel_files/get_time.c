#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/uaccess.h>

asmlinkage int sys_get_time(unsigned long __user *a,unsigned long __user *b){
	struct timespec t;
	getnstimeofday(&t);
	copy_to_user(a,&t.tv_sec,sizeof(unsigned long));
	copy_to_user(b,&t.tv_nsec,sizeof(unsigned long));
	return 0;
}
