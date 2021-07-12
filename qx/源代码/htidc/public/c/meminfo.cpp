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
    printf("此程序读取/proc/meminfo文件，把本服务器的内存使用率信息写入xml文件。\n");
    printf("hostname是本服务器的主机名，为了方便识别，也可以用IP。\n");
    printf("logfilename是本程序的日志文件名。\n");
    printf("outputpath是输出的xml文件存放的目录。\n");
    printf("此程序运行在需要监控的服务器上（本程序只适用Linux系统），采集后的xml文件由文件传输程序发送给数据处理服务程序入库。\n\n\n");
 
    return -1;
  }

  // 关闭全部的信号和输入输出
  // 设置信号,在shell状态下可用 "kill + 进程号" 正常终止些进程
  // 但请不要用 "kill -9 +进程号" 强行终止
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

    // 把字符串中间的多个空格全部转换为一个空格
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



