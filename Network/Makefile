network_benchmark: network_benchmark.o
	gcc -o network_benchmark -flto -O2  network_benchmark.o -lpthread
network_benchmark.o: network_benchmark.s
	gcc -c -flto -O2 network_benchmark.s
network_benchmark.s: network_benchmark.c
	gcc -flto -O2 -S network_benchmark.c
clean:
	rm *.o *.s network_benchmark

