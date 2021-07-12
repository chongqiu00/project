/*
 ����һ��ͨ�õĹ���ģ�飬����TCPЭ��ʵ���ļ�����ķ���ˣ����̡߳�
 ���ߣ���ũ�е�
*/

#include "_public.h"

struct st_arg
{
  int clienttype;
  char ip[31];              // �������˵�IP��ַ��
  int  port;                // �������˵Ķ˿ڡ�
  int  ptype;               // �ļ����ͳɹ����ļ��Ĵ���ʽ��1-�����ļ���2-�ƶ�������Ŀ¼��3-ɾ���ļ���
  char clientpath[301];     // �����ļ���ŵĸ�Ŀ¼��
  char clientpathbak[301];  // �ļ��ɹ����ͺ󣬱����ļ����ݵĸ�Ŀ¼����ptype==2ʱ��Ч��
  char srvpath[301];        // ������ļ���ŵĸ�Ŀ¼��
  char srvpathbak[301];     // �ļ��ɹ����պ󣬷�����ļ����ݵĸ�Ŀ¼����ptype==2ʱ��Ч��
  bool andchild;            // �Ƿ���clientpathĿ¼�¸�����Ŀ¼���ļ���true-�ǣ�false-��
  char matchname[301];      // �������ļ�����ƥ�䷽ʽ����"*.TXT,*.XML"��ע���ô�д��
  char okfilename[301];     // �ѷ��ͳɹ��ļ����嵥��
  int  timetvl;             // ɨ�豾��Ŀ¼�ļ���ʱ��������λ���롣
};

// ��xml����������starg�ṹ��
bool _xmltoarg(char *strxmlbuffer,struct st_arg *starg);

CLogFile logfile;

// �ȴ���¼
bool ClientLogin(int clientfd,struct st_arg *starg);

// �г�srvpathĿ¼���ļ����嵥�����ظ��ͻ��ˡ�
bool ListFile(int clientfd,struct st_arg *starg);

// �����˳�ʱ���õĺ���
void EXIT(int sig);

// ��ͻ���ͨ���̵߳�������
void *pth_main(void *arg);

// �����ļ�������
bool RecvFilesMain(int clientfd,struct st_arg *starg);

// �����ļ�������
bool SendFilesMain(int clientfd,struct st_arg *starg);

// ��ſͻ��������ӵ�socket������
vector<int> vclientfd;
void AddClient(int clientfd);      // �ѿͻ����µ�socket����vclientfd������
void RemoveClient(int clientfd);   // �رտͻ��˵�socket����vclientfd������ɾ����

int main(int argc,char *argv[])
{
  if (argc != 3)
  {
    printf("\n");
    printf("Using:/htidc/public/bin/tcpfileserver1 logfilename port\n");

    printf("Example:/htidc/public/bin/tcpfileserver1 /log/shqx/tcpfileserver1.log 5010\n\n");
    printf("��������һ����������ģ�飬����TCP/IP�����ļ��ķ���ˡ�\n");
    printf("��������õ��Ƕ��̵߳ķ���ˣ�����̵ķ���˳�����tcpfileserver.cpp��\n");
    printf("logfilename ��־�ļ�����\n");
    printf("port ���ڴ����ļ���TCP�˿ڡ�\n");

    return -1;
  }

  // �ر�ȫ�����źź��������
  // �����ź�,��shell״̬�¿��� "kill + ���̺�" ������ֹЩ����
  // ���벻Ҫ�� "kill -9 +���̺�" ǿ����ֹ
  CloseIOAndSignal(); signal(SIGINT,EXIT); signal(SIGTERM,EXIT);

  // �򿪳���������־������һ������̳�����־�����Զ��л�
  if (logfile.Open(argv[1],"a+",false) == false)
  {
    printf("logfile.Open(%s) failed.\n",argv[1]); return -1;
  }

  logfile.Write("fileserver started(%s).\n",argv[2]);

  CTcpServer TcpServer;

  if (TcpServer.InitServer(atoi(argv[2])) == false)
  {
    logfile.Write("TcpServer.InitServer(%s) failed.\n",argv[2]); return -1;
  }

  // �������˵�listenfd��vclientfd
  AddClient(TcpServer.m_listenfd);

  while (true)
  {
    // �ȴ��ͻ��˵�����
    if (TcpServer.Accept() == false)
    {
      logfile.Write("TcpServer.Accept() failed.\n"); continue;
    }

    pthread_t pthid;   // ����һ�̣߳��������������Ŀͻ���ͨ��
    if (pthread_create(&pthid,NULL,pth_main,(void*)(long)TcpServer.m_connfd)!=0)
    { 
      logfile.Write("�����߳�ʧ�ܣ������˳���n"); close(TcpServer.m_connfd); EXIT(-1);
    }

    logfile.Write("%s is connected.\n",TcpServer.GetIP());

    // ����ÿ���ͻ��˵�socket��vclientfd
    AddClient(TcpServer.m_connfd);
  }

  return 0;
}

