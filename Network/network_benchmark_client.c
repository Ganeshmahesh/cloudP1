#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#include <string.h>
#include <immintrin.h>
//#include <stdint.h>
//#include <stdbool.h>

#define ITERATIONS 1
//#define EXEC_TIME 100
//#define BILLION 1000000000L
//#define INSTR 9
//#define FILE_SIZE 10737418240  //10GB
//#define EXP_DURATION 10 //20s 

//int *thread_op_array; //array holding total operations completed by each thread
//char *target_mem_ptr;
//char *source_mem_ptr;
//bool isThroughPut_op = false;

struct transmission_info_t
{
  struct sockaddr_in server_addr;
  int packet_size;
} transmission_info;
/*
struct thread_data_t
{

  //FILE *thread_fp;
  //long block_size;
  //int from;
  //int to;
} *thread_data;

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
*/
void 
exec_threads (void *method, int no_threads);
/*
void*
read_write_op(void * param);

void*
seq_read_op(void* param);

void*
rand_read_op(void* param);

void*
read_write_op(void * param)
{
  int thread_id = (int )(intptr_t)param;
  struct timespec start;
  long start_address;
  long end_address;
  FILE *fp_start_addr;
  long block_size;
  long thread_total_op = 0;
  long end_time;
  char *buff;
  FILE *src_fp;

  src_fp = fopen("read_write_test_src.txt","r+");
  clock_gettime(CLOCK_REALTIME, &start);
  end_time = start.tv_sec + EXP_DURATION;

  block_size = thread_data[thread_id].block_size;

  buff = (char *) malloc(block_size);

  start_address = thread_data[thread_id].from * block_size;
  end_address = thread_data[thread_id].to * block_size + block_size - 1;
  
  fp_start_addr = thread_data[thread_id].thread_fp;
  lseek(fp_start_addr, start_address, SEEK_SET);//Initial seek to the thread file boundary
 // fp_end_addr = thread_data[thread_id].thread_fp + end_address;
  //inital seek of the src file pointer
  lseek(src_fp, start_address, SEEK_SET);

  int thread_block_complete = 0;   
  while((start.tv_sec < end_time) && thread_block_complete == 0)
  {
    
    lseek(fp_start_addr,start_address , SEEK_SET);
    lseek(src_fp,start_address , SEEK_SET);

    start_address += block_size;
    if(start_address >= end_address)
    {
      start_address = thread_data[thread_id].from * block_size;
      thread_block_complete = 1;
    }
    //copying from source file
    fread(buff, block_size, 1, src_fp);
    //writing contents to target file
    fwrite(buff, block_size, 1, fp_start_addr);

    clock_gettime(CLOCK_REALTIME,&start);
    thread_total_op++; 
    printf("start_address %ld \n",start_address);
  }
  thread_op_array[thread_id] = thread_total_op;
  printf("thread %d total op %ld\n",thread_id,thread_op_array[thread_id]);
}

void*
seq_read_op(void* param)
{ 
  int thread_id = (int )(intptr_t)param;
  struct timespec start;
  long start_address;
  long end_address;
  FILE *fp_start_addr;
  long block_size;
  long thread_total_op = 0;
  long end_time;
  char *buff;

  clock_gettime(CLOCK_REALTIME, &start);
  end_time = start.tv_sec + EXP_DURATION;

  block_size = thread_data[thread_id].block_size;

  buff = (char *) malloc(block_size);

  start_address = thread_data[thread_id].from * block_size;
  end_address = thread_data[thread_id].to * block_size + block_size - 1;
  
  fp_start_addr = thread_data[thread_id].thread_fp;
 // fp_end_addr = thread_data[thread_id].thread_fp + end_address;
  //inital seek of the src file pointer
   
  int thread_block_complete = 0;   
  while((start.tv_sec < end_time) && thread_block_complete == 0)
  {
    
    lseek(fp_start_addr,start_address , SEEK_SET);
    fread(buff, block_size, 1, fp_start_addr);

    start_address += block_size;
    if(start_address >= end_address)
    {
      start_address = thread_data[thread_id].from * block_size;
      thread_block_complete = 1;
    }
    //copying from source file

    clock_gettime(CLOCK_REALTIME,&start);
    thread_total_op++; 
    printf("start_address %ld \n",start_address);
  }
  thread_op_array[thread_id] = thread_total_op;
  
  printf("thread %d total op %ld\n",thread_id,thread_op_array[thread_id]);
}

void*
rand_read_op(void* param)
{ 
  int thread_id = (int )(intptr_t)param;
  struct timespec start;
  long start_address;
  long end_address;
  FILE *fp_start_addr;
  long block_size;
  long thread_total_op = 0;
  long end_time;
  char *buff;
  long from;
  long no_blocks = 0;
  
  srand(time(NULL));
  clock_gettime(CLOCK_REALTIME, &start);

  end_time = start.tv_sec + EXP_DURATION;

  block_size = thread_data[thread_id].block_size;

  buff = (char *) malloc(block_size);

  start_address = thread_data[thread_id].from * block_size;
  end_address = thread_data[thread_id].to * block_size + block_size - 1;
   
  from = thread_data[thread_id].from;
  no_blocks = thread_data[thread_id].to - thread_data[thread_id].from;

  fp_start_addr = thread_data[thread_id].thread_fp;
 // fp_end_addr = thread_data[thread_id].thread_fp + end_address;
  //inital seek of the src file pointer
   
  while(start.tv_sec < end_time)
  {
    
    lseek(fp_start_addr,start_address , SEEK_SET);
    fread(buff, block_size, 1, fp_start_addr);
 
    start_address =  (from + (rand()% no_blocks)) * block_size; 
    if(start_address >= end_address)
    {
      start_address = thread_data[thread_id].from * block_size;
    }
    //copying from source file

    clock_gettime(CLOCK_REALTIME,&start);
    thread_total_op++; 
    printf("start_address %ld \n",start_address);
  }

  thread_op_array[thread_id] = thread_total_op; 
  printf("thread %d total op %ld\n",thread_id,thread_op_array[thread_id]);
} 
*/
void exec_netw_perf_test(void* (*method)(void *),int no_threads)
{
  double latency, throughput;
  struct timespec start,end;

  clock_gettime(CLOCK_REALTIME,&start);
  exec_threads(method, no_threads);
  clock_gettime(CLOCK_REALTIME,&end);

  //double total_op = get_total_op(no_threads);
 // printf("%lf\n",total_op);
  double total_sec = (double)(end.tv_nsec-start.tv_nsec/1000000000)+(double)end.tv_sec-start.tv_sec;

  //throughput = ((total_op * (double) block_size) / 1048576) / (double) total_sec;
  //mesure in ms
 // latency = (total_sec / (double) total_op) * 1000;
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
}/*
void release_thread_data()
{
   free(target_mem_ptr);
   free(source_mem_ptr);
   free(thread_data);
   free(thread_op_array);
}
void init_thread_data(int no_threads, long block_size,  struct sockaddr_in *server)
{
  int i=0;
 // int block_count;
  //target_mem_ptr = (char *) malloc(MEMORY_CHUNK_SIZE);//initialize memory
  //source_mem_ptr = (char *) malloc(MEMORY_CHUNK_SIZE);
  thread_data = (struct thread_data_t *)malloc(sizeof(struct thread_data_t) * no_threads);
  //FILE *fp = fopen("disk_file.txt","r+");
 // block_count = FILE_SIZE / block_size;
  for(i=0;i<no_threads;i++)
  {
    thread_data[i].block_size = block_size;
    thread_data[i].thread_fp = fopen("disk_file.txt","r+");
    thread_data[i].from = i * (block_count / no_threads);
    thread_data[i].to = ((block_count / no_threads) * (i+1))-1;
  }
  thread_op_array= (int*) calloc(no_threads, sizeof(int));
}

udp_client()
{

}
*/
//client_arg is dummy right now
void *
udp_client(void *client_arg)
{
  //init packet
  char *packet = (char *) malloc(sizeof(char) * transmission_info.packet_size);
  memset(packet, 'h', sizeof(char) * (transmission_info.packet_size));

  int client_sock = socket(AF_INET, SOCK_DGRAM, 0);
  int sockaddr_size = sizeof(struct sockaddr_in);
  int i;
  for( i=0; i<ITERATIONS; ++i)
  {
    //printf("serveraddr %d",transmission_info.packet_size);
    int status = sendto(client_sock, packet, transmission_info.packet_size, 0,(struct sockaddr *) &transmission_info.server_addr, sizeof(struct sockaddr_in));

    if (status< 0)
      printf("send failed %s",strerror(errno));
      recvfrom(client_sock, packet, transmission_info.packet_size, 0,(struct sockaddr *) &transmission_info.server_addr, &sockaddr_size);
      printf("received %s",packet);
  }
 
}

