/*client.c*/

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>



#define PORT	4321
#define BUFFER_SIZE 1024
int sockfd;
sem_t   start;

void *thsend(void *argv)
{

int  sendbytes,recvbytes;
	char buf[BUFFER_SIZE];
	struct hostent *host;
	struct sockaddr_in serv_addr;
	char  str[512];
	//if(argc < 3)
//	{
	//	fprintf(stderr,"USAGE: ./client Hostname(or ip address) Text\n");
	//	exit(1);
//	}
	if((char *)argv==NULL)
		{
		fprintf(stderr,"USAGE: ./client Hostname(or ip address) Text\n");
	//	exit(1);
		}
strcpy(str,argv);
	/*地址解析函数*/
printf("%s",str);
	if ((host = gethostbyname(argv)) == NULL) {
		perror("gethostbyname");
	//	exit(1);
	}
	puts("test0");
	memset(buf, 0, sizeof(buf));
//	sprintf(buf, "%s", argv[2]);

	/*创建socket*/
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
	//	exit(1);
	}

	/*设置sockaddr_in 结构体中相关参数*/
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr = *((struct in_addr *) host->h_addr);
	bzero(&(serv_addr.sin_zero), 8);

	/*调用connect函数主动发起对服务器端的连接*/
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr))
			== -1) {
		perror("connect");
	//	exit(1);
	}

	/*发送消息给服务器端*/
sem_post(&start);
	do {
		puts("test0");

		scanf("%s", buf);
//strcpy(buf,"六年了，我们始终心怀汶川，我们见证汶川穿越灾难、走向新生，见证幸存者的不屈和成长。这是一场不能忘却的纪念。新汶川，新面貌！汶川加油！我们为你祈福！\n");
		if ((sendbytes = send(sockfd, buf, strlen(buf), 0)) == -1)
		{

			fprintf(stderr, "USAGE: ./client Hostname(or ip address) Text\n");

			perror("send");
		//	exit(1);

	}
	}while (strcmp(buf, "quit"));
	sleep(3);
	close(sockfd);
	//exit(0);














}


void  *threcv(){
	int recvbytes;
	char str[512];
	puts("000");
	sem_wait(&start);

	   while(1)
		{
			puts("001");

		   if((recvbytes = recv(sockfd, str, 512, 0)) == -1){
			perror("recv");
	//		exit(1);
		   }
			printf("Received a message: %s\n", str);

		}
}




int main(int argc, char *argv[]) {
pthread_t send;
pthread_t recv;
sem_init(&start,0,0);
pthread_create(&send,NULL,thsend,(void *)argv[1]);
pthread_create(&recv,NULL,threcv,NULL);


pthread_join(&send,NULL);
pthread_join(&recv,NULL);

return 0;


















}
