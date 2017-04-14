#include"HttpConnection.h"
#include"EventLoop.h"
#include"HttpServer.h"
#include<unistd.h>
#include<sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include<stdio.h>
#include<iostream>
#include <fcntl.h>
#include<string.h>
#include<sstream>


using namespace std;
using namespace http;


class HttpTask:public Task
{
private:
	HttpConnection *con_;
public:
	HttpTask(HttpConnection *con):con_(con)
	{
		
	}
	virtual int do_task()
	{
		cout<<"task do by thread["<<pthread_self()<<"]\n\n"<<endl;
		// if(cha_!=NULL)
		// {
			// cha_->handleEvent();
		// }
		
		string uri;
		string method;
		char *recvbuf_=con_->getRecvbuf();
		int fd_=con_->getFd();
		EventLoop* loop_=con_->getLoop();
		int recvlen_=con_->getRecvlen();
		
		
		cout<<"============================"<<endl;
		cout<<recvbuf_<<endl;
		
		
		cout<<recvlen_<<endl;
		stringstream ss(string(recvbuf_,recvbuf_+recvlen_));
		ss>>method>>uri;
		cout<<"method is "<<method<<",uri is "<<uri<<endl;


		
		//do_response(method,uri);
		
		//write(fd_,bb,strlen(bb));
		
		char content[2048]={0};
		const char *path=uri.c_str()+1;
		printf("%s\n",path);
		
		int filefd = open(path, O_RDONLY);
		if(filefd<0)
		{
			
			loop_->delEvent(con_);
			cout<<recvlen_<<endl;
			return -1;
		}
		struct stat st;
		fstat(filefd,&st);
		off_t size=st.st_size;
		//设置请求头
		sprintf(content, "HTTP/1.1 %s\r\n", "GET");
		sprintf(content, "%sContent-type: text/html\r\n", content);
		sprintf(content, "%sContent-length: %d\r\n", content, 
						(int)size);
		sprintf(content, "%sEncoding:UTF-8\r\n\r\n", content);
		
		write(fd_, content, strlen(content));
		//使用“零拷贝”发送文件
		sendfile(fd_, filefd, 0, size);
		
		
		
		return 0;
	}
};


HttpConnection::HttpConnection(EventLoop *loop,HttpServer* server,int fd,int events)
	:loop_(loop),
	server_(server),
	recvlen_(0)
	
{
	fd_=fd;
	events_=events;
	
}

void HttpConnection::handleRead()
{
	
	HttpTask *task=new HttpTask(this);
	ThreadPool *  calc_pool=server_->getCalcThreadPool();
	if(calc_pool==NULL)
	{
		task->do_task();
		delete task;
	}
	else
	{
		//会自动析构task
		calc_pool->addTask(task);
	}
	
}


void HttpConnection::handleWrite()
{
	
}


HttpConnection::~HttpConnection()
{
	
}

void HttpConnection::recvdata()
{
	recvlen_=read(fd_,recvbuf_,sizeof(recvbuf_));
	if(recvlen_==0)
	{
		cout<<"client close fd = "<<fd_<<endl;
		
		loop_->delEvent(this);
		return;
	}
	else if(recvlen_<0)
	{
		perror("read");
		return;
	}
	
}

//
void HttpConnection::handleEvent()
{
	
	if(revents_ & EPOLLIN && (events_ & EPOLLIN))
	{
		recvdata();
		if(recvlen_==0)
		{
			return;
		}
		else if(recvlen_<0)
		{
			perror("read");
			return;
		}
	
		handleRead();
	}
	else if(revents_ & EPOLLOUT && (events_ & EPOLLOUT) )
	{
		handleWrite();
	}
	
}




