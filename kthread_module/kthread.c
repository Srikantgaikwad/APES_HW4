#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/pid.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>



MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module to evaluate process tree lineage");
MODULE_AUTHOR("Srikant Gaikwad");


static int pid = 1;
module_param(pid, int, S_IRUGO);
MODULE_PARM_DESC(pid, "process ID = ");


int number_of_children(struct task_struct *task_find)
{
  struct list_head *node;
  int counter1=0;

  list_for_each(node, &(task_find->children))
  {
    counter1++;
  }
  return counter1;
}



static int kthread_init(void)
{
  struct task_struct *task_find;
  struct pid *pid_struct;
  printk(KERN_INFO "Entering the Kernel module to find the process tree lineage\n");
   
  if(pid==1)
  {
    task_find=current;
    
    for_each_process(task_find)
    {
     //Printing info of all the processes
      printk(KERN_INFO "[Process Information is as follows] Thread Name: %s, Process ID: %d, Process Status: %d, Number of Children: %u, Nice Value: %d", task_find->comm, task_find->pid, task_find->state, number_of_children(task_find), task_nice(task_find));
    }
  }
  else
  {
     task_find=current;
    pid_struct = find_get_pid(pid);
    task_find = pid_task(pid_struct, PIDTYPE_PID);
   
    for_each_process(task_find)
    {
     //Printing the info of all the processes
      printk(KERN_INFO "[Process Information is as follows] Thread Name: %s, Process ID: %d, Process Status: %d, Number of Children: %u, Nice Value: %d", task_find->comm, task_find->pid, task_find->state, number_of_children(task_find), task_nice(task_find));
    }
  }
    
  return 0;
}


static void kthread_exit(void)
{
  printk(KERN_INFO "Kernel module exits after finding the lineage \n");
}

module_init(kthread_init);
module_exit(kthread_exit);
