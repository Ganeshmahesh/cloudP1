#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#include <string.h>
#include <immintrin.h>

#define ITERATIONS 1
#define TRANSFER_SIZE 10737418240  //1GB

long thread_operation_count;

long *thread_op_array; //array holding total operations completed by each thread
struct transmission_info_t
{
  struct sockaddr_in server_addr;
  int packet_size;
} transmission_info;

long get_total_op(int no_threads)
{
  long total_op = 0;
  int j;
  for(j = 0; j < no_threads; j++)
  {
    total_op += thread_op_array[j];
  }
  return total_op;
}

void 
exec_threads (void *method, int no_threads);

void exec_netw_perf_test(void* (*method)(void *),int no_threads)
{
  double latency, throughput;
  struct timespec start,end;

  clock_gettime(CLOCK_REALTIME,&start);
  exec_threads(method, no_threads);
  clock_gettime(CLOCK_REALTIME,&end);

  double total_op = get_total_op(no_threads);
 // printf("%lf\n",total_op);
  double total_sec = (double)(end.tv_nsec-start.tv_nsec/1000000000)+(double)end.tv_sec-start.tv_sec;

  //Mb/s
  throughput = ((double)((TRANSFER_SIZE) * 8)/1000000) / (double) total_sec;
//  mesure in ms
  latency = (total_sec / (double) total_op) * 1000;
  //printf("throughput %lf\n",throughput );
  //printf("latency %lf\n",latency);
  fprintf(stdout,"total sec %lf\n",(double)end.tv_sec-start.tv_sec);
}

void exec_threads (void *method, int no_threads)
{
  pthread_t thread[8];
  int i;  
  for (i = 0; i < no_threads; i++)
  {
    pthread_create (&thread[i], NULL, method, (void *)(intptr_t)i);
  }  

  for (i = 0; i < no_threads; i++)
  {
    pthread_join(thread[i],NULL);
  }
}
//client_arg is dummy right now
void *
udp_client(void *client_arg)
{
  int thread_id = (int)(intptr_t)client_arg;
  long thread_total_op =0;
  //init packet
  char *packet = (char *) malloc(sizeof(char) * transmission_info.packet_size);
  memset(packet, 'h', sizeof(char) * (transmission_info.packet_size));

  int client_sock = socket(AF_INET, SOCK_DGRAM, 0);
  int sockaddr_size = sizeof(struct sockaddr_in);
  long i;
  for( i=0; i<thread_operation_count; ++i)
  {
    //printf("serveraddr %d",transmission_info.packet_size);
    int status = sendto(client_sock, packet, transmission_info.packet_size, 0,(struct sockaddr *) &transmission_info.server_addr, sizeof(struct sockaddr_in));

    if (status< 0)
      printf("send failed %s",strerror(errno));
      recvfrom(client_sock, packet, transmission_info.packet_size, 0,(struct sockaddr *) &transmission_info.server_addr, &sockaddr_size);
     // printf("received %s",packet);
      thread_total_op++;
  }
     thread_op_array[thread_id] = thread_total_op;
  
 
}

//client_arg is dummy right now
void *
tcp_client(void *client_arg)
{
  //init packet
  int thread_id = (int)(intptr_t)client_arg;
  long thread_total_op=0;
  char *packet = (char *) malloc(sizeof(char) * transmission_info.packet_size);
  memset(packet, 'h', sizeof(char) * (transmission_info.packet_size));
  //strcpy(packet, "test");
  int client_sock = socket(AF_INET, SOCK_STREAM, 0);
  int connection_error = connect(client_sock,(struct sockaddr *) &transmission_info.server_addr, sizeof(struct sockaddr_in));
  if(connection_error == -1)
  {
    fprintf(stderr,"tcp connection failed\n");
    pthread_exit((void *) (intptr_t)connection_error);
  } 
  long i;
  for( i=0; i<thread_operation_count; ++i)
  {
   int status;
   status =  send(client_sock, packet, transmission_info.packet_size, 0);
   if(status == -1)
   {
     printf("unable to send %s \n",strerror(errno));
     pthread_exit(NULL);
   }
   recv(client_sock,packet,transmission_info.packet_size, 0); 
   
   //printf("%sresponse \n",packet);
   thread_total_op++;
     //thread_op_array[(intptr_t)client_arg] +=1;
  }
//  thread_op_array[thread_id]=thread_total_op;
  close(client_sock);
}

void init_transmission_data(char *server_ip, int server_port, int packet_size)
{
  //init server socket
  transmission_info.server_addr.sin_family = AF_INET;
  transmission_info.server_addr.sin_addr.s_addr = inet_addr(server_ip);
  transmission_info.server_addr.sin_port = htons(server_port);
  
  transmission_info.packet_size = packet_size;
  thread_operation_count = TRANSFER_SIZE/packet_size;

}
 
//arg list param_space no_threada block_size
int main(int argc, char *argv[])
{
  int no_threads = 1;// 1,2,4,8
  int param_space = 1; // 1 TCP , 2 UDP
  long  packet_size = 1; //1 65536 (64kB) 
  long byte_val[1] = {65536};
  char server_ip[14] ;
  int server_port;

  memset(server_ip, '\0', sizeof(server_ip));
  strcpy( server_ip, "127.0.0.1");
  server_port = 4567;


  no_threads = atoi (argv[2]);
  param_space = atoi (argv[1]);
  packet_size = atoi (argv[3]);

  if(param_space == 2)
    packet_size = 65507; 
  else
    //65535
    packet_size = byte_val[packet_size - 1];
  thread_op_array = (long *)malloc(sizeof(long )*no_threads);
  int i;
  for (i = no_threads;i>0;i--)
  {
    thread_op_array = 0;
  }
  //thread must have buff size and server_addr
  init_transmission_data(server_ip,server_port, packet_size);

  if(param_space ==1)//TCP stack
  {
    exec_netw_perf_test( tcp_client, no_threads);
  }
  else //UDP stack
  {
    exec_netw_perf_test( udp_client, no_threads);
  }

 // release_thread_data();
  return 0; 
}

