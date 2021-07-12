/*
 �Ϻ�����APP�������������򡣶��̷߳�ʽ���������ӳء�
*/

#include "_freecplus.h"
#include "_ooci.h"

#define MAXCONNS 10  // ���ݿ����ӳصĴ�С��
pthread_mutex_t mutex[MAXCONNS];  // �����顣
connection conns[MAXCONNS];  // ���ݿ��������顣
bool initconns();   // ��ʼ���ݿ����ӳء�
connection *getconns();  // �����ӳ��л�ȡһ�����ݿ����ӡ�
bool freeconns(connection *in_conn);  // �ͷ����ݿ����ӡ�

// ҵ������
struct st_biz
{
  int  bizid;               // ҵ�����
  char userid[51];          // �豸ID
  int  ttytype;             // �û����豸���ͣ�0-δ֪��1-IOS��2-Andriod��2-���ɡ�
  int  usertype;            // �û����࣬0-δ֪��1-��ͨ�û���2-����־Ը�ߣ�3-�ڲ��û���
  double lon;
  double lat;
  double height;
  char   obtid[11];
  char   xmlbuffer[1001];
};

// ��xml����������stbiz�ṹ��
void xmltobiz(char *strxmlbuffer,struct st_biz *stbiz);

CTcpServer TcpServer;
CLogFile   logfile;

// �����˳�ʱ���õĺ���
void EXIT(int sig);

// ��ͻ���ͨ���̵߳�������
void *pth_main(void *arg);

// ����ҵ��
bool biz10000(int clientfd);

// ���û���¼ҵ��
bool biz10001(struct st_biz *stbiz,int clientfd);

// ��ȡ����ʵ��
bool biz10002(struct st_biz *stbiz,int clientfd);

// �����û�������־��
bool InsertUSERLOG(struct st_biz *stbiz,connection *conn);

// ��ſͻ��������ӵ�socket������
vector<int> vclientfd;
void AddClient(int clientfd);      // �ѿͻ����µ�socket����vclientfd������
void RemoveClient(int clientfd);   // �رտͻ��˵�socket����vclientfd������ɾ����

// �رտͻ��˵�socket����vclientfd������ɾ����
void RemoveClient(int clientfd);

