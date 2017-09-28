#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <immintrin.h>
#include <stdint.h>

#define ITERATIONS 1000000000
#define EXEC_TIME 20
#define BILLION 1000000000L
#define INSTR 9

void calculate_IOPS (int no_threads);
void calculate_FLOPS (int no_threads);
void run_tensec_exp (void (*method)(void *));
void exec_threads (void *,int);
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

  while (i < ITERATIONS)
  {
    k = k + a1+a2+a3+a4+a5+a6+a7+a8;
    i++;
  }
  printf ("completed thread:%d\n",  param);
  //pthread_exit (NULL);
}

void *
compute_float_op (void *param)
{
  volatile long double i = 0;
  volatile long double k = 2.5;
  volatile double a1=1.0;
  volatile double a2=3.0;
  volatile double a3=4.0;
  volatile double a4=7.0;
  volatile double a5=4.0;
  volatile double a6=8.0;
  volatile double a7=9.0;
  volatile double a8=3.0;


  while (i < ITERATIONS)
  {
    k = k + a1+a2+a3+a4+a5+a6+a7+a8;
    i++; 
  }
  printf ("completed thread:%d\n",  param);
  //pthread_exit (NULL);
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
  uint64_t diff;
  double gflops= 0.0f;

	clock_gettime (CLOCK_REALTIME, &start );
	exec_threads (compute_float_op, no_threads);
	clock_gettime(CLOCK_REALTIME,&end );
	//TODO:FLOPS calculation
	
  diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
  printf("start time : %ld\n", start.tv_sec);
  printf("end time : %ld\n", end.tv_sec);
  printf(" %llu time elapsed in nano seconds\n", diff);
  printf(" %llu time elapsed in seconds\n", end.tv_sec-start.tv_sec);

  gflops = ((long double)ITERATIONS * INSTR) / diff;
  printf("FLOPS = %lf\n",gflops);

}

void run_tensec_exp(void (*method)(void *))
{
  time_t start, end;
  time_t elapsed;
  start = time(NULL);
  int terminate = 1;
  time_t total_elapsed = start + EXEC_TIME;
  printf("%d start\n",ctime(&start));
  while (start < total_elapsed)
  {
    end = time (NULL);
    elapsed = difftime (end,start);
    printf("elapsed time %s",ctime(&start));
    printf("\n%d\n",elapsed);
    if (elapsed >= 1)
    {
      printf("\nSecond passed\n");
      //store clock data
      //printf("second passed\n");
      //total_elapsed += elapsed;
      elapsed = 0.0;
    }
    start = time (NULL);
   // else
   // {
      //total_elapsed += elapsed;
      //printf("elapsed %s\n",total_elapsed);
   // }
   // if(total_elapsed >= EXEC_TIME)
   // {
    //  terminate = 0;
   // }
   // else
   // {
      (*method)(NULL);//calling compute method
   // }
  }
}

void calculate_IOPS (int no_threads)
{

  struct timespec start, end;
  uint64_t diff;
  double giops = 0.0f;

	clock_gettime (CLOCK_REALTIME, &start );
	exec_threads (compute_integer_op, no_threads);
	clock_gettime(CLOCK_REALTIME,&end );
	//TODO:IOPS calculation
	
  diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
  printf("start time : %ld\n", start.tv_sec);
  printf("end time : %ld\n", end.tv_sec);
  printf(" %llu time elapsed in nano seconds\n", diff);
  printf(" %llu time elapsed in seconds\n", end.tv_sec-start.tv_sec);
  giops = ((double)ITERATIONS * INSTR) / diff;
  printf("GIOPS = %lf\n",giops);
}

int main (int argc, char *argv[])
{
 
  int no_threads = 1;// 1,2,4,8
  int operation = 1; // 1 Integer , 2 Float
  int exp = 2; //1 10 sec exp, 2 Thread

  no_threads = atoi (argv[3]);
  operation = atoi (argv[1]);
  exp = atoi (argv[2]);
  
  if(operation ==1)//Integer test
  {
    if(exp==2)//Thread test
    {
       calculate_IOPS(no_threads);
    }
		else
		{
			run_tensec_exp (compute_integer_op);//integer 10 sec exp
		}
	}
	else
	{
		if(exp==2)//Thread test
    {
       calculate_FLOPS (no_threads);
    }
    else
    {
      run_tensec_exp (compute_float_op);//float 10 sec exp
    }
  }
  return 0;	
}