// �˳�ʱ���õĺ���
void EXIT(int sig)
{
  signal(SIGINT,SIG_IGN); signal(SIGTERM,SIG_IGN);

  if (sig>0) signal(sig,SIG_IGN);

  logfile.Write("tcpfileserver1 exit,sig=%d...\n",sig);

  // �ر�vclientfd������ȫ����socket
  for (int ii=0;ii<vclientfd.size();ii++)
  {
    close(vclientfd[ii]);
  }

  exit(0);
}

// �ȴ���¼
bool ClientLogin(int clientfd,struct st_arg *starg)
{
  int  ibuflen=0;
  char strRecvBuffer[TCPBUFLEN+10]; // ���ձ��ĵĻ�����
  char strSendBuffer[TCPBUFLEN+10]; // ���ͱ��ĵĻ�����

  memset(strRecvBuffer,0,sizeof(strRecvBuffer));
  memset(strSendBuffer,0,sizeof(strSendBuffer));

  if (TcpRead(clientfd,strRecvBuffer,&ibuflen,20) == false)
  {
    logfile.Write("1 TcpRead() failed.\n"); return false;
  }
  // logfile.Write("1 strRecvBuffer=%s\n",strRecvBuffer);  // xxxxxx

  GetXMLBuffer(strRecvBuffer,"clienttype",&starg->clienttype);

  if ( (starg->clienttype==1) || (starg->clienttype==2) )
    strcpy(strSendBuffer,"ok");
  else
    strcpy(strSendBuffer,"failed");

  // logfile.Write("1 strSendBuffer=%s\n",strSendBuffer);  // xxxxxx
  if (TcpWrite(clientfd,strSendBuffer) == false)
  {
    logfile.Write("1 TcpWrite() failed.\n"); return false;
  }

  logfile.Write("login %s(clienttype=%d).\n",strSendBuffer,starg->clienttype);

  if (strcmp(strSendBuffer,"failed") == 0) return false;

  // �Ѳ�����������
  _xmltoarg(strRecvBuffer,starg);

  return true;
}

// �����ļ�������
bool RecvFilesMain(int clientfd,struct st_arg *starg)
{
  int  ibuflen=0;
  char strRecvBuffer[TCPBUFLEN+10]; // ���ձ��ĵĻ�����
  char strSendBuffer[TCPBUFLEN+10]; // ���ͱ��ĵĻ�����

  while (true)
  {
    memset(strRecvBuffer,0,sizeof(strRecvBuffer));
    memset(strSendBuffer,0,sizeof(strSendBuffer));

    if (TcpRead(clientfd,strRecvBuffer,&ibuflen,80) == false)
    {
      logfile.Write("TcpRead() failed.\n"); return false;
    }
    // logfile.Write("2 strRecvBuffer=%s\n",strRecvBuffer);  // xxxxxx

    // ������������
    if (strstr(strRecvBuffer,"activetest")!=0)
    {
      strcpy(strSendBuffer,"ok");
      // logfile.Write("2 strSendBuffer=%s\n",strSendBuffer);  // xxxxxx
      if (TcpWrite(clientfd,strSendBuffer) == false)
      {
        logfile.Write("2 TcpWrite() failed.\n"); return false;
      }

      continue;
    }

    struct st_fileinfo stfileinfo;
    memset(&stfileinfo,0,sizeof(struct st_fileinfo));

    // ��ȡ�����յ��ļ���ʱ��ʹ�С
    GetXMLBuffer(strRecvBuffer,"filename",stfileinfo.filename);
    GetXMLBuffer(strRecvBuffer,"filesize",&stfileinfo.filesize);
    GetXMLBuffer(strRecvBuffer,"mtime",stfileinfo.mtime);

    // ���ļ����е�clientpath�滻��srvpath��ҪС�ĵ���������
    UpdateStr(stfileinfo.filename,starg->clientpath,starg->srvpath,false);

    // �����ļ�������
    if (RecvFile(&logfile,clientfd,&stfileinfo)== false)
    {
      logfile.Write("RecvFile() failed.\n"); return false;
    }

    logfile.Write("recv %s ok.\n",stfileinfo.filename);
  }

  return true;
}


