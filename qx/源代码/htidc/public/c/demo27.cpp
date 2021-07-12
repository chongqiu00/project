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
  // �������ݿ�
  connection conn;
  if (conn.connecttodb("scott/tiger","Simplified Chinese_China.ZHS16GBK")!=0)
  {
    printf("conn.connecttodb() failed.\n"); return -1;
  }

  CFile File;

  // ���ļ�
  if (File.Open("/tmp/data/dat20191210122500.txt","r")==false)
  {
    printf("File.Open(dat20191210122500.txt) failed.\n"); return -1;
  }

  char strbuf[301];
  struct st_data stdata;

  CCmdStr cmdstr;    // �����ַ�����ֵ���

  sqlstatement stmt(&conn);  // ����sql����������
 
  while (true)
  {
    memset(strbuf,0,sizeof(strbuf));
    memset(&stdata,0,sizeof(struct st_data));

    // ��ȡһ������
    if (File.Fgets(strbuf,100,true)==false) break;

    // �����������ݵ��ṹ����
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

    // ׼��SQL���
    stmt.prepare("insert into t_stdata(id,ddatetime,t,r,wd,wf,p,u) values('%s','%s',%d,%d,%d,%d,%d,%u)",stdata.id,stdata.ddatetime,stdata.t,stdata.r,stdata.wd,stdata.wf,stdata.p,stdata.u);

    // ִ��SQL���
    if (stmt.execute() != 0)
    {
      printf("insert into t_stdata failed\n%s\n",stmt.m_cda.message);
    }
  }

  // �ύ���ݿ�����
  conn.commit();

  return 0;
}
