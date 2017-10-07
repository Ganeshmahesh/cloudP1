#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define ITERATIONS 100
/*
void exec_tcp_server(int packet_size,int no_threads)
{
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
}
*/
struct transmission_arguments
{
  int sock;
  int packet_size;
};

void * udpOprtData(void *arg)
{
  struct transmission_args *trans_arg = (struct transmission_args *)arg;
  struct sockaddr_in client;

  char * packet = (char *)malloc(sizeof(char) * trans_arg -> packet_size);
  memset(packet, 0, sizeof(char) * trams_arg -> packet_size);
  int sockaddr_size = sizeof(struct sockaddr_in);

  int i = 0;
  for (i = 0 ; i < ITERATIONS; ++i) {
    recvfrom(trans_arg -> sock, packet, tans_arg -> packet_size, 0, (struct sockaddr *)&client, &sockaddr_size);
  }
  free (packet);
  return NULL;
}


void exec_udp_server(int packet_size,int no_threads)
{
  struct sockaddr_in server;
  struct sockaddr_in client;
  int server_socket;
  int client_socket;
  int bind_success;
  pthread_t thread[no_threads];
  int sockaddr_size = sizeof(struct sockaddr_in);
  int i=0;
  char * packet = (char *) malloc(sizeof(char) * packet_size);
  memset(packet, 0, sizeof(char) * packet_size);

  server.sin_family = af_inet;
  server.sin_addr.s_addr = inaddr_any;
  server.sin_port = htons(4567);

  server_socket = socket(af_inet, sock_dgram, 0);
  if(server_socket == -1)
  {
    exit(-1);
  }

  bind_success = bind(server_sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
  
  if(bind_success == -1)
  {
    fprintf(stderr,"unable to bind\n");
    exit(-1);
  }
   struct transmission_args * trans_arg = (struct transmission_args *)malloc(sizeof(struct transmission_args));
   trans_arg -> sock = server_sock;
   trans_arg -> packet_size = packet_size;

  //listen(server_socket, 10);
  while( i < no_threads)
  {
    pthread_create(&threads[i], null, udpoprtdata, trans_arg);
    i++;
  }
  i=0;
  while(i < no_threads)
  {
    pthread_join(threads[i], null);
    i++;
  }
    close(server_socket);
}


void * tcpOprtData(void *arg)
{
  struct transmission_args *trans_arg = (struct transmission_args *)arg;
  struct sockaddr_in client;

  char * packet = (char *)malloc(sizeof(char) * trans_arg -> packet_size);
  memset(packet, 0, sizeof(char) * thr_arg -> packet_size);
  int sockaddr_size = sizeof(struct sockaddr_in);

  int i = 0;
  for (i = 0 ; i < ITERATIONS; ++i)
  {
    recv(trans_arg -> sock, packet, trans_arg -> buffer_size, 0);
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
  pthread_t thread[no_threads];
  int sockaddr_size = sizeof(struct sockaddr_in);
  int i=0;

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(4567);

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  bind_success = bind(server_sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
  
  if(bind_success == -1)
  {
    fprintf(stderr,"unable to bind\n");
    exit(-1);
  }

  listen(server_socket, 10);
  while( i < no_threads)
  {
    client_socket = accept( server_socket, (struct sockaddr *)&client,&sockaddr_in_size);
    if(client_sock == -1)
    {
      fprintf(stderr,"unable to accept client\n");
      exit(-1);
    }
    struct transmission_args * trans_arg = (struct transmission_args *)malloc(sizeof(struct transmission_args));
    trans_arg -> sock = client_sock;
    trans_arg -> packet_size = packet_size;
   
    pthread_create(&threads[i], NULL, tcpOprtData, trans_arg);
    pthread_join(threads[i], NULL);
    close(client_sock);
    i++;
  }
  close(server_sock);
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
  
  
  long byte_val[1] = {65536};

   packet_size = atoi (argv[3]);
   no_threads = atoi (argv[2]);
   param_space = atoi (argv[1]);

   packet_size = byte_val[packet_size - 1];

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

