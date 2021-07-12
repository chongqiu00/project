/*
 上海天气APP软件服务端主程序。
*/

#include "_public.h"
#include "_ooci.h"

// 业务请求
struct st_biz
{
  int  bizid;               // 业务代码
  char userid[51];          // 设备ID
  int  ttytype;             // 用户的设备类型，0-未知；1-IOS；2-Andriod，2-鸿蒙。
  int  usertype;            // 用户分类，0-未知；1-普通用户；2-气象志愿者；3-内部用户。
  double lon;
  double lat;
  double height;
  char   obtid[11];
  char   xmlbuffer[1001];
} stbiz;

// 把xml解析到参数stbiz结构中
void xmltobiz(char *strxmlbuffer);

CTcpServer TcpServer;
CLogFile   logfile;
connection conn;

char strRecvBuffer[TCPBUFLEN+10]; // 接收报文的缓冲区
char strSendBuffer[TCPBUFLEN+10]; // 发送报文的缓冲区

// 程序退出时调用的函数
void FathEXIT(int sig);
void ChldEXIT(int sig);

// 业务处理进程主函数
void ChldMain();

// 心跳业务
bool biz10000();

// 新用户登录业务
bool biz10001();

// 获取天气实况
bool biz10002();

// 插入用户请求日志表
bool InsertUSERLOG();

int main(int argc,char *argv[])
{
  if (argc != 3)
  {
    printf("\n");
    printf("Using:/htidc/shtqapp/bin/shtqappserver logfilename port\n");

    printf("Example:/htidc/shtqapp/bin/shtqappserver /log/shtqapp/shtqappserver.log 5015\n\n");
    printf("本程序是上海天气APP软件的服务端。\n");
    printf("logfilename 日志文件名。\n");
    printf("port 用于传输文件的TCP端口。\n");

    return -1;
  }

  // 关闭全部的信号和输入输出
  // 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程
  // 但请不要用 "kill -9 +进程号" 强行终止
  CloseIOAndSignal(); signal(SIGINT,FathEXIT); signal(SIGTERM,FathEXIT);

  // 打开程序运行日志，这是一个多进程程序，日志不能自动切换
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
    // 等待客户端的连接
    if (TcpServer.Accept() == false)
    {
      logfile.Write("TcpServer.Accept() failed.\n"); continue;
    }

    // 新的客户端连接
    if (fork() > 0)
    {
      // 父进程关闭刚建立起来的sock连接，并回到Accept继续监听
      TcpServer.CloseClient(); continue;
    }

    // 进入子进程的流程
    signal(SIGINT,ChldEXIT); signal(SIGTERM,ChldEXIT);

    // 子进程需要关掉监听的sock
    TcpServer.CloseListen();

    // 业务处理进程主函数
    ChldMain();

    ChldEXIT(0);
  }

  return 0;
}

// 父进程退出时调用的函数
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

// 子进程退出时调用的函数
void ChldEXIT(int sig)
{
  if (sig > 0) 
  {
    signal(sig,SIG_IGN); signal(SIGINT,SIG_IGN); signal(SIGTERM,SIG_IGN);
  }

  exit(0);
}

// 业务处理进程主函数
void ChldMain()
{
  while (true)
  {
    memset(strRecvBuffer,0,sizeof(strRecvBuffer));
    memset(strSendBuffer,0,sizeof(strSendBuffer));

    // 接收客户端的业务请求报文，如果返回false，认为是客户端退出或网络原因，不写错误日志
    if (TcpServer.Read(strRecvBuffer,50) == false)
    {
      // logfile.Write("TcpServer.Read() failed.\n"); 
      return;
    }

    logfile.Write("strRecvBuffer=%s\n",strRecvBuffer);  // xxxxxx

    // 把参数解析出来
    xmltobiz(strRecvBuffer);

    if (stbiz.bizid==10000)    // 心跳报文
    {
      if (biz10000()==true) continue;
      else return;
    }

    CTimer Timer;

    // 连接数据库
    if (conn.connecttodb("shtqapp/pwdidc@snorcl11g_198","Simplified Chinese_China.ZHS16GBK",true)!=0)
    {
      logfile.Write("conn.connettodb() failed.\n"); return;
    }

    logfile.Write("conn=%lf\n",Timer.Elapsed());

    // 新用户登录 
    if (stbiz.bizid==10001)    
    {
      if (biz10001()==true) continue;
      else return;
    }

    // 获取天气实况
    if (stbiz.bizid==10002)    
    {
      if (biz10002()==true) continue;
      else return;
    }

    // 体力活

    logfile.Write("非法报文%s\n",strRecvBuffer); return;
  }
}

// 把xml解析到参数starg结构中
void xmltobiz(char *strxmlbuffer)
{
  memset(&stbiz,0,sizeof(struct st_biz));

  // 业务代码
  GetXMLBuffer(strxmlbuffer,"bizid",&stbiz.bizid);
  // logfile.Write("bizid=%d\n",stbiz.bizid);

  // 用户设备ID
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

// 心跳业务
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

// 新用户登录
bool biz10001()
{
CTimer Timer;
  // 插入用户基本信息表T_USERINFO
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

  // 插入用户请求日志表
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
  // 最后发送一个ok
  strcpy(strSendBuffer,"ok");
  if (TcpServer.Write(strSendBuffer) == false)
  {
    logfile.Write("biz10001 TcpServer.Write() failed.\n"); return false;
  }

  return true;
}

// 插入用户请求日志表
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

// 获取天气实况
bool biz10002()
{

   return true;
}
