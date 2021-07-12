#include "_public.h"

CTcpClient TcpClient;
CLogFile logfile;

int main(int argc,char *argv[])
{
  logfile.Open("/tmp/demo23.log","w",false);

  if (TcpClient.ConnectToServer("127.0.0.1",5015)==false) { printf("conn failed.\n"); return -1; }

  char strSendBuffer[301],strRecvBuffer[301];

  int pid=fork();

  for (int ii=0;ii<100;ii++)
  {
    memset(strSendBuffer,0,sizeof(strSendBuffer));
    memset(strRecvBuffer,0,sizeof(strRecvBuffer));

    sprintf(strSendBuffer,"this is %10d(%10d)",ii,getpid());

    logfile.Write("send=%s=\n",strSendBuffer);
    if (TcpClient.Write(strSendBuffer)==false) { printf("send failed.\n"); return -1; }

    sleep(1);

    if (TcpClient.Read(strRecvBuffer,80)==false)  { printf("recv failed.\n"); return -1; }
    logfile.Write("recv=%s,%10d=\n",strRecvBuffer,getpid());
  }
  
  return 0;
}

