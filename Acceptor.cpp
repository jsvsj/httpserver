#include"Acceptor.h"
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include<stdio.h>
#include <arpa/inet.h>
#include"HttpConnection.h"
#include"EventLoop.h"
#include"HttpServer.h"
#include<fcntl.h>
#include<sys/epoll.h>


using namespace std;
using namespace http;


Acceptor::Acceptor(EventLoop *loop,HttpServer* server,int port,char *ip)
	:port_(port),
	ip_(ip),
	index_(-1),
	server_(server),
	loop_(loop)
{
	fd_=::socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK,0);
	if(fd_<0)
	{
		cout<<"error socket"<<endl;
		exit(1);
	}
	int op=1;
	
	int ret=::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR,&op, sizeof(op));
	if(ret<0)
	{
		cout<<"err setsockopt"<<endl;
	}
}

void Acceptor::Bind()
{
	int ret;
	sockaddr_in seraddr;
	::bzero(&seraddr,sizeof(seraddr));
	if(ip_==NULL)
	{	
		seraddr.sin_addr.s_addr=htonl(INADDR_ANY);
	}
	else
	{
		ret=::inet_pton(fd_,ip_,&seraddr.sin_addr.s_addr);
	}
	
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(port_);
	ret=::bind(fd_,(struct sockaddr*)&seraddr,sizeof(seraddr));
	if(ret<0)
	{
		perror("bind:");
		exit(1);
	}
}

void Acceptor::Listen()
{
	if(::listen(fd_, SOMAXCONN )<0)
	{
		perror("listen:");
		exit(1);
	}
	this->foxonread();
	
	
}


Acceptor::~Acceptor()
{
	
}

void Acceptor::handleEvent()
{
	if(revents_ & EPOLLIN)
	{
		handleRead();
		revents_=0;
	}
	
}

void Acceptor::handleRead()
{
	struct sockaddr_in cliaddr;
	memset(&cliaddr,0,sizeof(cliaddr));
	socklen_t len=sizeof(cliaddr);
	
	int conn=::accept(fd_,(struct sockaddr*)&cliaddr,&len);
	//设置非阻塞
	
	if(conn<0)
	{
		perror("Acceptor::handleRead accept:");
		exit(1);
	}
	int flags = fcntl(conn,F_GETFL,0);  
	flags |= O_NONBLOCK;  
	if(fcntl(conn,F_SETFL,flags)<0)
	{
		perror("fcntl");
		exit(1);
	}		
	
	cout<<"accept ok  client fd = "<<conn<<endl;
	printf("ip = %s port = %d\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
	
	EventLoop *ioloop=server_->getNextLoop();
	
	HttpConnection *co=new HttpConnection(ioloop,server_,conn,EPOLLIN);
	cout<<"num of client is "<<server_->getConnections().size()<<endl;
	ioloop->addEvent(co);
	
}


