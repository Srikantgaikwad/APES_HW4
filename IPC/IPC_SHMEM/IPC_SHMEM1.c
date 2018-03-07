#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

typedef struct Message
{
  int8_t Msg[100];
  uint8_t led_Control;
  size_t len;
}Msg_t;

int main()
{
  
  Msg_t msg_send ={0};
  const char* msg = "This message is sent from process1 to process2";
  memmove(msg_send.Msg,msg,strlen(msg));
  msg_send.len = strlen(msg);
  msg_send.led_Control = 0;  
 
  
  const int SIZE = 4096;
 
  
  const char* IPC_ex = "Shared_mem";
 
  
  int shm_file;
 
  
  void* ptr;
 
  shm_file = shm_open(IPC_ex, O_CREAT | O_RDWR, 0666);
 
  ftruncate(shm_file, SIZE);
 
  ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_file, 0);
 
  sprintf(ptr, "Inside Process 2: %s\tLED status: %s\n",
	msg_send.Msg, msg_send.led_Control?"ON":"OFF");
   
  sleep(7);
  
  printf("%s", (char*)ptr);
  shm_unlink(IPC_ex);
  return 0;
}
