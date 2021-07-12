#include "_public.h"
#include "_ooci.h"

void EXIT(int sig);

struct st_TBSPACEINFO
{
  long  taskid;
  char  nodip[31];
  char  tablespace[101];
  double  total;
  double  used;
  double  available;
  double  usep;
  double alarmvalue;
  int  alarmsts;
  char crttime[21];
  int  rsts;
};

struct st_TBSPACEINFO stTBSPACEINFO;

CLogFile logfile;
connection conn;

int main(int argc,char *argv[])
{
  if (argc != 5)
  {
    printf("\n");
    printf("Using:./tbspaceinfo hostname logfilename outputpath username/password@tnsnames\n");

    printf("Example:/htidc/public/bin/tbspaceinfo 10.153.98.13 /tmp/htidc/log/tbspaceinfo_10.153.98.13.log /tmp/htidc/monclient shqx/pwdidc@SZQX_10.153.98.13\n\n");
    printf("�˳�������Զ�����ݿ⣬��Զ�����ݿ��ռ�ʹ������Ϣд��xml�ļ���\n");
    printf("hostname�Ǳ�����������������Ϊ�˷���ʶ��Ҳ������IP��\n");
    printf("logfilename�Ǳ��������־�ļ�����\n");
    printf("outputpath�������xml�ļ���ŵ�Ŀ¼��\n");
    printf("username/password@tnsnamesΪ����ص�Զ�����ݿ���û���/����@��������\n");
    printf("�˳�����������������Ӧ�ó���ķ������ϡ�\n\n\n");

    return -1;
  }

  // �ر�ȫ�����źź��������
  // �����ź�,��shell״̬�¿��� "kill + ���̺�" ������ֹЩ����
  // ���벻Ҫ�� "kill -9 +���̺�" ǿ����ֹ
  CloseIOAndSignal(); signal(SIGINT,EXIT); signal(SIGTERM,EXIT);

  if (logfile.Open(argv[2],"a+") == false)
  {
    printf("logfile.Open(%s) failed.\n",argv[2]); return -1;
  }

  if (conn.connecttodb(argv[4],"Simplified Chinese_China.ZHS16GBK") != 0)
  {
    logfile.Write("conn.connecttodb(%s) failed.\n",argv[4]); return -1;
  }

  sqlstatement stmt;
  stmt.connect(&conn);
  stmt.prepare("\
    select f.tablespace_name,a.total,u.used,f.free,(u.used/a.total)*100  from\
        (select tablespace_name,sum(bytes/(1024*1024*1024)) total from DBA_DATA_FILES\
          group by tablespace_name) a,\
        (select tablespace_name,round(sum(bytes/(1024*1024*1024))) used from DBA_EXTENTS\
          group by tablespace_name) u,\
        (select tablespace_name,round(sum(bytes/(1024*1024*1024))) free from DBA_FREE_SPACE\
          group by tablespace_name) f\
    where a.tablespace_name = f.tablespace_name\
      and a.tablespace_name = u.tablespace_name\
      and f.tablespace_name in (select tablespace_name from DBA_TABLESPACES where contents='PERMANENT')");
  stmt.bindout(1,stTBSPACEINFO.tablespace,100);
  stmt.bindout(2,&stTBSPACEINFO.total);
  stmt.bindout(3,&stTBSPACEINFO.used);
  stmt.bindout(4,&stTBSPACEINFO.available);
  stmt.bindout(5,&stTBSPACEINFO.usep);

  if (stmt.execute() != 0)
  {
    logfile.Write("select DBA_DATA_FILES,DBA_EXTENTS,DBA_FREE_SPACE failed.\n%s\n",stmt.m_cda.message); return -1;
  }

  char strLocalTime[21];
  memset(strLocalTime,0,sizeof(strLocalTime));
  LocalTime(strLocalTime,"yyyymmddhh24miss");

  char strXMLFileName[301];
  memset(strXMLFileName,0,sizeof(strXMLFileName));
  snprintf(strXMLFileName,300,"%s/tbspaceinfo_%s_%s.xml",argv[3],strLocalTime,argv[1]);

  CFile XMLFile;
  if (XMLFile.OpenForRename(strXMLFileName,"w+") == false )
  {
    logfile.Write("XMLFile.OpenForRename(%s) failed.\n",strXMLFileName); return -1;
  }

  XMLFile.Fprintf("<data>\n");

  while (true)
  {
    memset(&stTBSPACEINFO,0,sizeof(stTBSPACEINFO));

    if (stmt.next() != 0) break;

    XMLFile.Fprintf(\
            "<nodip>%s</nodip>"\
            "<crttime>%s</crttime>"\
            "<tablespace>%s</tablespace>"\
            "<total>%0.02f</total>"\
            "<used>%0.02f</used>"\
            "<available>%0.02f</available>"\
            "<usep>%0.02f</usep><endl/>\n",
             argv[1],
             strLocalTime,
             stTBSPACEINFO.tablespace,
             stTBSPACEINFO.total,
             stTBSPACEINFO.used,
             stTBSPACEINFO.available,
             stTBSPACEINFO.usep);
  }

  XMLFile.Fprintf("</data>\n");

  XMLFile.CloseAndRename();

  logfile.Write("�����ļ�%s.\n",strXMLFileName);

  exit(0);
}

void EXIT(int sig)
{
  if (sig > 0) signal(sig,SIG_IGN);

  logfile.Write("catching the signal(%d).\n",sig);

  logfile.Write("tbspaceinfo exit.\n");

  exit(0);
}