int main(int argc,char *argv[])
{
  if (argc != 3)
  {
    printf("\n");
    printf("Using:/htidc/shtqapp1/bin/shtqappserver1 logfilename port\n");

    printf("Example:/htidc/shtqapp1/bin/shtqappserver1 /log/shtqapp/shtqappserver1.log 5015\n\n");
    printf("���������Ϻ�����APP����ķ���ˡ�\n");
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

  logfile.Write("shtqappserver started(%s).\n",argv[2]);

  if (TcpServer.InitServer(atoi(argv[2])) == false)
  {
    logfile.Write("TcpServer.InitServer(%s) failed.\n",argv[2]); EXIT(-1);
  }

  // �������˵�listenfd��vclientfd
  AddClient(TcpServer.m_listenfd);

  if (initconns()==false)  // ��ʼ�����ݿ����ӳء�
  {
    logfile.Write("initconns() failed.\n"); EXIT(-1);
  }

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

  for (int ii=0;ii<MAXCONNS;ii++)
  {
    logfile.Write("disconnect and pthread_mutex_destroy.\n");
    conns[ii].disconnect();
    pthread_mutex_destroy(&mutex[ii]);
  }

  exit(0);
}

// ��ͻ���ͨ���̵߳�������
void *pth_main(void *arg)
{
  int clientfd=(long) arg; // arg����Ϊ�¿ͻ��˵�socket��

  pthread_detach(pthread_self());

  struct st_biz stbiz;

  int  ibuflen=0;
  char strRecvBuffer[1024]; // ���ձ��ĵĻ�����

  while (true)
  {
    memset(strRecvBuffer,0,sizeof(strRecvBuffer));

    // ���տͻ��˵�ҵ�������ģ��������false����Ϊ�ǿͻ����˳�������ԭ�򣬲�д������־
    if (TcpRead(clientfd,strRecvBuffer,&ibuflen,50) == false)
    {
      // logfile.Write("TcpRead() failed.\n"); 
      break;
    }

    logfile.Write("strRecvBuffer=%s\n",strRecvBuffer);  // xxxxxx

    // �Ѳ�����������
    xmltobiz(strRecvBuffer,&stbiz);

    if (stbiz.bizid==10000)    // ��������
    {
      if (biz10000(clientfd)==true) continue;
      else break;
    }

    // ���û���¼ 
    if (stbiz.bizid==10001)    
    {
      if (biz10001(&stbiz,clientfd)==true) continue;
      else break;
    }

    // ��ȡ����ʵ��
    if (stbiz.bizid==10002)    
    {
      if (biz10002(&stbiz,clientfd)==true) continue;
      else break;
    }

    // ������

    logfile.Write("�Ƿ�����%s\n",strRecvBuffer); break;
  }

  RemoveClient(clientfd);

  pthread_exit(0);
}

// ��xml����������starg�ṹ��
void xmltobiz(char *strxmlbuffer,struct st_biz *stbiz)
{
  memset(stbiz,0,sizeof(struct st_biz));

  // ҵ�����
  GetXMLBuffer(strxmlbuffer,"bizid",&stbiz->bizid);
  // logfile.Write("bizid=%d\n",stbiz->bizid);

  // �û��豸ID
  GetXMLBuffer(strxmlbuffer,"userid",stbiz->userid,50);
  // logfile.Write("userid=%s\n",stbiz->userid);

  GetXMLBuffer(strxmlbuffer,"obtid",stbiz->obtid,10);
  // logfile.Write("obtid=%s\n",stbiz->obtid);

  GetXMLBuffer(strxmlbuffer,"lat",&stbiz->lat);
  // logfile.Write("lat=%lf\n",stbiz->lat);

  GetXMLBuffer(strxmlbuffer,"lon",&stbiz->lon);
  // logfile.Write("lon=%lf\n",stbiz->lon);

  GetXMLBuffer(strxmlbuffer,"height",&stbiz->height);
  // logfile.Write("height=%lf\n",stbiz->height);

  strncpy(stbiz->xmlbuffer,strxmlbuffer,1000);

  return;
}

// ����ҵ��
bool biz10000(int clientfd)
{
  char strSendBuffer[1024]; // ���ͱ��ĵĻ�����

  memset(strSendBuffer,0,sizeof(strSendBuffer));
  strcpy(strSendBuffer,"ok");

  if (TcpWrite(clientfd,strSendBuffer) == false)
  {
    logfile.Write("biz10000 TcpWrite() failed.\n"); return false;
  }

  return true;
}

// ���û���¼
bool biz10001(struct st_biz *stbiz,int clientfd)
{
  CTimer Timer;
  char strSendBuffer[1024]; // ���ͱ��ĵĻ�����
  
  connection *conn=getconns();  // ��ȡһ�����ݿ����ӡ�

  // �����û�������Ϣ��T_USERINFO
  sqlstatement stmt(conn);
  stmt.prepare("insert into T_USERINFO(userid,downtime,ttytype,keyid) values(:1,sysdate,:2,SEQ_USERINFO.nextval)");
  stmt.bindin(1, stbiz->userid,50);
  stmt.bindin(2,&stbiz->ttytype);
  if (stmt.execute() != 0)
  {
    if (stmt.m_cda.rc!=1)
    {
      logfile.Write("insert T_USERINFO failed.\n%s\n%s\n",stmt.m_cda.message,stmt.m_sql); freeconns(conn); return false;
    }
  }

  logfile.Write("insert T_USERINFO =%lf\n",Timer.Elapsed());

  // �����û�������־��
  if (InsertUSERLOG(stbiz,conn)==false) { freeconns(conn); return false; }

  logfile.Write("insert T_USERLOG =%lf\n",Timer.Elapsed());

  char strobtid[6],strobtname[31],strlon[11],strlat[11];
  stmt.prepare("select obtid,obtname,lon,lat from T_OBTCODE where rsts=1 and rownum<=30");
  stmt.bindout(1,strobtid,5);
  stmt.bindout(2,strobtname,30);
  stmt.bindout(3,strlon,10);
  stmt.bindout(4,strlat,10);
  if (stmt.execute() != 0)
  {
    logfile.Write("select T_OBTCODE failed.\n%s\n%s\n",stmt.m_cda.message,stmt.m_sql); freeconns(conn); return false;
  }

  while (true)
  {
    memset(strobtid,0,sizeof(strobtid)); 
    memset(strobtname,0,sizeof(strobtname));
    memset(strlon,0,sizeof(strlon)); 
    memset(strlat,0,sizeof(strlat));
    memset(strSendBuffer,0,sizeof(strSendBuffer));

    if (stmt.next()!=0) break;

    sprintf(strSendBuffer,"<obtid>%s</obtid><obtname>%s</obtname><lon>%s</lon><lat>%s</lat><endl/>",strobtid,strobtname,strlon,strlat);

    if (TcpWrite(clientfd,strSendBuffer) == false)
    {
      logfile.Write("biz10001 TcpWrite() failed.\n"); freeconns(conn); return false;
    }
  }

  logfile.Write("select =%lf\n",Timer.Elapsed());

  // �����һ��ok
  strcpy(strSendBuffer,"ok");
  if (TcpWrite(clientfd,strSendBuffer) == false)
  {
    logfile.Write("biz10001 TcpWrite() failed.\n"); freeconns(conn); return false;
  }

  freeconns(conn);

  return true;
}

// �����û�������־��
bool InsertUSERLOG(struct st_biz *stbiz,connection *conn)
{
  sqlstatement stmt(conn);
  stmt.prepare("insert into T_USERLOG(logid,userid,atime,bizid,obtid,lon,lat,height,xmlbuffer) values(SEQ_USERLOG.nextval,:1,sysdate,:2,:3,:4,:5,:6,:7)");
  stmt.bindin(1, stbiz->userid,50);
  stmt.bindin(2,&stbiz->bizid);
  stmt.bindin(3, stbiz->obtid,10);
  stmt.bindin(4,&stbiz->lon);
  stmt.bindin(5,&stbiz->lat);
  stmt.bindin(6,&stbiz->height);
  stmt.bindin(7, stbiz->xmlbuffer,10000);

  if (stmt.execute() != 0)
  {
    logfile.Write("insert T_USERLOG failed.\n%s\n%s\n",stmt.m_cda.message,stmt.m_sql); return false;
  }

  return true;
}

// ��ȡ����ʵ��
bool biz10002(struct st_biz *stbiz,int clientfd)
{

   return true;
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
    if (vclientfd[ii]==clientfd) 
    { close(clientfd); vclientfd.erase(vclientfd.begin()+ii); return; }
  }
}

