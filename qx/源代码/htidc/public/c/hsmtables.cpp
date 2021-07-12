#include "_public.h"
#include "_ooci.h"

char logfilename[301];
char connstr[101];
char srctname[51];
char dsttname[51];
char where[1024];
char hourstr[101];
char localhour[21];
int  maxcount=1;

connection conn;
CLogFile logfile;

void EXIT(int sig);

// ��ȡdsttname��ȫ������
char strColumnStr[2048];
bool GetColumnStr();

// ��ʾ����İ���
void _help(char *argv[]);

bool _hsmtables();

int main(int argc,char *argv[])
{
  if (argc != 2) { _help(argv); return -1; }

  memset(logfilename,0,sizeof(logfilename));
  memset(connstr,0,sizeof(connstr));
  memset(srctname,0,sizeof(srctname));
  memset(dsttname,0,sizeof(dsttname));
  memset(where,0,sizeof(where));
  memset(hourstr,0,sizeof(hourstr));

  GetXMLBuffer(argv[1],"logfilename",logfilename,300);
  GetXMLBuffer(argv[1],"connstr",connstr,100);
  GetXMLBuffer(argv[1],"srctname",srctname,50);
  GetXMLBuffer(argv[1],"dsttname",dsttname,50);
  GetXMLBuffer(argv[1],"where",where,1000);
  GetXMLBuffer(argv[1],"maxcount",&maxcount);
  GetXMLBuffer(argv[1],"hourstr",hourstr,2000);

  if (strlen(logfilename) == 0) { printf("logfilename is null.\n"); return -1; }
  if (strlen(connstr) == 0)     { printf("connstr is null.\n"); return -1; }
  if (strlen(srctname) == 0)    { printf("srctname is null.\n"); return -1; }
  if (strlen(dsttname) == 0)    { printf("dsttname is null.\n"); return -1; }
  if (strlen(where) == 0)       { printf("where is null.\n"); return -1; }
 if ( (maxcount<1) || (maxcount>1000) ) { printf("maxcount %d is invalid,should in 1-1000.\n",maxcount); return -1; }
  if (strlen(hourstr) == 0)     { printf("hourstr is null.\n"); return -1; }

  // �ر�ȫ�����źź��������
  CloseIOAndSignal();

  // ��������˳����ź�
  signal(SIGINT,EXIT); signal(SIGTERM,EXIT);

  // ����־�ļ�
  if (logfile.Open(logfilename,"a+") == false)
  {
    printf("logfile.Open(%s) failed.\n",logfilename); return -1;
  }

  while (true)
  {
    // �жϵ�ǰʱ���Ƿ�������ʱ��֮��
    memset(localhour,0,sizeof(localhour));
    LocalTime(localhour,"hh24");
    if (strstr(hourstr,localhour)==0) { sleep(60); continue; }

    // �������ݿ�
    if (conn.connecttodb(connstr,"Simplified Chinese_China.ZHS16GBK") != 0)
    {
      logfile.Write("connect database %s failed.\n",connstr); sleep(60); continue; 
    }

    logfile.Write("from table %s to %s.\n",srctname,dsttname);

    if (_hsmtables() == false) logfile.Write("_hsmtables failed.\n"); 

    conn.disconnect();

    sleep(60); 
  }

  return 0;
}

void EXIT(int sig)
{
  printf("�����˳���sig=%d\n\n",sig);

  exit(0);
}


// ��ʾ����İ���
void _help(char *argv[])
{
  printf("\nUsing:/htidc/public/bin/hsmtables \"<logfilename>/log/shqx/hsmtables_SURFDATA.log</logfilename><connstr>shqx/pwdidc@snorcl11g_198</connstr><srctname>T_SURFDATA</srctname><dsttname>T_SURFDATA_HIS</dsttname><where>where ddatetime<sysdate-10</where><maxcount>500</maxcount><hourstr>23,01,02,03,04,05,06</hourstr>\"\n\n");

  printf("����һ�����߳�������������е����ݡ�\n");
  printf("<logfilename>/log/shqx/hsmtables_SURFDATA.log</logfilename> ������������־�ļ�����\n");
  printf("<connstr>szidc/pwdidc@SZQX_10.153.97.251</connstr> Ŀ�����ݿ�����Ӳ�����\n");
  printf("<srctname>T_SURFDATA</srctname> ����Դ������\n");
  printf("<dsttname>T_SURFDATA_HIS</dsttname> Ŀ�����ݱ�����\n");
  printf("<where>where ddatetime<sysdate-10</where> ��Ǩ�����ݵ�������\n");
  printf("<maxcount>500</maxcount> ����ִ������Ǩ�Ƶļ�¼����ȡֵ��1-1000֮�䡣\n");
  printf("<hourstr>23,01,02,03,04,05,06</hourstr> ������������ʱ�Σ�Сʱ��ʱ��֮���ð�ǵĶ��ŷָ�����\n\n");

  return;
}

