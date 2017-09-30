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
#define MEMORY_CHUNK_SIZE 1000000000  //1GB

long` get_total_op(int no_threads)
{
  long total_op = 0;
  int j;
  for(j = 0; j < no_threads; J++)
  {
    total_op += thread_op_array[i];
  }
  return total_op;
}

int *thread_op_array; //array holding total operations completed by each thread
char *mem_ptr;

void allocate_memory_chunks(int no_threads)
{
  int i;
  
  mem_ptr = calloc(no_threads,sizof(char));
  for(i = 0;i < no_threads; i++)
  {
    mem_ptr[i] = (char *) malloc(sizeof(char)*MEMORY_CHUNK_SIZE);
  }

}

void*
read_write_op(void * param);
void*
seq_write_op(void* param);
void*
rand_write_op(void* param);

void*
read_write_op(void * param)
{
}
void*
seq_write_op(void* param)
{
}
void*
rand_write_op(void* param)
{
  
}

void calculate_mem_perf(void (*method)(void *),int no_threads, int block_size)
{
  double latency, throughput;
  struct timespec start,end;

  clock_gettime(CLOCK_REALTIME,&start);
  exec_threads(method, no_threads, block_size);
  clock_gettime(CLOCK_REALTIME,&end);
}

void exec_threads (void *method, int no_threads)
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

void init_thread_data(int no_threads)
{
  allocate_memory_chunks( no_threads); //initializing memory
  thread_op_array= (int*) calloc(no_threads, sizeof(int));
}

//arg list param_space no_threada block_size
int main(int argc, int *argv[])
{
  int no_threads = 1;// 1,2,4,8
  int param_space = 1; // 1 read/write , 2 sequential write, 3 random write
  int block_size = 1; //1 8B,2 8KB, 3 8MB, 4 80MB 

  no_threads = atoi (argv[2]);
  param_space = atoi (argv[1]);
  block_size = atoi (argv[3]);

  init_thread_data(no_threads);

  if(param_space ==1)//read/write operation
  {
    calculate_mem_perf( read_write_op,no_threads, block_size);
  }
  else if(param_space == 2)//sequential write operation
  {
    calculate_mem_perf( seq_write_op,no_threads, block_size);
  }
  else//randome write
  {
    calculate_mem_perf( ramd_write_op,no_threads, block_size);
  }
  return 0; 
}

