#include "_public.h"

int main()
{
  CTcpClient TcpClient;

  if (TcpClient.ConnectToServer("118.89.50.198",5508)==false) // ���������������
  {
    printf("TcpClient.ConnectToServer failed.\n"); return -1;
  }

  char strbuf[301];
  SNPRINTF(strbuf,100,"�����û��");

  TcpClient.Write(strbuf);    // �����˷��ͱ���

  memset(strbuf,0,sizeof(strbuf));
  TcpClient.Read(strbuf,50);  // ���շ���˵Ļظ�

  printf("recv:%s\n",strbuf);

  return 0;
}
