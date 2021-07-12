#include "_public.h"

int main()
{
  CTcpServer TcpServer;

  if (TcpServer.InitServer(5508) == false) // ��ʼ�������
  {
    printf("TcpServer.InitServer failed.\n"); return -1;
  }

  TcpServer.Accept();    // ���ܿͻ��˵�����

  printf("�ͻ���������%s.\n",TcpServer.GetIP());

  char strbuf[300];
  memset(strbuf,0,sizeof(strbuf));

  TcpServer.Read(strbuf,50);   // ���տͻ��˵ı���

  printf("recv:%s\n",strbuf);

  TcpServer.Write("���ˡ�");   // ��ͻ��˻ظ�

  return 0;
}
