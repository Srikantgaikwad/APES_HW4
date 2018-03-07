#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef struct Message
{
  int8_t Msg[50];
  uint8_t led_Control;
  size_t len;
}Msg_t;

int main()
{

  
  Msg_t msg_send ={0};
  const char* msg = "Message from process2 to process1";
  memmove(msg_send.Msg,msg,strlen(msg));
  msg_send.len = strlen(msg);
  msg_send.led_Control = 1;  

  
  const int SIZE = 4096;
  const char* IPC_ex = "Shared_mem";
  int shm_file;
  void* ptr;
  shm_file = shm_open(IPC_ex, O_RDWR, 0666);
 
  ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_file, 0);
  printf("%s", (char*)ptr);
  sleep(1);
  
  ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_file, 0);

  sprintf(ptr, "Inside Process 1: %s\tLED status: %s\n",
  msg_send.Msg, msg_send.led_Control?"ON":"OFF");
 

  
  shm_unlink(IPC_ex);
  return 0;
}