bool initconns()   // ��ʼ���ݿ����ӳ�
{
  for (int ii=0;ii<MAXCONNS;ii++)
  {
    logfile.Write("%d,connecttodb and pthread_mutex_init.\n",ii);

    // �������ݿ�
    if (conns[ii].connecttodb("shtqapp/pwdidc@snorcl11g_198","Simplified Chinese_China.ZHS16GBK",true)!=0)
    {
      logfile.Write("conns[%d].connettodb() failed.\n",ii); return false;
    }

    pthread_mutex_init(&mutex[ii],0); // ������
  }

  return true;
}

connection *getconns()
{
  // for (int jj=0;jj<1000;jj++)
  while (true)
  {
    for (int ii=0;ii<MAXCONNS;ii++)
    {
      if (pthread_mutex_trylock(&mutex[ii])==0) 
      {
        // logfile.Write("jj=%d,ii=%d\n",jj,ii);
        logfile.Write("get conns is %d.\n",ii);
        return &conns[ii]; 
      }
    }
  
    usleep(10000);
  }
}

bool freeconns(connection *in_conn)
{
  for (int ii=0;ii<MAXCONNS;ii++)
  {
    if (in_conn==&conns[ii]) 
    {
      logfile.Write("free conn %d.\n",ii);
      pthread_mutex_unlock(&mutex[ii]); in_conn=0; return true;
    }
  }

  return false;
}
