#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define DEFAULT_PORT 8080
#define FALSE	0
#define TRUE	1

int listenfd;
extern char *optarg;

int initRedirectServer(int port){
	int sockfd = -1;
	int err = -1;
	int sock_opt = 1;	//for setsockopt()
	sockfd = socket(AF_INET , SOCK_STREAM , 0);

	if (sockfd == -1){
		perror("Error");
		//fprintf(stderr,"Fail to create a socket.\n");
		return -1;
	}

	struct sockaddr_in info;
	bzero(&info,sizeof(info));
	info.sin_family = AF_INET;
	info.sin_addr.s_addr = INADDR_ANY;
	info.sin_port = htons(port);

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&sock_opt, sizeof(sock_opt));

	err = bind(sockfd, (struct sockaddr *)&info, sizeof(info));
	if(err == -1){
		perror("Error");
		close(sockfd);
		return -1;
	}
	
        err = listen(sockfd, 5);
        if(err == -1){
                perror("Error");
                close(sockfd);
                return -1;
        }

	fprintf(stderr,"%s: init socket success, port=%d\n", __FUNCTION__, port);
	return sockfd;
}

void main(int argc, char** argv){
	char c;
	int loop=TRUE;
	fd_set fds;
	struct timeval tv;
	int retval;
	int maxsock;


	//Default Values PATH = ~/ and PORT=10000
	int port=DEFAULT_PORT; 

	//Parsing the command line arguments
	while ((c = getopt (argc, argv, "p:")) != -1){
		switch (c)
		{
			case 'p':
				port=atoi(optarg);
				break;
			case '?':
				fprintf(stderr,"Wrong arguments given!!!\n");
				exit(1);
			default:
				exit(1);
		}
	}

	if((listenfd=initRedirectServer(port)) < 0){
		fprintf(stderr,"Socket init Fail\n");
		return;
	}

	maxsock = listenfd;

	while(loop){
		FD_ZERO(&fds);
		FD_SET(listenfd, &fds);
		maxsock = (maxsock > listenfd)?maxsock: listenfd;

	        /* Wait up to five seconds. */
	        tv.tv_sec = 300;
	        tv.tv_usec = 0;

		retval = select(maxsock + 1, &fds, NULL, NULL, &tv);

		if (retval == -1){
			fprintf(stderr,"select error\n");
			continue;
		}else if(retval == 0){
			fprintf(stderr,"No data within five seconds.\n");
			loop=FALSE;
			continue;
		}else if (FD_ISSET(listenfd, &fds)){
			fprintf(stderr,"Data is available now.\n");
			//loop=FLASE;
			continue;
		}
	}
	
	close(listenfd);

	return;
}
