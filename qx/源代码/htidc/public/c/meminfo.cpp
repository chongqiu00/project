#include "_public.h"

void EXIT(int sig);

CLogFile logfile;

struct st_meminfo
{
  double memtotal;
  double memused;
  double memfree;
  double memusep;
};

struct st_meminfo stmeminfo;

bool LoadMEMInfo();

int main(int argc,char *argv[])
{
  if (argc != 4)
  {
    printf("\n");
    printf("Using:./meminfo hostname logfilename outputpath\n");

    printf("Example:/htidc/public/bin/meminfo 10.153.98.26 /tmp/htidc/log/meminfo.log /tmp/htidc/monclient\n\n");
    printf("�˳����ȡ/proc/meminfo�ļ����ѱ����������ڴ�ʹ������Ϣд��xml�ļ���\n");
    printf("hostname�Ǳ�����������������Ϊ�˷���ʶ��Ҳ������IP��\n");
    printf("logfilename�Ǳ��������־�ļ�����\n");
    printf("outputpath�������xml�ļ���ŵ�Ŀ¼��\n");
    printf("�˳�����������Ҫ��صķ������ϣ�������ֻ����Linuxϵͳ�����ɼ����xml�ļ����ļ���������͸����ݴ�����������⡣\n\n\n");
 
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

  memset(&stmeminfo,0,sizeof(struct st_meminfo));

  if (LoadMEMInfo() ==false) EXIT(-1);
  
  char strLocalTime[21];
  memset(strLocalTime,0,sizeof(strLocalTime));
  LocalTime(strLocalTime,"yyyymmddhh24miss");

  char strXMLFileName[301];
  memset(strXMLFileName,0,sizeof(strXMLFileName));
  snprintf(strXMLFileName,300,"%s/meminfo_%s_%s.xml",argv[3],strLocalTime,argv[1]);

  CFile XMLFile;
  if (XMLFile.OpenForRename(strXMLFileName,"w+") == false )
  {
    logfile.Write("XMLFile.OpenForRename(%s) failed.\n",strXMLFileName); EXIT(-1);
  }

  XMLFile.Fprintf("<data>\n");

  XMLFile.Fprintf("<nodip>%s</nodip><crttime>%s</crttime><memtotal>%.02f</memtotal><memused>%.02f</memused><memfree>%.02f</memfree><memusep>%.02f</memusep><endl/>\n",argv[1],strLocalTime,stmeminfo.memtotal,stmeminfo.memused,stmeminfo.memfree,stmeminfo.memusep);

  XMLFile.Fprintf("</data>\n");

  XMLFile.CloseAndRename();

  logfile.Write("create %s ok.\n",strXMLFileName);

  exit(0);
}

void EXIT(int sig)
{
  if (sig > 0) signal(sig,SIG_IGN);

  logfile.Write("catching the signal(%d).\n",sig);

  logfile.Write("meminfo exit.\n");

  exit(0);
}


bool LoadMEMInfo()
{
  CFile MEMFile;

  if (MEMFile.Open("/proc/meminfo","r") == false )
  {
    logfile.Write("MEMFile.Open(/proc/meminfo) failed.\n"); return false;
  }

  CCmdStr CmdStr;
  char strBuffer[1024];

  while (true)
  {
    memset(strBuffer,0,sizeof(strBuffer));

    if (MEMFile.FFGETS(strBuffer,500) == false) break;

    // ���ַ����м�Ķ���ո�ȫ��ת��Ϊһ���ո�
    UpdateStr(strBuffer,"  "," ");

    ToLower(strBuffer);

    CmdStr.SplitToCmd(strBuffer," ");

    if (strcmp(CmdStr.m_vCmdStr[0].c_str(),"memtotal:")==0) stmeminfo.memtotal=atof(CmdStr.m_vCmdStr[1].c_str())/(1024.0);
    if (strcmp(CmdStr.m_vCmdStr[0].c_str(),"memfree:")==0)  stmeminfo.memfree=atof(CmdStr.m_vCmdStr[1].c_str())/(1024.0);
  }

  stmeminfo.memused=stmeminfo.memtotal-stmeminfo.memfree;
  stmeminfo.memusep=(stmeminfo.memused/stmeminfo.memtotal)*100.0;

  return true;
}



