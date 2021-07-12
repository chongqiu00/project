#include "_freecplus.h"

CTcpClient TcpClient;

char strSendBuffer[301],strRecvBuffer[301];

bool biz10000();  // 心跳

bool biz10001();   // 新用户登录 

int main(int argc,char *argv[])
{
  //if (TcpClient.ConnectToServer("127.0.0.1",5015)==false) { printf("conn failed.\n"); return -1; }
  if (TcpClient.ConnectToServer("172.16.0.15",5015)==false) { printf("conn failed.\n"); return -1; }
  //if (TcpClient.ConnectToServer("118.89.50.198",5015)==false) { printf("conn failed.\n"); return -1; }

  if (biz10000()==false) return 0;   // 心跳

  CTimer Timer;
  if (biz10001()==false) return 0;   // 新用户登录 
  printf("biz10001=%lf\n",Timer.Elapsed());

  sleep(1);
  
  return 0;
}

bool biz10000()
{
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  strcpy(strSendBuffer,"<bizid>10000</bizid>");

  //printf("send=%s=\n",strSendBuffer);
  if (TcpClient.Write(strSendBuffer)==false) { printf("send failed.\n"); return false; }

  if (TcpClient.Read(strRecvBuffer,20)==false)  { printf("recv failed.\n"); return false; }
  //printf("recv=%s=\n",strRecvBuffer);

  return true;
}

bool biz10001()
{
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  strcpy(strSendBuffer,"<bizid>10001</bizid><userid>52:54:00:83:0f:c1</userid><ttytype>1</ttytype><lat>20.234518</lat><lon>115.90832</lon><height>150.5</height>");

  //printf("send=%s=\n",strSendBuffer);
  if (TcpClient.Write(strSendBuffer)==false) { printf("send failed.\n"); return false; }

  // 接收全部的站点信息
  while (1)
  {
    memset(strRecvBuffer,0,sizeof(strRecvBuffer));
    if (TcpClient.Read(strRecvBuffer,20)==false)  { printf("recv failed.\n"); return false; }
    // printf("recv=%s=\n",strRecvBuffer);

    if (strcmp(strRecvBuffer,"ok")==0) break;
  }

  return true;
}
