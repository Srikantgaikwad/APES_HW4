#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

typedef struct Message
{
  int8_t msg[200];
  uint8_t led_Control;
  size_t len;
}Msg_t;

int main(int argc,char* argv[])
{

  
  Msg_t msg_to_C ={0};
  const char* msg_pc = "Hello message from parent to child";
  memmove(msg_to_C.msg,msg_pc,strlen(msg_pc));

  msg_to_C.len = strlen(msg_pc);
  msg_to_C.led_Control = 1;
  
  
  Msg_t msg_to_P ={0};
  const char* msg_cp = "Hello message from child to parent";
  memmove(msg_to_P.msg,msg_cp,strlen(msg_cp));
 
  msg_to_P.len = strlen(msg_to_P.msg);
  msg_to_P.led_Control = 0;



  int c2p[2];
  int p2c[2];
  pipe(c2p);   
  pipe(p2c);   
  

  if(fork() == 0)   
  {
   
    close(c2p[0]);   //closing the pipe for reading for child to parent
    close(p2c[1]);   //closing the pipe for writing for parent to child
     
    /* write data from son process to father process*/
    write(c2p[1],(char*)&msg_to_P, sizeof(msg_to_P));

    /* Structure to receive message from parent */ 
    Msg_t msg_from_P = {0};
    /* read message from parent process */
    read(p2c[0],(char*)&msg_from_P, sizeof(msg_from_P));

    printf("Inside the Child process: %s\tLED status: %s\n",
	msg_from_P.msg, msg_from_P.led_Control?"ON":"OFF");
  }
  else
  {
  
    close(c2p[1]);   //closing the pipe for writing from child to parent
    close(p2c[0]);   //closing the pipe for reading for parent to child
    
  
    write(p2c[1],(char*)&msg_to_C, sizeof(msg_to_C));
    
    /* Structure to receive message from child*/
    Msg_t msg_from_C= {0};        
    
    /* read message from child process*/
    read(c2p[0],(char*)&msg_from_C, sizeof(msg_from_C));
 
    printf("Inside the Parent Process: %s\tLED status: %s\n",
	msg_from_C.msg, msg_from_C.led_Control?"ON":"OFF");
  }
     
    return 0;
}
