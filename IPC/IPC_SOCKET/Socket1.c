#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Message
{
  int8_t Msg[100];
  uint8_t led_Control;
  size_t len;
}Msg_t;

void main(void)
{
  
  Msg_t send_message ={0};
  const char* msg = "This message is sent from process1 to process2";
  memmove(send_message.Msg,msg,strlen(msg));
  send_message.len = strlen(msg);
  send_message.led_Control = 0;  

  int s;
  int var;

  struct sockaddr_in server_address;
  bzero(&server_address, sizeof(server_address));

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr("127.0.0.7");
  server_address.sin_port = htons((u_short) 9876);

  /* create the socket */
  s = socket(PF_INET, SOCK_STREAM, 0);
  connect(s, (struct sockaddr *)&server_address, sizeof(server_address));

  /* send the message */ 
  send(s, (char*)&send_message, sizeof(send_message), 0);

  sleep(2);

  /* receive the message */ 
  Msg_t reveived_msg = {0};
  var = read(s, (char*)&reveived_msg, 1024);
  printf("Inside process 1: %s\tLED status: %s\n",
	reveived_msg.Msg, reveived_msg.led_Control?"ON":"OFF");

  /* close the connection */
  close(s);
}
