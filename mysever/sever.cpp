#include<cstdio>

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	//创建服务端socket 
	int listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenFd == -1)
	{
		printf("创建listenFd失败！\n");
		perror("socket");
		exit(-1);
	}
	
	 
	struct sockaddr_in severAddr;                    //服务端地址信息数据结构 
	memset(&severAddr, 0, sizeof(severAddr));        //初始化 
	severAddr.sin_family = AF_INET;                  //协议族 
	severAddr.sin_addr.s_addr = htonl(INADDR_ANY);   //任意本地IP地址
	severAddr.sin_port = htons(5051);                //指定通信端口
	
	//服务端用于通信的地址端口绑定在socket上
	if (bind(listenFd, (struct sockaddr*)severAddr, sizeof(severAddr)) != 0) 
	{
		close(listenFd);
		perror("bind");
		exit(-1);
	}
	
	//把socket设为监听模式 
	if (listen(listenFd, 5) != 0) 
	{
		close(listenFd);
		perror("listen");
		exit(-1);
	}
	
	//客户端地址信息结构体 
	struct sockaddr_in clientAddr;
	//int sockLen = sizeof(struct sockaddr_in);
	//得到客户端socket 
	int clientFd = accept(listenFd, (struct sockaddr*)&clientAddr, sizeof(struct sockaddr_in));
	printf("客户端：%s已连接\n", inet_ntoa(clientAddr.sin_addr) );
	
	int bufferSize = 1024;
	char buffer[bufferSize];
	while( 1 )
	{
		int iret;
		//接收 
		if (iret = recv(clientFd, buffer, bufferSize, 0) <= 0)
		{
			printf("iret = %d\n", iret);
			break;
		}
		printf("iret= %d:%s", iret, buffer);
		//发送 
		char sendBuffer[] = "OK";
		if (iret = send(clientFd, buffer, bufferSize, 0) <= 0)
		{
			printf("iret = %d", iret);
			break;
		}
		printf("iret = %d:%s", iret, sendBuffer);
	}
	
	close(listenFd);
	close(clientFd);
	
	return 0;
}