#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>
#include <ctype.h>
#include "list.h"

#define BACKLOG 5

int running = 1;
list_t *stor;

// the argument we will pass to the connection-handler threads
struct connection {
    struct sockaddr_storage addr;
    socklen_t addr_len;
    int fd;
};

int server(char *port);
void *echo(void *arg);

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s [port]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

    (void) server(argv[1]);
    return EXIT_SUCCESS;
}

void handler(int signal)
{
	running = 0;
}


int server(char *port)
{
    struct addrinfo hint, *info_list, *info;
    struct connection *con;
    int error, sfd;
    pthread_t tid;

    stor = malloc(sizeof(list_t));
    initLinked(stor);

    // initialize hints
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;
    	// setting AI_PASSIVE means that we want to create a listening socket

    // get socket and address info for listening port
    // - for a listening socket, give NULL as the host name (because the socket is on
    //   the local host)
    error = getaddrinfo(NULL, port, &hint, &info_list);
    if (error != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        return -1;
    }

    // attempt to create socket
    for (info = info_list; info != NULL; info = info->ai_next) {
        sfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

        // if we couldn't create the socket, try the next method
        if (sfd == -1) {
            continue;
        }

        // if we were able to create the socket, try to set it up for
        // incoming connections;
        //
        // note that this requires two steps:
        // - bind associates the socket with the specified port on the local host
        // - listen sets up a queue for incoming connections and allows us to use accept
        if ((bind(sfd, info->ai_addr, info->ai_addrlen) == 0) &&
            (listen(sfd, BACKLOG) == 0)) {
            break;
        }

        // unable to set it up, so try the next method
        close(sfd);
    }

    if (info == NULL) {
        // we reached the end of result without successfuly binding a socket
        fprintf(stderr, "Could not bind\n");
        return -1;
    }

    freeaddrinfo(info_list);

	struct sigaction act;
	act.sa_handler = handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);

	sigset_t mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);


    // at this point sfd is bound and listening
    printf("Waiting for connection\n");
	while (running) {
    	// create argument struct for child thread
		con = malloc(sizeof(struct connection));
        con->addr_len = sizeof(struct sockaddr_storage);
        	// addr_len is a read/write parameter to accept
        	// we set the initial value, saying how much space is available
        	// after the call to accept, this field will contain the actual address length

        // wait for an incoming connection
        con->fd = accept(sfd, (struct sockaddr *) &con->addr, &con->addr_len);
        	// we provide
        	// sfd - the listening socket
        	// &con->addr - a location to write the address of the remote host
        	// &con->addr_len - a location to write the length of the address
        	//
        	// accept will block until a remote host tries to connect
        	// it returns a new socket that can be used to communicate with the remote
        	// host, and writes the address of the remote hist into the provided location

        // if we got back -1, it means something went wrong
        if (con->fd == -1) {
            perror("accept");
            continue;
        }

        // temporarily block SIGINT (child will inherit mask)
        error = pthread_sigmask(SIG_BLOCK, &mask, NULL);
        if (error != 0) {
        	fprintf(stderr, "sigmask: %s\n", strerror(error));
        	abort();
        }

		// spin off a worker thread to handle the remote connection
        error = pthread_create(&tid, NULL, echo, con);

		// if we couldn't spin off the thread, clean up and wait for another connection
        if (error != 0) {
            fprintf(stderr, "Unable to create thread: %d\n", error);
            close(con->fd);
            free(con);
            continue;
        }

		// otherwise, detach the thread and wait for the next connection request
        pthread_detach(tid);

        // unblock SIGINT
        error = pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
        if (error != 0) {
        	fprintf(stderr, "sigmask: %s\n", strerror(error));
        	abort();
        }

    }

	puts("No longer listening.");
	pthread_detach(pthread_self());
	pthread_exit(NULL);

    // never reach here
    return 0;
}

#define BUFSIZE 100

void *echo(void *arg)
{
    char host[100], port[10], buf[BUFSIZE + 1], buf2[100], key[100];
    struct connection *c = (struct connection *) arg;
    int error, nread, nwrite;

	// find out the name and port of the remote host
    error = getnameinfo((struct sockaddr *) &c->addr, c->addr_len, host, 100, port, 10, NI_NUMERICSERV);
    	// we provide:
    	// the address and its length
    	// a buffer to write the host name, and its length
    	// a buffer to write the port (as a string), and its length
    	// flags, in this case saying that we want the port as a number, not a service name
    if (error != 0) {
        fprintf(stderr, "getnameinfo: %s", gai_strerror(error));
        close(c->fd);
        return NULL;
    }

    printf("[%s:%s] connection\n", host, port);

    while ((nread = read(c->fd, buf, BUFSIZE)) > 0) {
        buf[nread] = '\0'; //buf holds the command
        char cmd[4]; //holds command (get, set, del)
        for (size_t i = 0; i < 3; i++) {
          cmd[i] = toupper(buf[i]);
        }
        cmd[3] = '\0';
        int counter = 0; //counter for number of newlines (breaks payload into sections)
        char tmp[100]; // for storing key argument
        int tmpC = 0; //counter for above
        char tmp2[100]; // for storing value argument in set
        int tmpC2 = 0; //counter for above
        char byteLength[100]; //holds byteLength
        char *value; //holds function return (get, set, del)

        if (strcmp(cmd, "GET")==0) {
          nread = read(c->fd, byteLength, BUFSIZE); //read byteLength

          nread = read(c->fd, tmp, BUFSIZE); //read key
          char *tmpKey = malloc(sizeof(tmp));
          tmpKey=tmp;

          value = get(stor, tmpKey);

          if (atoi(byteLength)!=strlen(tmpKey)) { //check for length errors
            write(c->fd, "LEN\n", 4);
            close(c->fd);
            free(c);
          }

          write(c->fd, value, strlen(value));
        }
        else if (strcmp(cmd, "SET")==0) {
          nread = read(c->fd, byteLength, BUFSIZE); //read byteLength

          nread = read(c->fd, tmp, BUFSIZE); //read key

          char *tmpKey = malloc(sizeof(tmp));
          tmpKey=tmp;

          nread = read(c->fd, tmp2, BUFSIZE); //read value

          char *tmpVal = malloc(sizeof(tmp2));
          tmpVal=tmp2;

          if (atoi(byteLength)!=strlen(tmpKey)+strlen(tmpVal)) { //check for length errors
            write(c->fd, "LEN\n", 4);
            close(c->fd);
            free(c);
          }
          value = set(stor, tmpKey, tmpVal);

          //write(c->fd, value, strlen(value));
          write(c->fd, "OKS\n", 4);

        }
        else if (strcmp(cmd, "DEL")==0) {
          char byteLength[100];
          nread = read(c->fd, byteLength, BUFSIZE); //read byteLength

          nread = read(c->fd, tmp, BUFSIZE); //read key

          char *tmpKey = malloc(sizeof(tmp));
          tmpKey=tmp;

          value = del(stor, tmpKey);

          if (atoi(byteLength)!=strlen(tmpKey)) { //check for length errors
            write(c->fd, "LEN\n", 4);
            close(c->fd);
            free(c);
          }

          write(c->fd, value, strlen(value));
        }
        else{
          write(c->fd, "BAD\n", 4); //bad command error
          close(c->fd);
          free(c);
          return NULL;
        }
    }



    printf("[%s:%s] got EOF\n", host, port);

    close(c->fd);
    free(c);
    return NULL;
}