//client_arg is dummy right now
void *
tcp_client(void *client_arg)
{
  //init packet
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
  int i;
  for( i=0; i<ITERATIONS; ++i)
  {
   int status;
   status =  send(client_sock, packet, transmission_info.packet_size, 0);
   if(status == -1)
   {
     printf("unable to send %s \n",strerror(errno));
     pthread_exit(NULL);
   }
   recv(client_sock,packet,transmission_info.packet_size, 0); 
   printf("response %s\n",packet);
  }
  
  close(client_sock);
}

void init_transmission_data(char *server_ip, int server_port, int packet_size)
{
  //init server socket
  transmission_info.server_addr.sin_family = AF_INET;
  transmission_info.server_addr.sin_addr.s_addr = inet_addr(server_ip);
  transmission_info.server_addr.sin_port = htons(server_port);
  
  transmission_info.packet_size = packet_size;
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
//  strcpy(server_ip, argv[4]);
 // server_port = atoi (argv[5]);
  /*
  if(block_size == 2 || block_size == 3 || block_size == 4) // compute throughput only if block_size == 2,3,4
  {
    isThroughPut_op = true;
  }

  if(block_size == 1)
  {
    block_size = byte_val[block_size-1];
  }
  else if(block_size == 2)
  {
    block_size = byte_val[block_size-1];
  }
  else if(block_size == 3)
  {
    block_size = byte_val[block_size-1];
  }
  else if(block_size == 4)
  {
    block_size = byte_val[block_size-1];
  }
*/
  if(param_space == 2)
    packet_size = 65507; 
  else
    //65535
    packet_size = byte_val[packet_size - 1];

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