// �����ļ�������
bool SendFilesMain(int clientfd,struct st_arg *starg)
{
  int  ibuflen=0;
  char strRecvBuffer[TCPBUFLEN+10]; // ���ձ��ĵĻ�����
  char strSendBuffer[TCPBUFLEN+10]; // ���ͱ��ĵĻ�����

  while (true)
  {
    memset(strRecvBuffer,0,sizeof(strRecvBuffer));
    if (TcpRead(clientfd,strRecvBuffer,&ibuflen,80) == false)
    {
      logfile.Write("TcpRead() failed.\n"); return false;
    }
    // logfile.Write("3 strRecvBuffer=%s\n",strRecvBuffer);  // xxxxxx

    // ������������
    if (strstr(strRecvBuffer,"activetest")!=0)
    {
      memset(strSendBuffer,0,sizeof(strSendBuffer));
      strcpy(strSendBuffer,"ok");
      // logfile.Write("3 strSendBuffer=%s\n",strSendBuffer);  // xxxxxx
      if (TcpWrite(clientfd,strSendBuffer) == false)
      {
        logfile.Write("3 TcpWrite() failed.\n"); return false;
      }

      continue;
    }

    // �����ȡ�ļ��б���
    if (strcmp(strRecvBuffer,"<list>")==0)
    {
      if (ListFile(clientfd,starg)==false)
      {
        logfile.Write("ListFile() failed.\n"); return false;
      }

      continue;
    }

    // ȡ�ļ�����
    if (strncmp(strRecvBuffer,"<filename>",10)==0)
    {
      // ��ȡ�����յ��ļ���ʱ��ʹ�С
      struct st_fileinfo stfileinfo;
      memset(&stfileinfo,0,sizeof(struct st_fileinfo));
      GetXMLBuffer(strRecvBuffer,"filename",stfileinfo.filename);
      GetXMLBuffer(strRecvBuffer,"filesize",&stfileinfo.filesize);
      GetXMLBuffer(strRecvBuffer,"mtime",stfileinfo.mtime);

      // ���ļ����͸��ͻ���
      if (SendFile(&logfile,clientfd,&stfileinfo)==false) return false;

      logfile.Write("put %s ...ok.\n",stfileinfo.filename);

      // ɾ������˵��ļ�
      if (starg->ptype==2) REMOVE(stfileinfo.filename);

      // ���ݷ���˵��ļ�
      if (starg->ptype==3) 
      {
        char strfilenamebak[301];
        memset(strfilenamebak,0,sizeof(strfilenamebak));
        strcpy(strfilenamebak,stfileinfo.filename);
        UpdateStr(strfilenamebak,starg->srvpath,starg->srvpathbak,false);  // ҪС�ĵ���������
        if (RENAME(stfileinfo.filename,strfilenamebak)==false)
        {
          logfile.Write("RENAME %s to %s failed.\n",stfileinfo.filename,strfilenamebak); return false;
        }
      }
    }
  }

  return true;
}

