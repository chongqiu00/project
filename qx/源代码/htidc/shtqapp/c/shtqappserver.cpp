/*
 �Ϻ�����APP��������������
*/

#include "_public.h"
#include "_ooci.h"

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
} stbiz;

// ��xml����������stbiz�ṹ��
void xmltobiz(char *strxmlbuffer);

CTcpServer TcpServer;
CLogFile   logfile;
connection conn;

char strRecvBuffer[TCPBUFLEN+10]; // ���ձ��ĵĻ�����
char strSendBuffer[TCPBUFLEN+10]; // ���ͱ��ĵĻ�����

// �����˳�ʱ���õĺ���
void FathEXIT(int sig);
void ChldEXIT(int sig);

// ҵ�������������
void ChldMain();

// ����ҵ��
bool biz10000();

// ���û���¼ҵ��
bool biz10001();

// ��ȡ����ʵ��
bool biz10002();

// �����û�������־��
bool InsertUSERLOG();

int main(int argc,char *argv[])
{
  if (argc != 3)
  {
    printf("\n");
    printf("Using:/htidc/shtqapp/bin/shtqappserver logfilename port\n");

    printf("Example:/htidc/shtqapp/bin/shtqappserver /log/shtqapp/shtqappserver.log 5015\n\n");
    printf("���������Ϻ�����APP����ķ���ˡ�\n");
    printf("logfilename ��־�ļ�����\n");
    printf("port ���ڴ����ļ���TCP�˿ڡ�\n");

    return -1;
  }

  // �ر�ȫ�����źź��������
  // �����ź�,��shell״̬�¿��� "kill + ���̺�" ������ֹЩ����
  // ���벻Ҫ�� "kill -9 +���̺�" ǿ����ֹ
  CloseIOAndSignal(); signal(SIGINT,FathEXIT); signal(SIGTERM,FathEXIT);

  // �򿪳���������־������һ������̳�����־�����Զ��л�
  if (logfile.Open(argv[1],"a+",false) == false)
  {
    printf("logfile.Open(%s) failed.\n",argv[1]); return -1;
  }

  logfile.Write("shtqappserver started(%s).\n",argv[2]);

  if (TcpServer.InitServer(atoi(argv[2])) == false)
  {
    logfile.Write("TcpServer.InitServer(%s) failed.\n",argv[2]); exit(-1);
  }

  while (true)
  {
    // �ȴ��ͻ��˵�����
    if (TcpServer.Accept() == false)
    {
      logfile.Write("TcpServer.Accept() failed.\n"); continue;
    }

    // �µĿͻ�������
    if (fork() > 0)
    {
      // �����̹رոս���������sock���ӣ����ص�Accept��������
      TcpServer.CloseClient(); continue;
    }

    // �����ӽ��̵�����
    signal(SIGINT,ChldEXIT); signal(SIGTERM,ChldEXIT);

    // �ӽ�����Ҫ�ص�������sock
    TcpServer.CloseListen();

    // ҵ�������������
    ChldMain();

    ChldEXIT(0);
  }

  return 0;
}

// �������˳�ʱ���õĺ���
void FathEXIT(int sig)
{
  if (sig > 0)
  {
    signal(sig,SIG_IGN); signal(SIGINT,SIG_IGN); signal(SIGTERM,SIG_IGN);
    logfile.Write("catching the signal(%d).\n",sig);
  }

  kill(0,15);

  logfile.Write("shtqappserver EXIT.\n");

  exit(0);
}

// �ӽ����˳�ʱ���õĺ���
void ChldEXIT(int sig)
{
  if (sig > 0) 
  {
    signal(sig,SIG_IGN); signal(SIGINT,SIG_IGN); signal(SIGTERM,SIG_IGN);
  }

  exit(0);
}

// ҵ�������������
void ChldMain()
{
  while (true)
  {
    memset(strRecvBuffer,0,sizeof(strRecvBuffer));
    memset(strSendBuffer,0,sizeof(strSendBuffer));

    // ���տͻ��˵�ҵ�������ģ��������false����Ϊ�ǿͻ����˳�������ԭ�򣬲�д������־
    if (TcpServer.Read(strRecvBuffer,50) == false)
    {
      // logfile.Write("TcpServer.Read() failed.\n"); 
      return;
    }

    logfile.Write("strRecvBuffer=%s\n",strRecvBuffer);  // xxxxxx

    // �Ѳ�����������
    xmltobiz(strRecvBuffer);

    if (stbiz.bizid==10000)    // ��������
    {
      if (biz10000()==true) continue;
      else return;
    }

    CTimer Timer;

    // �������ݿ�
    if (conn.connecttodb("shtqapp/pwdidc@snorcl11g_198","Simplified Chinese_China.ZHS16GBK",true)!=0)
    {
      logfile.Write("conn.connettodb() failed.\n"); return;
    }

    logfile.Write("conn=%lf\n",Timer.Elapsed());

    // ���û���¼ 
    if (stbiz.bizid==10001)    
    {
      if (biz10001()==true) continue;
      else return;
    }

    // ��ȡ����ʵ��
    if (stbiz.bizid==10002)    
    {
      if (biz10002()==true) continue;
      else return;
    }

    // ������

    logfile.Write("�Ƿ�����%s\n",strRecvBuffer); return;
  }
}

