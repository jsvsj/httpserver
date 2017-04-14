#include"HttpServer.h"
#include<stdio.h>

using namespace http;
using namespace std;

int main(int argc,char *argv[])
{
	
							//io线程数 计算线程数
	HttpServer server(8888,0,2,3);
	server.start();
	
	return 0;
}
