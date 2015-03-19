/* net_select.c */
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <semaphore.h>




#define PORT				4321
#define MAX_QUE_CONN_NM			5
#define MAX_SOCK_FD			FD_SETSIZE
#define BUFFER_SIZE			1024
int sockfd;
char msg[1024];
sem_t   full,avail;
fd_set inset, tmp_inset;



void *thsend(){

 int fd;
	 char tmp[1024];
puts("11");
 while (1) {
	 sem_wait(&full);
	 puts("12");

strcpy(tmp,msg);
sem_post(&avail);
 		/*调用select函数*/
 		if (!(select(MAX_SOCK_FD,NULL,  &tmp_inset, NULL, NULL) > 0)) {
 			perror("select");
 			close(sockfd);
 		//	exit(1);
 		}

 		for ( fd = 0; fd < MAX_SOCK_FD; fd++) {
 			if (FD_ISSET(fd, &tmp_inset) > 0) {

 					if (fd > sockfd)
 				{
puts("puts");
puts(tmp);
 						 write(fd, tmp, 512);


 				}
 			}
 		}


 	}

 puts("14");
pthread_exit(NULL);


}


void *threcv() {
	struct sockaddr_in server_sockaddr, client_sockaddr;
	int sin_size, count, sendbytes;
//	fd_set inset, tmp_inset;
	int sockfd, client_fd,fd;
	char buf[BUFFER_SIZE];
puts("9");
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
	//	exit(1);
	}

puts("7");
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(PORT);
	server_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_sockaddr.sin_zero), 8);
	puts("13");

	int i = 1;/* 使得重复使用本地地址与套接字进行绑定 */
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));
	if (bind(sockfd, (struct sockaddr *) &server_sockaddr,
			sizeof(struct sockaddr)) == -1) {
		perror("bind");
	//	exit(1);
	}

	if (listen(sockfd, MAX_QUE_CONN_NM) == -1) {
		perror("listen");
	//	exit(1);
	}
	printf("listening....\n");

	/*将调用socket函数的描述符作为文件描述符*/
	FD_ZERO(&inset);
	FD_SET(sockfd, &inset);

	while (1) {
		puts("5");
		tmp_inset = inset;
		sin_size = sizeof(struct sockaddr_in);
		memset(buf, 0, sizeof(buf));
		/*调用select函数*/
		if (!(select(MAX_SOCK_FD, &tmp_inset, NULL, NULL, NULL) > 0)) {
			perror("select");
			close(sockfd);
		//	exit(1);
		}
		puts("6");

		for (fd = 0; fd < MAX_SOCK_FD; fd++) {
			if (FD_ISSET(fd, &tmp_inset) > 0) {
				if (fd == sockfd) { /* 服务端接收客户端的连接请求 */
					if ((client_fd = accept(sockfd,
							(struct sockaddr *) &client_sockaddr, &sin_size))
							== -1) {
						perror("accept");
		//				exit(1);
					}
					FD_SET(client_fd, &inset);
					printf("New connection from %d(socket)\n", client_fd);
				} else /* 处理从客户端发来的消息 */
				{


					if ((count = recv(fd, buf, BUFFER_SIZE, 0)) > 0) {
						printf("Received a message from %d: %s\n", fd, buf);

						puts("test1");
sem_wait(&avail);


strcpy(msg,buf);
sem_post(&full);
puts("msg");
puts(msg);


					} else {
						close(fd);
						FD_CLR(fd, &inset);
						printf("Client %d(socket) has left\n", fd);
					}
				}
			} /* end of if FD_ISSET*/
		} /* end of for fd*/
	} /* end if while while*/
	close(fd);


	close(sockfd);
	pthread_exit(NULL);


}

int main() {
pthread_t   send;
pthread_t   recv;


sem_init(&avail,0,1);
puts("0");
sem_init(&full,0,0);
puts("1");
pthread_create(&recv,NULL,threcv,NULL);
puts("2");
pthread_create(&send,NULL,thsend,NULL);
puts("3");
pthread_join(&recv,NULL);
pthread_join(&send,NULL);


return 0;

}
