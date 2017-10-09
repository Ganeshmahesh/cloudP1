network_benchmark_server: network_benchmark_server.o
	gcc -o network_benchmark_server -flto -O2  network_benchmark_server.o -lpthread
network_benchmark_server.o: network_benchmark_server.s
	gcc -c -flto -O2 network_benchmark_server.s
network_benchmark_server.s: network_benchmark_server.c
	gcc -flto -O2 -S network_benchmark_server.c
clean:
	rm *.o *.s network_benchmark_server

