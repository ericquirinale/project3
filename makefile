main: server.c list.c
	gcc -g server.c list.c -o server -lpthread
