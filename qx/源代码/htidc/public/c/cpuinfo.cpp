#include "_public.h"

void EXIT(int sig);

CLogFile logfile;

struct st_cpuinfo
{
  double user;
  double sys;
  double wait;
  double nice;
  double idle;
  double irq;
  double softirq;
  double total;
};

struct st_cpuinfo stcpuinfo1,stcpuinfo2,stcpuinfo3;

bool LoadCPUInfo(struct st_cpuinfo &stcpuinfo);

int main(int argc,char *argv[])
{
  if (argc != 4)
  {
    printf("\n");
    printf("Using:./cpuinfo hostname logfilename outputpath\n");
    printf("Example:/htidc/public/bin/cpuinfo 118.89.50.198 /tmp/htidc/log/cpuinfo.log /tmp/htidc/monclient\n\n");

    printf("�˳����ȡ/proc/stat�ļ����ѱ���������CPUʹ������Ϣд��xml�ļ���\n");
    printf("hostname�Ǳ�����������������Ϊ�˷���ʶ��Ҳ������IP��\n");
    printf("logfilename�Ǳ��������־�ļ�����\n");
    printf("outputpath�������xml�ļ���ŵ�Ŀ¼��\n");
    printf("�˳�����������Ҫ��صķ������ϣ�������ֻ����Linuxϵͳ�����ɼ����xml�ļ����ļ���������͸����ݴ�����������⡣\n\n\n");
 
    return -1;
  }

  //memset(strHostName,0,sizeof(strHostName));
  //strncpy(strHostName,argv[2],20);

  // �ر�ȫ�����źź��������
  // �����ź�,��shell״̬�¿��� "kill + ���̺�" ������ֹЩ����
  // ���벻Ҫ�� "kill -9 +���̺�" ǿ����ֹ
  CloseIOAndSignal(); signal(SIGINT,EXIT); signal(SIGTERM,EXIT);

  if (logfile.Open(argv[2],"a+") == false)
  {
    printf("logfile.Open(%s) failed.\n",argv[2]); return -1;
  }

  memset(&stcpuinfo1,0,sizeof(struct st_cpuinfo));
  memset(&stcpuinfo2,0,sizeof(struct st_cpuinfo));
  memset(&stcpuinfo3,0,sizeof(struct st_cpuinfo));

  if (LoadCPUInfo(stcpuinfo1) ==false) return -1;
  
  sleep(60);

  if (LoadCPUInfo(stcpuinfo2) ==false) return -1;

  stcpuinfo3.user=stcpuinfo2.user-stcpuinfo1.user;
  stcpuinfo3.sys=stcpuinfo2.sys-stcpuinfo1.sys;
  stcpuinfo3.wait=stcpuinfo2.wait-stcpuinfo1.wait;
  stcpuinfo3.nice=stcpuinfo2.nice-stcpuinfo1.nice;
  stcpuinfo3.idle=stcpuinfo2.idle-stcpuinfo1.idle;
  stcpuinfo3.irq=stcpuinfo2.irq-stcpuinfo1.irq;
  stcpuinfo3.softirq=stcpuinfo2.softirq-stcpuinfo1.softirq;

  stcpuinfo3.total=stcpuinfo3.user+stcpuinfo3.sys+stcpuinfo3.wait+stcpuinfo3.nice+stcpuinfo3.idle+stcpuinfo3.irq+stcpuinfo3.softirq;

  char strLocalTime[21];
  memset(strLocalTime,0,sizeof(strLocalTime));
  LocalTime(strLocalTime,"yyyymmddhh24miss");

  char strXMLFileName[301];
  memset(strXMLFileName,0,sizeof(strXMLFileName));
  snprintf(strXMLFileName,300,"%s/cpuinfo_%s_%s.xml",argv[3],strLocalTime,argv[1]);

  CFile XMLFile;
  if (XMLFile.OpenForRename(strXMLFileName,"w+") == false )
  {
    logfile.Write("XMLFile.OpenForRename(%s) failed.\n",strXMLFileName); return -1;
  }

  XMLFile.Fprintf("<data>\n");

  XMLFile.Fprintf("<nodip>%s</nodip><crttime>%s</crttime><user>%0.02f</user><sys>%0.02f</sys><wait>%0.02f</wait><nice>%0.02f</nice><idle>%0.02f</idle><usep>%0.02f</usep><endl/>\n",argv[1],strLocalTime,stcpuinfo3.user/stcpuinfo3.total*100.0,stcpuinfo3.sys/stcpuinfo3.total*100.0,stcpuinfo3.wait/stcpuinfo3.total*100.0,stcpuinfo3.nice/stcpuinfo3.total*100.0,stcpuinfo3.idle/stcpuinfo3.total*100.0,100.0-stcpuinfo3.nice/stcpuinfo3.total*100.0);

  XMLFile.Fprintf("</data>\n");

  XMLFile.CloseAndRename();

  logfile.Write("create %s ok.\n",strXMLFileName);

  exit(0);
}

void EXIT(int sig)
{
  if (sig > 0) signal(sig,SIG_IGN);

  logfile.Write("catching the signal(%d).\n",sig);

  logfile.Write("cpuinfo exit.\n");

  exit(0);
}


bool LoadCPUInfo(struct st_cpuinfo &stcpuinfo)
{
  CFile CPUFile;

  if (CPUFile.Open("/proc/stat","r") == false )
  {
    logfile.Write("CPUFile.OpenForRead(/proc/stat) failed.\n"); return false;
  }

  CCmdStr CmdStr;
  char strBuffer[1024];

  while (true)
  {
    memset(strBuffer,0,sizeof(strBuffer));

    if (CPUFile.FFGETS(strBuffer,500) == false) break;

    // ɾ���ַ���ǰ��Ŀո�
    DeleteLRChar(strBuffer,' ');

    // ���ַ����м�Ķ���ո�ȫ��ת��Ϊһ���ո�
    UpdateStr(strBuffer,"  "," ");

    ToLower(strBuffer);

    CmdStr.SplitToCmd(strBuffer," ");

    if (strcmp(CmdStr.m_vCmdStr[0].c_str(),"cpu")==0) 
    {
      stcpuinfo.user=atof(CmdStr.m_vCmdStr[1].c_str());
      stcpuinfo.sys=atof(CmdStr.m_vCmdStr[2].c_str());
      stcpuinfo.wait=atof(CmdStr.m_vCmdStr[3].c_str());
      stcpuinfo.nice=atof(CmdStr.m_vCmdStr[4].c_str());
      stcpuinfo.idle=atof(CmdStr.m_vCmdStr[5].c_str());
      stcpuinfo.irq=atof(CmdStr.m_vCmdStr[6].c_str());
      stcpuinfo.softirq=atof(CmdStr.m_vCmdStr[7].c_str());

      return true;
    }
  }

  logfile.Write("Read /proc/stat failed.\n"); 

  return false;
}