bool _hsmtables()
{
  // ��ȡdsttname��ȫ������
  if (GetColumnStr() == false) return false;

  int  ccount=0;
  char strrowid[51],strrowidn[maxcount][51];

  // �����ݵ�rowid��srctname���в����
  sqlstatement selstmt(&conn);
  selstmt.prepare("select rowid from %s %s",srctname,where);
  selstmt.bindout(1, strrowid,50);
  if (selstmt.execute() != 0)
  {
    logfile.Write("%s failed.\n%s\n",selstmt.m_sql,selstmt.m_cda.message); return false;
  }

  // ���ɲ���dsttname���ɾ��srctname���SQL
  int ii=0;
  char strInsertSQL[10241],strDeleteSQL[10241];
  memset(strInsertSQL,0,sizeof(strInsertSQL));
  memset(strDeleteSQL,0,sizeof(strDeleteSQL));
  sprintf(strInsertSQL,"insert into %s(%s) select %s from %s where rowid in (",dsttname,strColumnStr,strColumnStr,srctname);
  sprintf(strDeleteSQL,"delete from %s where rowid in (",srctname);

  char strtemp[11];
  for (ii=0; ii<maxcount; ii++)
  {
    memset(strtemp,0,sizeof(strtemp));
    if (ii==0) sprintf(strtemp,":%d",ii+1);
    if (ii >0) sprintf(strtemp,",:%d",ii+1);
    strcat(strInsertSQL,strtemp);
    strcat(strDeleteSQL,strtemp);
  }
  strcat(strInsertSQL,")");
  strcat(strDeleteSQL,")");

  sqlstatement insstmt(&conn);
  insstmt.prepare(strInsertSQL);

  sqlstatement delstmt(&conn);
  delstmt.prepare(strDeleteSQL);

  for (ii=0; ii<maxcount; ii++)
  {
    insstmt.bindin(ii+1,strrowidn[ii],50);
    delstmt.bindin(ii+1,strrowidn[ii],50);
  }

  // ÿmaxcount��¼��ִ��һ��
  while (true)
  {
    memset(strrowid,0,sizeof(strrowid));

    if (selstmt.next() != 0) break;

    strcpy(strrowidn[ccount],strrowid);

    ccount++;

    if (ccount == maxcount)
    {
      if (insstmt.execute() != 0)
      {
        if (insstmt.m_cda.rc != 1)
        {
          logfile.Write("_hsmtables insert %s failed.\n%s\n",dsttname,insstmt.m_cda.message); return false;
        }
      }

      if (delstmt.execute() != 0)
      {
        logfile.Write("_hsmtables delete %s failed.\n%s\n",dsttname,insstmt.m_cda.message); return false;
      }

      conn.commit();

      memset(strrowidn,0,sizeof(strrowidn));

      ccount=0;
    }

    if (fmod(selstmt.m_cda.rpc,10000) < 1)
    {
      logfile.Write("%s to %s ok(%d).\n",srctname,dsttname,selstmt.m_cda.rpc);

      // �жϵ�ǰʱ���Ƿ�������ʱ��֮��
      memset(localhour,0,sizeof(localhour));
      LocalTime(localhour,"hh24");
      if (strstr(hourstr,localhour)==0) return true;
    }
  }

  // ������ѭ�������ʱ���������maxcount���������ﴦ��
  for (ii=0; ii<ccount; ii++)
  {
    insstmt.prepare("\
      BEGIN\
        insert into %s(%s) select %s from %s where rowid=:1;\
        delete from %s where rowid=:2;\
      END;",dsttname,strColumnStr,strColumnStr,srctname,srctname);
    insstmt.bindin(1,strrowidn[ii],50);
    insstmt.bindin(2,strrowidn[ii],50);
    if (insstmt.execute() != 0)
    {
      if (insstmt.m_cda.rc != 1)
      {
        logfile.Write("_hsmtables insert %s or delete %s failed.\n%s\n",dsttname,srctname,insstmt.m_cda.message); return false;
      }
    }
  }

  conn.commit();

  logfile.Write("%s to %s finish(%d).\n",srctname,dsttname,selstmt.m_cda.rpc);

  return true;
}

// ��ȡdsttname��ȫ������
bool GetColumnStr()
{
  memset(strColumnStr,0,sizeof(strColumnStr));
 
  char column_name[51];
  sqlstatement stmt(&conn);
  stmt.prepare("select lower(column_name) from USER_TAB_COLUMNS where table_name=upper('%s') order by column_id",dsttname);
  stmt.bindout(1,column_name,50);

  if (stmt.execute() != 0)
  {
    logfile.Write("%s failed.\n%s\n",stmt.m_sql,stmt.m_cda.message); return false;
  }

  while(true)
  {
    memset(column_name,0,sizeof(column_name));
    if (stmt.next()!=0) break;

    if (stmt.m_cda.rpc>1) strcat(strColumnStr,",");

    strcat(strColumnStr,column_name);
  }

  if (stmt.m_cda.rpc==0) { logfile.Write("��%s�����ڡ�\n",dsttname); return false; }

  return true;
}
