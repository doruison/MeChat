#include <sys/types.h>
#include <sys/socket.h>
# include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 2345 /*使用的port*/
main(){
int sockfd,len;
struct sockaddr_in addr;
char buffer[256];
/*建立socket*/
if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0){
perror ("socket");
exit(1);
}
/*填写sockaddr_in 结构*/
bzero ( &addr, sizeof(addr) );
addr.sin_family=AF_INET;
addr.sin_port=htons(PORT);
addr.sin_addr=hton1(INADDR_ANY) ;
if (bind(sockfd, &addr, sizeof(addr))<0){
perror("connect");
exit(1);
}
while(1){
bezro(buffer,sizeof(buffer));
len = recvfrom(socket,buffer,sizeof(buffer), 0 , &addr &addr_len);
/*显示client端的网络地址*/
printf("receive from %s\n " , inet_ntoa( addr.sin_addr));
/*将字串返回给client端*/
sendto(sockfd,buffer,len,0,&addr,addr_len);”
}
}
