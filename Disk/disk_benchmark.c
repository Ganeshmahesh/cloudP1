#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <immintrin.h>
#include <stdint.h>

#define ITERATIONS 1000000000
#define EXEC_TIME 100
#define BILLION 1000000000L
#define INSTR 9
#define FILE_SIZE 10737418240  //10GB
#define EXP_DURATION 10 //20s 

int *thread_op_array; //array holding total operations completed by each thread
char *target_mem_ptr;
char *source_mem_ptr;

struct thread_data_t
{
  FILE *thread_fp;
  long block_size;
  int from;
  int to;
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

void 
exec_threads (void *method, int no_threads, int block_size);

void*
read_write_op(void * param);

void*
seq_read_op(void* param);

void*
rand_read_op(void* param);

void*
read_write_op(void * param)
{
  
  int thread_id = (int )param;
  struct timespec start;
  long start_address;
  long end_address;
  char *src_start_addr;
  char *trgt_start_addr;
  char *src_end_addr;
  char *trgt_end_addr;
  long block_size;
  long thread_total_op = 0;
  long end_time;

  clock_gettime(CLOCK_REALTIME, &start);
  end_time = start.tv_sec + EXP_DURATION;

  block_size = thread_data[thread_id].block_size;
  start_address = thread_data[thread_id].from * block_size;
  end_address = thread_data[thread_id].to * block_size + block_size - 1;
  
 src_start_addr = source_mem_ptr + start_address;
 src_end_addr = source_mem_ptr + end_address;
  
 trgt_start_addr = target_mem_ptr + start_address;
 trgt_end_addr = target_mem_ptr + end_address;
 /*
 printf("thread id: %d\n",thread_id);
 printf("block_size: %ld\n",block_size);
 printf("start_address: %ld\n",start_address);
 printf("end_address: %ld\n\n",end_address);*/

  while(start.tv_sec < end_time)
  {
    memcpy(trgt_start_addr, src_start_addr, block_size);
    clock_gettime(CLOCK_REALTIME, &start);
//    printf("thread %d,trgt_start_addr %p src_start_addr %p time:%ld\n",thread_id,trgt_start_addr,src_start_addr,start.tv_sec);

    trgt_start_addr += block_size;
    src_start_addr += block_size;

    if(trgt_start_addr >= trgt_end_addr)
    {
      trgt_start_addr = target_mem_ptr + start_address;
      src_start_addr = source_mem_ptr + start_address;
    }

    thread_total_op++;
  }
  thread_op_array[thread_id] = thread_total_op;
  printf("thread %d total op %ld\n",thread_id,thread_op_array[thread_id]);
}

void*
seq_read_op(void* param)
{ 
  int thread_id = (int )param;
  struct timespec start;
  long start_address;
  long end_address;
  FILE *fp_start_addr;
  FILE *fp_end_addr;
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
  
  fp_start_addr = thread_data[thread_id].thread_fp + start_address;
  fp_end_addr = thread_data[thread_id].thread_fp + end_address;
 
  while(start.tv_sec < end_time)
  {
    fgets(buff, block_size, fp_start_addr);
    clock_gettime(CLOCK_REALTIME,&start);
    
    lseek(fp_start_addr,start_address , SEEK_SET);
    start_address += block_size;
    if(start_address >= end_address)
    {
      start_address = thread_data[thread_id].from * block_size;
    }

    thread_total_op++; 
    printf("start_address %ld \n",start_address);
  }
  
  thread_op_array[thread_id] = thread_total_op;
  printf("thread %d total op %ld\n",thread_id,thread_op_array[thread_id]);
}

void*
rand_read_op(void* param)
{ 
  int thread_id = (int )param;
  struct timespec start;
  long start_address;
  long end_address;
  FILE *fp_start_addr;
  FILE *fp_end_addr;
  long block_size;
  long thread_total_op = 0;
  long end_time;
  long no_blocks = 0;
  char *buff;
  long from;


  buff = (char *) malloc(block_size);
  
  srand(time(NULL));
  clock_gettime(CLOCK_REALTIME, &start);
  end_time = start.tv_sec + EXP_DURATION;

  block_size = thread_data[thread_id].block_size;
  start_address = thread_data[thread_id].from * block_size;
  end_address = thread_data[thread_id].to * block_size + block_size - 1;
  
  from = thread_data[thread_id].from;

  no_blocks = thread_data[thread_id].to - thread_data[thread_id].from;
 
  fp_start_addr = thread_data[thread_id].thread_fp + start_address;
  fp_end_addr =  thread_data[thread_id].thread_fp  + end_address;
 
  while(start.tv_sec < end_time)
  {
    fgets(buff, block_size, fp_start_addr);
    clock_gettime(CLOCK_REALTIME,&start);
    
    start_address =  (from + (rand()% no_blocks)) * block_size; 
    lseek(fp_start_addr, start_address, SEEK_SET);

    if(start_address >= end_address)
    {
      start_address = thread_data[thread_id].from * block_size;
    }

    thread_total_op++; 
    printf("start_address %ld \n",start_address);
  }
  
  thread_op_array[thread_id] = thread_total_op;
  printf("thread %d total op %ld\n",thread_id,thread_op_array[thread_id]);
} 

void calculate_mem_perf(void* (*method)(void *),int no_threads, long block_size)
{
  double latency, throughput;
  struct timespec start,end;
  clock_gettime(CLOCK_REALTIME,&start);
  exec_threads(method, no_threads, block_size);
  clock_gettime(CLOCK_REALTIME,&end);
  double total_op = get_total_op(no_threads);
  printf("%lf\n",total_op);
  double total_sec = (double)(end.tv_nsec-start.tv_nsec/1000000)+(double)end.tv_sec-start.tv_sec;

  throughput = ((total_op * (double) block_size) / 1048576) / (double) total_sec;
  latency = (total_sec / (double) total_op) * 1000;
  printf("throughput %lf\n",throughput );
  printf("latency %lf\n",latency);
  printf("total sec %lf\n",(double)end.tv_sec-start.tv_sec);

}

void exec_threads (void *method, int no_threads, int block_size)
{
  pthread_t thread[8];
  int i;  
  for (i = 0; i < no_threads; i++)
  {
    pthread_create (&thread[i], NULL, method, (void *)i);
  }  

  for (i = 0; i < no_threads; i++)
  {
    pthread_join(thread[i],NULL);
  }
}
void release_thread_data()
{
   free(target_mem_ptr);
   free(source_mem_ptr);
   free(thread_data);
   free(thread_op_array);
}
void init_thread_data(int no_threads, long block_size)
{
  int i=0;
  int block_count;
  //target_mem_ptr = (char *) malloc(MEMORY_CHUNK_SIZE);//initialize memory
  //source_mem_ptr = (char *) malloc(MEMORY_CHUNK_SIZE);
  thread_data = (struct thread_data_t *)malloc(sizeof(struct thread_data_t) * no_threads);
  FILE *fp = fopen("disk_file.txt","r+");
  block_count = FILE_SIZE / block_size;
  for(i=0;i<no_threads;i++)
  {
    thread_data[i].block_size = block_size;
    thread_data[i].thread_fp = fopen("disk_file.txt","r+");
    thread_data[i].from = i * (block_count / no_threads);
    thread_data[i].to = ((block_count / no_threads) * (i+1))-1;
  }
  thread_op_array= (int*) calloc(no_threads, sizeof(int));
}

//arg list param_space no_threada block_size
int main(int argc, char *argv[])
{
  int no_threads = 1;// 1,2,4,8
  int param_space = 1; // 1 read/write , 2 sequential write, 3 random write
  long  block_size = 1; //1 8B,2 8KB, 3 8MB, 4 80MB 

  no_threads = atoi (argv[2]);
  param_space = atoi (argv[1]);
  block_size = atoi (argv[3]);

  init_thread_data(no_threads,block_size);

  if(param_space ==1)//read/write operation
  {
    calculate_mem_perf( read_write_op,no_threads, block_size);
  }
  else if(param_space == 2)//sequential write operation
  {
    calculate_mem_perf( seq_read_op,no_threads, block_size);
  }
  else//randome write
  {
    calculate_mem_perf( rand_read_op,no_threads, block_size);
  }
  release_thread_data();
  return 0; 
}

