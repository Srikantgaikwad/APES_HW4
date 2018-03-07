#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kfifo.h>
#include <linux/kthread.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/sched.h>

/* Configure the Module License, Descrption and Authorship */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Information transfer in kthreads using kfifo");
MODULE_AUTHOR("Srikant Gaikwad");


static struct task_struct *th_1;

static struct task_struct *th_2;

static struct task_struct *info;

static DECLARE_KFIFO(kfifo, struct task_struct*, 32);
static DEFINE_MUTEX(lock_fifo);

static int th_1_func(void *spock)
{
  while (!kthread_should_stop())
  {
    printk(KERN_INFO "Entering Thread 1 which generates information and put it into kfifo\n");
   
    mutex_lock(&lock_fifo);

    kfifo_put(&kfifo, current);

    mutex_unlock(&lock_fifo);
    
   //schedule_timeout(HZ*10);
    ssleep(3);
  }

  printk(KERN_INFO "Exiting Thread 1 \n");
  return 0;
}

static int th_2_func(void *spock)
{
  while (!kthread_should_stop())
  {
    while (!kfifo_is_empty(&kfifo)) 
    {
      printk(KERN_INFO "Entering Thread 2 which uses info from thread 1 and put it in kernel logger\n");
      
      mutex_lock(&lock_fifo);
      
      kfifo_get(&kfifo, &info);

      printk(KERN_INFO "Previous process -> PID: %d,  vruntime: %llu\n", 
             list_prev_entry(info, tasks)->pid, list_prev_entry(info, tasks)->se.vruntime);
      printk(KERN_INFO "Current Process -> PID: %d,  vruntime: %llu\n",	
             info->pid, info->se.vruntime);
      printk(KERN_INFO "Next Process -> PID: %d, vruntime: %llu\n",
             list_next_entry(info, tasks)->pid,list_next_entry(info, tasks)->se.vruntime);

      mutex_unlock(&lock_fifo);
      ssleep(3);
    }
  }

  printk(KERN_INFO "Exit Thread 2\n");
  return 0;
}


int queue_enter(void)
{

  printk(KERN_INFO "Load kernel module");

  INIT_KFIFO(kfifo);

  th_1 = kthread_create(th_1_func, NULL, "Thread 1");
  if(IS_ERR(th_1))	
  {
    printk(KERN_ERR "Error in thread 1.\n");
    return -1;
  }
  else
  {
    wake_up_process(th_1);
  }

  
  th_2 = kthread_create(th_2_func, NULL, "Thread 2");
  if(IS_ERR(th_2))	
  {
    printk(KERN_ERR "Error in thread 2.\n");
    return -1;
  }
  else
  {
    wake_up_process(th_2);    
  }

  return 0;
}

void queue_exit(void)
{

  kthread_stop(th_1);
  kthread_stop(th_2);
  kfifo_free(&kfifo);
  
  printk(KERN_INFO "Removing the module which was generating information regarding current process");
}

module_init(queue_enter);
module_exit(queue_exit);
