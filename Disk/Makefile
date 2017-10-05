disk_benchmark: disk_benchmark.o
	gcc -o disk_benchmark -flto -O2  disk_benchmark.o -lpthread
disk_benchmark.o: disk_benchmark.s
	gcc -c -flto -O2 disk_benchmark.s
disk_benchmark.s: disk_benchmark.c
	gcc -flto -O2 -S disk_benchmark.c
clean:
	rm *.o *.s disk_benchmark

