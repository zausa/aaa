all:
	gcc -pthread -o mu multithread.c
	gcc -pthread -o client client.c
	./mu
clean:
	rm client
	rm mu
