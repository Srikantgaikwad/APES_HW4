#include <stdlib.h>  
#include <stdio.h>  
#include <string.h> 
#include <sys/types.h>  
#include <mqueue.h>  
#include <stdint.h>  
#include <sys/stat.h>  


#define QUEUE "/Mast_Queue"

typedef struct Message
{
  int8_t Msg[100];
  uint8_t led_Control;
  size_t len;
}Msg_t;
  
int main(int argc, char **argv)  
{  
  
  mqd_t mqueue;  
  struct mq_attr attr; 

  
  Msg_t msg_send ={0};
  const char* msg = "Message sent from process2 to process1";
  memmove(msg_send.Msg,msg,strlen(msg));
  msg_send.len = strlen(msg);
  msg_send.led_Control = 0;  
   
  attr.mq_flags = 0;  
  attr.mq_maxmsg = 10;  
  attr.mq_msgsize = sizeof(Msg_t);  
  attr.mq_curmsgs = 0;  
    
  mqueue = mq_open(QUEUE, O_CREAT | O_RDWR, 0644, &attr);  
  if (mqueue == -1) 
  {
    perror("ERROR");
  }
    
   
  Msg_t msg_rec = {0};

  if(mq_receive(mqueue, (char*)&msg_rec, sizeof(msg_rec),0) == -1)
  {
    perror("QUEUE RECEIVE ERROR"); 
  }
	
  printf("Inside process 2: %s\tLED status: %s\n",
	msg_rec.Msg, msg_rec.led_Control?"ON":"OFF");


  if(mq_send(mqueue, (char*)&msg_send, sizeof(msg_send), 0) == -1)
  {
    perror("QUEUE SEND ERROR");
  }

  mq_close(mqueue);
  mq_unlink(QUEUE);
  
  return 0;  
} 
