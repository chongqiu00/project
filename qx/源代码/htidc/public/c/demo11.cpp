//
// ��������ʾ����CTcpClient�࣬ʵ��socketͨѶ�Ŀͻ���
// 

#include "_public.h"

int main(int argc,char *argv[])
{
  CFile File;

  File.Open("/tmp/aaa.dat","r");  //  /tmp/aaa.dat.tmp

  char strbuf[301];

  // while ��Ҫ10���ʱ��
  while(true)
  {
    memset(strbuf,0,sizeof(strbuf));

    //if (File.FFGETS(strbuf,300,"<endl/>")==false) break;
    if (File.Fgets(strbuf,300)==false) break;

    // ���ļ���д������
    printf("=%s=\n",strbuf);

  }

  // File.CloseAndRename();    // rename /tmp/aaa.dat.tmp /tmp/aaa.dat
  
  return 0;

  if (argc != 3)
  {
    printf("\n");
    printf("Using:./demo11 ip port\n\n");

    printf("Example:./demo11 118.89.50.198 5010\n\n");

    printf("��������ʾ����CTcpClient�࣬ʵ��socketͨѶ�Ŀͻ��ˡ�\n\n");

    return -1;
  }

  CTcpClient TcpClient;

  // ���������������
  if (TcpClient.ConnectToServer(argv[1],atoi(argv[2])) == false)
  {
    printf("TcpClient.ConnectToServer(%s,%d) failed.\n",argv[1],atoi(argv[2])); return -1;
  }

  char strRecvBuffer[1024],strSendBuffer[1024];

  memset(strSendBuffer,0,sizeof(strSendBuffer));
  strcpy(strSendBuffer,\
        "Ӣ�����һ�֣�����������������������һ����ʽ����������ζ�ź�ħ�ϴγư�ŷ�ڵĻƽ�һ��ȫ���˳���");

  // ��strSendBuffer���ݷ��͸������
  if (TcpClient.Write(strSendBuffer)==false)
  {
    printf("TcpClient.Write() failed.\n"); return -1;
  }

  printf("send ok:%s\n",strSendBuffer);

  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  // ���շ���˷��صı���
  if (TcpClient.Read(strRecvBuffer)==false)
  {
    if (TcpClient.m_btimeout==true) printf("timeout\n");
    printf("TcpClient.Read() failed.\n"); return -1;
  }

  printf("recv ok:%s\n",strRecvBuffer);

  return 0;
}

