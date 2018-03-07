#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define PORTNUMBER 9876

typedef struct Message
{
  int8_t Msg[100];
  uint8_t led_Control;
  size_t len;
}Msg_t;

int main(void)
{  
  
  Msg_t msg_send ={0};
  const char* msg = "This message is sent from Process2 to Process1";
  memmove(msg_send.Msg,msg,strlen(msg));
  msg_send.len = strlen(msg);
  msg_send.led_Control = 1; 

  
  int listen_s, connected_s, nbytes;
  struct sockaddr_in server_address, client_address;
  char buf[256]; 
  unsigned long addr;
  int addrlen;

  if ( (listen_s = socket(PF_INET, SOCK_STREAM, 0) ) == -1 )
  {
    perror("ERROR in socket()");
    exit(1);
  }

  bzero(&server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons((u_short)PORTNUMBER);

  if ( bind(listen_s, (struct sockaddr *)&server_address, sizeof(server_address)) == -1 )
  {
    perror("ERROR in bind()");
    exit(2);
  }

  if ( listen(listen_s, 10) == -1 )
  {
    perror("ERROR in listen()");
    exit(3);
  }

  bzero(&client_address, sizeof(client_address));
  addrlen = sizeof(client_address);

  if ( (connected_s = accept(listen_s, (struct sockaddr *)&client_address, &addrlen) ) == -1 )
  {
    perror("ERROR in accept()");
    exit(4);
  }

  printf("Connection successful on %s\n", inet_ntoa(client_address.sin_addr));

  /* receive the message */ 
  Msg_t msg_rec = {0};
  nbytes = recv(connected_s, (char*)&msg_rec, sizeof(msg_rec), 0);
  if (nbytes > 0)
  {
    buf[nbytes] = '\0';
    printf("Inside Process 2: %s\tLED status: %s\n",
	msg_rec.Msg, msg_rec.led_Control?"ON":"OFF");
  }

  
  send(connected_s , (char*)&msg_send, sizeof(msg_send) , 0 );
 
  /* closing the socket */
  close(connected_s);
  close(listen_s);
  exit(0);
}
