#include "_public.h"

int main()
{
  CTcpClient TcpClient;

  if (TcpClient.ConnectToServer("118.89.50.198",5508)==false) // 向服务器发起连接
  {
    printf("TcpClient.ConnectToServer failed.\n"); return -1;
  }

  char strbuf[301];
  SNPRINTF(strbuf,100,"你吃了没？");

  TcpClient.Write(strbuf);    // 向服务端发送报文

  memset(strbuf,0,sizeof(strbuf));
  TcpClient.Read(strbuf,50);  // 接收服务端的回复

  printf("recv:%s\n",strbuf);

  return 0;
}
