memory_benchmark: memory_benchmark.o
	gcc -o  -flto -O2  memory_benchmark.o -lpthread
memory_benchmark.o: memory_benchmark.s
	gcc -c -flto -O2 memory_benchmark.s
memory_benchmark.s: memory_benchmark.c
	gcc -flto -O2 -S memory_benchmark.c
clean:
	rm *.o *.s memory_benchmark

