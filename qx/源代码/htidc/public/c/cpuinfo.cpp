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

    printf("此程序读取/proc/stat文件，把本服务器的CPU使用率信息写入xml文件。\n");
    printf("hostname是本服务器的主机名，为了方便识别，也可以用IP。\n");
    printf("logfilename是本程序的日志文件名。\n");
    printf("outputpath是输出的xml文件存放的目录。\n");
    printf("此程序运行在需要监控的服务器上（本程序只适用Linux系统），采集后的xml文件由文件传输程序发送给数据处理服务程序入库。\n\n\n");
 
    return -1;
  }

  //memset(strHostName,0,sizeof(strHostName));
  //strncpy(strHostName,argv[2],20);

  // 关闭全部的信号和输入输出
  // 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程
  // 但请不要用 "kill -9 +进程号" 强行终止
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

    // 删除字符串前后的空格
    DeleteLRChar(strBuffer,' ');

    // 把字符串中间的多个空格全部转换为一个空格
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



