#include "_public.h"

CTcpServer TcpServer;
CLogFile logfile;

int main(int argc,char *argv[])
{
  logfile.Open("/tmp/demo25.log","w",false);

  if (TcpServer.InitServer(5015)==false) { printf("init failed.\n"); return -1; }

  TcpServer.Accept();

  char strSendBuffer[301],strRecvBuffer[301];

  for (int ii=0;ii<10;ii++)
  {
    memset(strSendBuffer,0,sizeof(strSendBuffer));
    memset(strRecvBuffer,0,sizeof(strRecvBuffer));

    if (TcpServer.Read(strRecvBuffer,80)==false)  { printf("recv failed.\n"); return -1; }
    logfile.Write("recv=%s=\n",strRecvBuffer);

    sprintf(strSendBuffer,"%s resp",strRecvBuffer);

    logfile.Write("send=%s=\n",strSendBuffer);
    if (TcpServer.Write(strSendBuffer)==false) { printf("send failed.\n"); return -1; }
  }
  
  return 0;
}

