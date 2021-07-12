#include "_public.h"
#include "_ooci.h"

struct st_data
{
  char id[6];
  char ddatetime[20];
  int  t;
  int  r;
  int  wd;
  int  wf;
  int  p;
  int  u;
};

int main()
{
  // 连接数据库
  connection conn;
  if (conn.connecttodb("scott/tiger","Simplified Chinese_China.ZHS16GBK")!=0)
  {
    printf("conn.connecttodb() failed.\n"); return -1;
  }

  CFile File;

  // 打开文件
  if (File.Open("/tmp/data/dat20191210122500.txt","r")==false)
  {
    printf("File.Open(dat20191210122500.txt) failed.\n"); return -1;
  }

  char strbuf[301];
  struct st_data stdata;

  CCmdStr cmdstr;    // 定义字符串拆分的类

  sqlstatement stmt(&conn);  // 定义sql语句操作的类
 
  while (true)
  {
    memset(strbuf,0,sizeof(strbuf));
    memset(&stdata,0,sizeof(struct st_data));

    // 读取一行数据
    if (File.Fgets(strbuf,100,true)==false) break;

    // 解析数据内容到结构体中
    cmdstr.SplitToCmd(strbuf,",");
    cmdstr.GetValue(0,stdata.id,5);
    cmdstr.GetValue(1,stdata.ddatetime,14);
    cmdstr.GetValue(2,&stdata.t);
    cmdstr.GetValue(3,&stdata.r);
    cmdstr.GetValue(4,&stdata.wd);
    cmdstr.GetValue(5,&stdata.wf);
    cmdstr.GetValue(6,&stdata.p);
    cmdstr.GetValue(7,&stdata.u);
   
    printf("id=%s,ddatetime=%s,t=%d,r=%d,wd=%d,wf=%d,p=%d,u=%d\n",stdata.id,stdata.ddatetime,stdata.t,stdata.r,stdata.wd,stdata.wf,stdata.p,stdata.u);

    // 准备SQL语句
    stmt.prepare("insert into t_stdata(id,ddatetime,t,r,wd,wf,p,u) values('%s','%s',%d,%d,%d,%d,%d,%u)",stdata.id,stdata.ddatetime,stdata.t,stdata.r,stdata.wd,stdata.wf,stdata.p,stdata.u);

    // 执行SQL语句
    if (stmt.execute() != 0)
    {
      printf("insert into t_stdata failed\n%s\n",stmt.m_cda.message);
    }
  }

  // 提交数据库事务
  conn.commit();

  return 0;
}
