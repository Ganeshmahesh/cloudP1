test: test.o
	gcc -mavx2 -o test -flto -O2  test.o -lpthread
test.o: test.s
	gcc -c -flto -O2 test.s
test.s: test.c
	gcc -mavx2 -flto -O2 -S test.c
clean:
	rm *.o *.s test