// ��xml����������starg�ṹ��
void xmltobiz(char *strxmlbuffer)
{
  memset(&stbiz,0,sizeof(struct st_biz));

  // ҵ�����
  GetXMLBuffer(strxmlbuffer,"bizid",&stbiz.bizid);
  // logfile.Write("bizid=%d\n",stbiz.bizid);

  // �û��豸ID
  GetXMLBuffer(strxmlbuffer,"userid",stbiz.userid,50);
  // logfile.Write("userid=%s\n",stbiz.userid);

  GetXMLBuffer(strxmlbuffer,"obtid",stbiz.obtid,10);
  // logfile.Write("obtid=%s\n",stbiz.obtid);

  GetXMLBuffer(strxmlbuffer,"lat",&stbiz.lat);
  // logfile.Write("lat=%lf\n",stbiz.lat);

  GetXMLBuffer(strxmlbuffer,"lon",&stbiz.lon);
  // logfile.Write("lon=%lf\n",stbiz.lon);

  GetXMLBuffer(strxmlbuffer,"height",&stbiz.height);
  // logfile.Write("height=%lf\n",stbiz.height);

  strncpy(stbiz.xmlbuffer,strxmlbuffer,1000);

  return;
}

// ����ҵ��
bool biz10000()
{
  memset(strSendBuffer,0,sizeof(strSendBuffer));
  strcpy(strSendBuffer,"ok");

  if (TcpServer.Write(strSendBuffer) == false)
  {
    logfile.Write("biz10000 TcpServer.Write() failed.\n"); return false;
  }

  return true;
}

// ���û���¼
bool biz10001()
{
CTimer Timer;
  // �����û�������Ϣ��T_USERINFO
  sqlstatement stmt(&conn);
  stmt.prepare("insert into T_USERINFO(userid,downtime,ttytype,keyid) values(:1,sysdate,:2,SEQ_USERINFO.nextval)");
  stmt.bindin(1, stbiz.userid,50);
  stmt.bindin(2,&stbiz.ttytype);
  if (stmt.execute() != 0)
  {
    if (stmt.m_cda.rc!=1)
    {
      logfile.Write("insert T_USERINFO failed.\n%s\n%s\n",stmt.m_cda.message,stmt.m_sql); return false;
    }
  }

  logfile.Write("insert T_USERINFO =%lf\n",Timer.Elapsed());

  // �����û�������־��
  if (InsertUSERLOG()==false) return false;

  logfile.Write("insert T_USERLOG =%lf\n",Timer.Elapsed());

  char strobtid[6],strobtname[31],strlon[11],strlat[11];
  stmt.prepare("select obtid,obtname,lon,lat from T_OBTCODE where rsts=1 and rownum<=30");
  stmt.bindout(1,strobtid,5);
  stmt.bindout(2,strobtname,30);
  stmt.bindout(3,strlon,10);
  stmt.bindout(4,strlat,10);
  if (stmt.execute() != 0)
  {
    logfile.Write("select T_OBTCODE failed.\n%s\n%s\n",stmt.m_cda.message,stmt.m_sql); return false;
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

    if (TcpServer.Write(strSendBuffer) == false)
    {
      logfile.Write("biz10001 TcpServer.Write() failed.\n"); return false;
    }
  }

  logfile.Write("select =%lf\n",Timer.Elapsed());
  // �����һ��ok
  strcpy(strSendBuffer,"ok");
  if (TcpServer.Write(strSendBuffer) == false)
  {
    logfile.Write("biz10001 TcpServer.Write() failed.\n"); return false;
  }

  return true;
}

// �����û�������־��
bool InsertUSERLOG()
{
  sqlstatement stmt(&conn);
  stmt.prepare("insert into T_USERLOG(logid,userid,atime,bizid,obtid,lon,lat,height,xmlbuffer) values(SEQ_USERLOG.nextval,:1,sysdate,:2,:3,:4,:5,:6,:7)");
  stmt.bindin(1, stbiz.userid,50);
  stmt.bindin(2,&stbiz.bizid);
  stmt.bindin(3, stbiz.obtid,10);
  stmt.bindin(4,&stbiz.lon);
  stmt.bindin(5,&stbiz.lat);
  stmt.bindin(6,&stbiz.height);
  stmt.bindin(7, stbiz.xmlbuffer,10000);

  if (stmt.execute() != 0)
  {
    logfile.Write("insert T_USERLOG failed.\n%s\n%s\n",stmt.m_cda.message,stmt.m_sql); return false;
  }

  return true;
}

// ��ȡ����ʵ��
bool biz10002()
{

   return true;
}
