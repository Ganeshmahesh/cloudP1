#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <immintrin.h>
#include <stdint.h>

//#define ITERATIONS 1000000000
#define EXEC_TIME 100
#define BILLION 1000000000L
#define INSTR 24

long ITERATIONS;
FILE *fp=NULL;
//long c[8];
void calculate_IOPS (int no_threads);
void calculate_FLOPS (int no_threads);
//void run_tensec_exp (void (*method)(void *), int no_threads);
void exec_threads (void *,int);
//void *tensecexp_exec(void *param);

void *
compute_integer_op (void *param)
{
  volatile unsigned long i = 0;
  volatile unsigned long k = 100;
  volatile unsigned int a1=1;
  volatile unsigned int a2=3;
  volatile unsigned int a3=4;
  volatile unsigned int a4=7;
  volatile unsigned int a5=4;
  volatile unsigned int a6=8;
  volatile unsigned int a7=9;
  volatile unsigned int a8=3;

  volatile unsigned int a9=3;
  volatile unsigned int a10=3;
  volatile unsigned int a11=3;
  volatile unsigned int a12=3;
  volatile unsigned int a13=3;
  volatile unsigned int a14=3;
  volatile unsigned int a15=3;
  volatile unsigned int a16=3;
  volatile unsigned int a17=3;
  volatile unsigned int a18=3;
  volatile unsigned int a19=3;
  volatile unsigned int a20=3;
  volatile unsigned int a21=3;
  volatile unsigned int a22=3;
  volatile unsigned int a23=3;

  while (i < ITERATIONS)
  {
    k = k + a1+a2+a3+a4-a5+a6+a7*a8+a9+a10+a11*a12+a13+a14/a15+a16+a17
          +a18+a19+a20+a21+a22+a23;
    i++;
  }
  //printf ("completed thread:%d\n", (intptr_t) param);
  //pthread_exit (NULL);
}
/*
struct tensec_args_struct
{
  void (*method)(void *);
  int index;
};

void *
tensecexp_exec (void *param)
{
  struct tensec_args_struct *arg=(struct tensec_args_struct * ) param;
  while(1)
  {
  (* (arg-> method)) (1);
   c[arg->index] += INSTR;
   //debugging
   printf("argIndex=%d\n",arg->index);
   printf("c[arg->index] %ld\n",c[arg->index]);
  }
}
*/
void *
compute_float_op (void *param)
{
  volatile long double i = 0.0;
  volatile long double k = 2.5;
  volatile double a1=1.0;
  volatile double a2=3.0;
  volatile double a3=4.0;
  volatile double a4=7.0;
  volatile double a5=4.0;
  volatile double a6=8.0;
  volatile double a7=9.0;
  volatile double a8=3.0;
  volatile double a9=9.0;
  volatile double a10=10.0;
  volatile double a11=11.0;
  volatile double a12=12.0;
  volatile double a13=13.0;
  volatile double a14=14.0;
  volatile double a15=15.0;
  volatile double a16=16.0;
  volatile double a17=17.0;
  volatile double a18=18.0;
  volatile double a19=19.0;
  volatile double a20=20.0;
  volatile double a21=21.0;
  volatile double a22=22.0;
  volatile double a23=23.0;
                        
  while (i < ITERATIONS)
  {
    k = k + a1+a2+a3+a4+a5+a6+a7-a8+a9+a10+a11+a12*a13*a14+a15/a16+a17
      +a18+a19+a20+a21+a22+a23;
    i++;
  }

  //printf ("completed thread:%d\n", (intptr_t) param);
  //pthread_exit (NULL);
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

void *
compute_avx (void *param)
{
  __m256i operand1 = _mm256_set_epi8(8,9,6,5,6,7,9,5,
				     8,9,6,5,6,7,9,5,
				     8,9,6,5,6,7,9,5,
				     8,9,6,5,6,7,9,5);

  __m256i operand2 = _mm256_set_epi8(27,29,46,55,36,37,
				     59,25,27,29,46,55,
				     36,37,59,25,27,29,
				     46,55,36,37,59,25,
				     27,29,46,55,36,37,
 				     27,29);
  int k;
  __m256i result;
  for (k = 0; k < 1000000000; k++)
  {
    result = _mm256_add_epi8 (operand1, operand2);
    result = _mm256_add_epi8 (operand1, operand2);
    result = _mm256_add_epi8 (operand1, operand2);
    result = _mm256_add_epi8 (operand1, operand2);
  }
}

void calculate_FLOPS (int no_threads)
{
  struct timespec start, end;
  uint64_t  total_time;
  long double gflops= 0.0f;

	clock_gettime (CLOCK_REALTIME, &start );
	exec_threads (compute_float_op, no_threads);
	clock_gettime(CLOCK_REALTIME, &end);

	//TODO:verify FLOPS calculation
  //time in sec
  total_time = BILLION * (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec);
/*
  printf("start time : %ld\n", start.tv_sec);
  printf("end time : %ld\n", end.tv_sec);
  printf("nano seconds %ld %ld\n",end.tv_nsec, start.tv_nsec);
  printf(" %lld time elapsed in nano seconds\n", total_time);
  printf(" %lld time elapsed in seconds\n", ((BILLION * end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)));
*/
  gflops = ((long double)BILLION * INSTR) / total_time;
 // printf("FLOPS = %llf\n",gflops);
  fprintf(fp,"%d  %llf\n",no_threads,gflops);
}

/*
void *
time_tracker(void *k)
{
  struct timespec start;
  clock_gettime(CLOCK_REALTIME,&start);
  long total_elapsed = start.tv_sec + EXEC_TIME;
  int i=0;
  while ((start.tv_sec < total_elapsed )&& i < 600)
  {
    sleep(1);
    printf("total instructions %ld\n",(c[0]+c[1]+c[2]+c[3]+c[4]+c[5]+c[6]+c[7]));
    clock_gettime (CLOCK_REALTIME,&start);
    i++;
  }
  
}

void run_tensec_exp(void (*method)(void *),int no_threads)
{
 // struct timespec start, end;
 // clock_gettime(CLOCK_REALTIME,&start);
 // long total_elapsed = start.tv_sec + EXEC_TIME;
  pthread_t threads[no_threads];
  pthread_t counter_thread;
  struct tensec_args_struct tensec_args[no_threads];
  int i=0;
  for(i=0;i<no_threads;i++)
  {
     tensec_args[i].method = method;
     tensec_args[i].index = i;
     pthread_create (&threads[i], NULL, tensecexp_exec, (void *)&(tensec_args[i]));
  }

  pthread_create (&counter_thread,NULL,time_tracker,(void *)i);
  i=0;
  pthread_join(counter_thread,NULL);
 for(i=0;i<no_threads;i++)
  {
    pthread_kill(&threads[i],1);
  }
  printf("600 samples taken from 10 min exp");
}
*/
void calculate_IOPS (int no_threads)
{

  struct timespec start, end;
  uint64_t total_time;
  long double iops = 0.0f;

	clock_gettime (CLOCK_REALTIME, &start );
	exec_threads (compute_integer_op, no_threads);
	//exec_threads (compute_avx, no_threads);
	clock_gettime(CLOCK_REALTIME,&end );
	//TODO:IOPS calculation
	//total time including the nanosec difference
  total_time = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
  //printf("start time : %ld\n", start.tv_sec);
  //printf("end time : %ld\n", end.tv_sec);
  //printf(" %llu time elapsed in nano seconds\n", total_time);
  //printf(" %llu time elapsed in seconds\n", end.tv_sec-start.tv_sec);
  iops = ((double)BILLION * INSTR) / total_time;
  //printf("iops %llf\n",iops);
  fprintf(fp,"%d  %llf\n",no_threads,iops);
}

int main (int argc, char *argv[])
{ 
  int no_threads = 1;// 1,2,4,8
  int operation = 1; // 1 Integer , 2 Float
  int exp = 2; //1 10 sec exp, 2 Thread

  no_threads = atoi (argv[3]);
  operation = atoi (argv[1]);
  exp = atoi (argv[2]);

  ITERATIONS = BILLION / no_threads;

  if(operation ==1)//Integer test
  {
    if(exp==2)//Thread test
    {
       fp = fopen("IOPS","a+");
       calculate_IOPS(no_threads);
    }
		else
		{
			//run_tensec_exp (compute_integer_op,8);//integer 10 sec exp
		}
	}
	else
	{
		if(exp==2)//Thread test
    {
      fp = fopen("FLOPS","a+");
      calculate_FLOPS (no_threads);
    }
    else
    {
     // run_tensec_exp (compute_float_op,8);//float 10 sec exp
    }
  }
  return 0;	
}