// ��xml����������starg�ṹ��
bool _xmltoarg(char *strxmlbuffer,struct st_arg *starg)
{
  GetXMLBuffer(strxmlbuffer,"ip",starg->ip);
  GetXMLBuffer(strxmlbuffer,"port",&starg->port);
  GetXMLBuffer(strxmlbuffer,"ptype",&starg->ptype);
  GetXMLBuffer(strxmlbuffer,"clientpath",starg->clientpath);
  GetXMLBuffer(strxmlbuffer,"clientpathbak",starg->clientpathbak);
  GetXMLBuffer(strxmlbuffer,"srvpath",starg->srvpath);
  GetXMLBuffer(strxmlbuffer,"srvpathbak",starg->srvpathbak);
  GetXMLBuffer(strxmlbuffer,"andchild",&starg->andchild);
  GetXMLBuffer(strxmlbuffer,"matchname",starg->matchname);
  GetXMLBuffer(strxmlbuffer,"okfilename",starg->okfilename);
  GetXMLBuffer(strxmlbuffer,"timetvl",&starg->timetvl);

  return true;
}

// �г�srvpathĿ¼���ļ����嵥�����ظ��ͻ��ˡ�
bool ListFile(int clientfd,struct st_arg *starg)
{
  int  ibuflen=0;
  char strRecvBuffer[TCPBUFLEN+10]; // ���ձ��ĵĻ�����
  char strSendBuffer[TCPBUFLEN+10]; // ���ͱ��ĵĻ�����

  CDir Dir;

  // ע�⣬���Ŀ¼�µ����ļ�������50000�����������ļ����ܽ�������
  if (Dir.OpenDir(starg->srvpath,starg->matchname,50000,starg->andchild,false)==false)
  {
    logfile.Write("Dir.OpenDir(%s) ʧ�ܡ�\n",starg->srvpath); return false;
  }

  // �Ȱ��ļ��������ظ��ͻ���
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  sprintf(strSendBuffer,"<totalfile>%d</totalfile>",Dir.m_vFileName.size());
  // logfile.Write("4 strSendBuffer=%s\n",strSendBuffer);  // xxxxxx
  if (TcpWrite(clientfd,strSendBuffer) == false)
  {
    logfile.Write("4 TcpWrite() failed.\n"); return false;
  }

  // ���ļ���Ϣһ�����ķ��ظ��ͻ���
  while (true)
  {
    if (Dir.ReadDir()==false) break;

    memset(strSendBuffer,0,sizeof(strSendBuffer));
    sprintf(strSendBuffer,"<filename>%s</filename><mtime>%s</mtime><filesize>%d</filesize>",Dir.m_FullFileName,Dir.m_ModifyTime,Dir.m_FileSize);
    // logfile.Write("5 strSendBuffer=%s\n",strSendBuffer);  // xxxxxx
    if (TcpWrite(clientfd,strSendBuffer) == false)
    {
      logfile.Write("5 TcpWrite() failed.\n"); return false;
    }
  }

  return true;
}

// ��ͻ���ͨ���̵߳�������
void *pth_main(void *arg)
{
  int clientfd=(long) arg; // arg����Ϊ�¿ͻ��˵�socket��

  pthread_detach(pthread_self());

  struct st_arg starg;
  memset(&starg,0,sizeof(struct st_arg));

  // �ȴ��ͻ��˵ĵ�¼
  if (ClientLogin(clientfd,&starg) == false) {  RemoveClient(clientfd); pthread_exit(0); }

  // �����ļ�������
  if (starg.clienttype==1) 
  {
    if (RecvFilesMain(clientfd,&starg) == false) { RemoveClient(clientfd); pthread_exit(0); }
  }

  // �����ļ�������
  if (starg.clienttype==2) 
  {
    if (SendFilesMain(clientfd,&starg) == false) { RemoveClient(clientfd); pthread_exit(0); }
  }

  RemoveClient(clientfd); 

  pthread_exit(0);
}

// �ѿͻ����µ�socket����vclientfd������
void AddClient(int clientfd)
{
  vclientfd.push_back(clientfd);
}

// �رտͻ��˵�socket����vclientfd������ɾ����
void RemoveClient(int clientfd)  
{
  for (int ii=0;ii<vclientfd.size();ii++)
  {
    if (vclientfd[ii]==clientfd) { close(clientfd); vclientfd.erase(vclientfd.begin()+ii); return; }
  }
}

