#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define ITERATIONS 1
#define TRANSFER_SIZE 10737418240 //1GB

long thread_operation_count;
struct transmission_arguments
{
  int sock;
  int packet_size;
};

void * udpOprtData(void *arg)
{
  //printf("udpOprtData\n");
  struct transmission_arguments *trans_arg = (struct transmission_arguments *)arg;
  struct sockaddr_in client;

  char * packet = (char *)malloc(sizeof(char) * trans_arg -> packet_size);
  memset(packet, 'x', sizeof(char) * trans_arg -> packet_size);
  int sockaddr_size = sizeof(struct sockaddr_in);
//printf("%d",trans_arg->packet_size);
  int i = 0;
  for (i = 0 ; i < thread_operation_count; ++i) 
  {
    //:printf("server addr %ld\n",trans_arg->sock);
    int status =recvfrom(trans_arg -> sock, packet, trans_arg -> packet_size, 0, (struct sockaddr *)&client, &sockaddr_size);
    if(status == -1)
    {
      printf("error info %s",strerror(errno));
      pthread_exit(NULL);
    }
    else
    //  printf("%s oh yeah baby !\n",packet);
    //  printf("exiting");
    //
    memset(packet, 'z',sizeof(char)*trans_arg->packet_size);
 status =   sendto(trans_arg->sock, packet, trans_arg->packet_size, 0, (struct sockaddr *)&client, sockaddr_size);
    if(status ==-1)
    {
      printf("%s\n",strerror(errno));
    }
  }

  free (packet);
  return NULL;
}


void exec_udp_server(int packet_size,int no_threads)
{
  //printf("exec_udp_server");
  struct sockaddr_in server;
  struct sockaddr_in client;
  int server_socket;
  int client_socket;
  int bind_success;
  pthread_t threads[no_threads];
  int sockaddr_size = sizeof(struct sockaddr_in);
  int i=0;
  char * packet = (char *) malloc(sizeof(char) * packet_size);
  memset(packet, 'a', sizeof(char) * packet_size);

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  server.sin_port = htons(4567);

  server_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if(server_socket == -1)
  {
    exit(-1);
  }

  bind_success = bind(server_socket, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
  
  if(bind_success == -1)
  {
    fprintf(stderr,"unable to bind\n");
    exit(-1);
  }
   struct transmission_arguments * trans_arg = (struct transmission_arguments *)malloc(sizeof(struct transmission_arguments));
   trans_arg -> sock = server_socket;
   trans_arg -> packet_size = packet_size;

  //listen(server_socket, 10);
  while( i < no_threads)
  {
    pthread_create(&threads[i], NULL, udpOprtData, trans_arg);
    i++;
  }
  i=0;
  while(i < no_threads)
  {
    pthread_join(threads[i], NULL);
    i++;
  }
    close(server_socket);
}


void * tcpOprtData(void *arg)
{
  struct transmission_arguments *trans_arg = (struct transmission_arguments *)arg;
  struct sockaddr_in client;

  char * packet = (char *)malloc(sizeof(char) * trans_arg -> packet_size);
  memset(packet, 'o', sizeof(char) * trans_arg -> packet_size);
  int sockaddr_size = sizeof(struct sockaddr_in);

  int i = 0;
  for (i = 0 ; i < thread_operation_count; ++i)
  {
    int status = recv(trans_arg -> sock, packet, trans_arg -> packet_size, 0);

    if( status > 0)
    {
      //printf("arrived packet %s oh yeah!\n",packet);
      memset(packet, 'g', sizeof(char) * trans_arg-> packet_size);
      send(trans_arg ->sock, packet, trans_arg->packet_size,0);
    }
       if(status == -1)
       {
         printf("unable to send %s \n",strerror(errno));
       }

  }
  free(packet);
  return NULL;
}


void exec_tcp_server(int packet_size,int no_threads)
{
  struct sockaddr_in server;
  struct sockaddr_in client;
  int server_socket;
  int client_socket;
  int bind_success;
  pthread_t threads[no_threads];
  int sockaddr_size = sizeof(struct sockaddr_in);
  int i=0;

  //initialize server
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(4567);

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  bind_success = bind(server_socket, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
  
  if(bind_success == -1)
  {
    fprintf(stderr,"unable to bind\n");
    exit(-1);
  }

  listen(server_socket, 10);
  while( i < no_threads)
  {
    client_socket = accept( server_socket, (struct sockaddr *)&client,&sockaddr_size);
    if(client_socket == -1)
    {
      fprintf(stderr,"unable to accept client\n");
      exit(-1);
    }
    struct transmission_arguments * trans_arg = (struct transmission_arguments *)malloc(sizeof(struct transmission_arguments));
    trans_arg -> sock = client_socket;
    trans_arg -> packet_size = packet_size;
   
    pthread_create(&threads[i], NULL, tcpOprtData, trans_arg);
    pthread_join(threads[i], NULL);
    close(client_socket);
    i++;
  }
  close(server_socket);
}
/*
  double latency, throughput;
  struct timespec start,end;

  //clock_gettime(CLOCK_REALTIME,&start);
  exec_threads(method, no_threads);
  //clock_gettime(CLOCK_REALTIME,&end);

  //double total_op = get_total_op(no_threads);
  // // printf("%lf\n",total_op);
  //double total_sec = (double)(end.tv_nsec-start.tv_nsec/1000000000)+(double)end.tv_sec-start.tv_sec;
  //
  //     //throughput = ((total_op * (double) block_size) / 1048576) / (double) total_sec;
  //       //mesure in ms
  //        // latency = (total_sec / (double) total_op) * 1000;
  //          //printf("throughput %lf\n",throughput );
  //            //printf("latency %lf\n",latency);
   fprintf(stdout,"total sec %lf\n",(double)end.tv_sec-start.tv_sec);
}*/
int main(int argc, char *argv[])
{
  int packet_size = 1;//1 65536 (64kb)
  int no_threads = 1;//1, 2, 4, 8
  int param_space = 1; //1 TCP, 2 UDP
  
  
  int byte_val[1] = {65536};

   packet_size = atoi (argv[3]);
   no_threads = atoi (argv[2]);
   param_space = atoi (argv[1]);

   if(param_space ==2)
   {
     packet_size =65507;
   }
   else
   {
     packet_size = byte_val[packet_size - 1];
   }
   thread_operation_count = TRANSFER_SIZE/packet_size;
   if( param_space == 1) //TCP stack
   {
     exec_tcp_server( packet_size, no_threads);
   }
   else //UDP stack 
   {  
     exec_udp_server( packet_size, no_threads);
   }
  return 0;
}

