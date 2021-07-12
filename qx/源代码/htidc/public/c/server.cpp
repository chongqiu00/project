#include "_public.h"

int main()
{
  CTcpServer TcpServer;

  if (TcpServer.InitServer(5508) == false) // 初始化服务端
  {
    printf("TcpServer.InitServer failed.\n"); return -1;
  }

  TcpServer.Accept();    // 接受客户端的连接

  printf("客户端已连接%s.\n",TcpServer.GetIP());

  char strbuf[300];
  memset(strbuf,0,sizeof(strbuf));

  TcpServer.Read(strbuf,50);   // 接收客户端的报文

  printf("recv:%s\n",strbuf);

  TcpServer.Write("吃了。");   // 向客户端回复

  return 0;
}
