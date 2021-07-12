//
// ��������ʾ����CTcpServer�࣬ʵ��socketͨѶ�ķ����
// 

#include "_public.h"

#pragma pack(1)
struct datarec /* ����=100 byte */ 
{ unsigned   char baoformat;         //  1 ��˲ʱ���У�
  unsigned   char station;           //  2 ��˲ʱ���У�
  unsigned   char stationname[6];    //  3 ��˲ʱ���У�
  unsigned   int  rec_time;          //  4 ��˲ʱ���У�
  unsigned   long rec_date;          //  5 ��˲ʱ���У�
  unsigned   int  wd2dd;             //  6 ��˲ʱ���У�
  unsigned   int  wd2df;             //  7��˲ʱ���У�
  unsigned   int  wd10dd;            //  8��˲ʱ���У�
  unsigned   int  wd10df;            //  9��˲ʱ���У�
  unsigned   int  wd10maxdd;         // 10 
  unsigned   int  wd10maxdf;         // 11
  unsigned   int  wd3smaxdd;         // 12
  unsigned   int  wd3smaxdf;         // 13 
  unsigned   char wd10maxtime;       // 14
  unsigned   char wd3smaxtime;       // 15
	             int  temp;      //         16��˲ʱ���У�
	             int  maxtemp;   //         17 
  	          int  mintemp;      //      18
  unsigned   char maxtemptime;       // 19
  unsigned   char mintemptime;       // 20  
  unsigned   int  hourrf;            // 21��˲ʱ���У�
  unsigned   int  dayrf;             // 22��˲ʱ���У�
	             char rh;        //         23 ��˲ʱ���У�
	             char maxrh;     //         24
  unsigned   char maxrhtime;         // 25 
 	          char minrh;        //      26
  unsigned   char minrhtime;         // 27 
  unsigned   char rfmark1;           // 28
 	          int  dp;           //      29��˲ʱ���У�
          int  ps;                // 30 //  �˲ʱ���У�
	    	       int  maxps;   //           31
  	       int  minps;           //   32 
  unsigned   char maxpstime;         // 33
  unsigned   char minpstime;         // 34
  unsigned   char minrf[12];         // 35  
 	          int  Bps;          //      36
  unsigned   char Ys;                // 37��˲ʱ���У�
  unsigned   char w3a;               // 38��˲ʱ���У�
  unsigned   int  wd3daymax;         // 39
  unsigned   int  wf3daymax;         // 40
  unsigned   int  wd3daymaxtime;     // 41 
  unsigned   int  wd10daymax;        // 42 
  unsigned   int  wf10daymax;        // 43
  unsigned   int  wd10daymaxtime;    // 44 
 	          int  daymaxtemp;   //      45
  unsigned   int  daymaxtemptime;    // 46
             int  daymintemp;        // 47 
  unsigned   int  daymintemptime;    // 48
  unsigned   int  WpV;               //49��˲ʱ���У�
  unsigned   char Tempspace[2];      //50��˲ʱ���У�
}  Recv_Data;


int main(int argc,char *argv[])
{
  printf("sizeof=%d=\n",sizeof(Recv_Data));
 
  if (argc != 2)
  {
    printf("\n");
    printf("Using:./demo12 port\n\n");

    printf("Example:./demo12 5010\n\n");

    printf("��������ʾ����CTcpServer�࣬ʵ��socketͨѶ�ķ���ˡ�\n\n");

    return -1;
  }

  CTcpServer TcpServer;

  // ����˳�ʼ��
  if (TcpServer.InitServer(atoi(argv[1])) == false)
  {
    printf("TcpServer.InitServer(%s) failed.\n",argv[1]); return -1;
  }

  // �ȴ��ͻ��˵�����
  if (TcpServer.Accept() == false)
  {
    printf("TcpServer.Accept() failed.\n"); return -1;
  }

  printf("client ip is %s\n",TcpServer.GetIP());

  char strRecvBuffer[1024],strSendBuffer[1024];

  memset(strRecvBuffer,0,sizeof(strRecvBuffer));

  // ��ȡ�ͻ��˵ı��ģ���ʱ����10��
  //if (TcpServer.Read(strRecvBuffer,10)==false) 
  if (TcpServer.Read(strRecvBuffer)==false) 
  {
    if (TcpServer.m_btimeout==true) printf("timeout\n");
    printf("TcpServer.Read() failed.\n"); return -1;
  }

  printf("recv ok:%s\n",strRecvBuffer);

  memset(strSendBuffer,0,sizeof(strSendBuffer));
  strcpy(strSendBuffer,"ok");

  // ��ͻ��˷�����Ӧ����
  if (TcpServer.Write(strSendBuffer)==false) 
  {
    printf("TcpServer.Write() failed.\n"); return -1;
  }

  printf("send ok:%s\n",strSendBuffer);

  return 0;
}

