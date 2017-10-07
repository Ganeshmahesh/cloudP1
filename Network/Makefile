network_benchmark_client: network_benchmark_client.o
	gcc -o network_benchmark_client -flto -O2  network_benchmark_client.o -lpthread
network_benchmark_client.o: network_benchmark_client.s
	gcc -c -flto -O2 network_benchmark_client.s
network_benchmark_client.s: network_benchmark_client.c
	gcc -flto -O2 -S network_benchmark_client.c
clean:
	rm *.o *.s network_benchmark_client

