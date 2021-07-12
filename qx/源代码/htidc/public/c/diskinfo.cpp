#include "_public.h"

void EXIT(int sig);

CLogFile logfile;

int main(int argc,char *argv[])
{
  if (argc != 4)
  {
    printf("\n");
    printf("Using:./diskinfo hostname logfilename outputpath\n");

    printf("Example:/htidc/public/bin/diskinfo 118.89.50.198 /tmp/htidc/log/diskinfo.log /tmp/htidc/monclient\n\n");
    printf("此程序调用df命名，把本服务器的磁盘使用率信息写入xml文件。\n");
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

  FILE *fp=0;

  if ( (fp=popen("df -k --block-size=1M","r")) == NULL )
  {
    logfile.Write("popen(df -k --block-size=1M) failed.\n"); return false;
  }

  char strXMLFileName[301],strLocalTime[21];
  memset(strXMLFileName,0,sizeof(strXMLFileName));
  memset(strLocalTime,0,sizeof(strLocalTime));
  LocalTime(strLocalTime,"yyyymmddhh24miss");
  snprintf(strXMLFileName,300,"%s/diskinfo_%s_%s.xml",argv[3],strLocalTime,argv[1]);

  CFile XMLFile;
  if (XMLFile.OpenForRename(strXMLFileName,"w+") == false )
  {
    logfile.Write("XMLFile.OpenForRename(%s) failed.\n",strXMLFileName); pclose(fp); return -1;
  }

  XMLFile.Fprintf("<data>\n");

  CCmdStr CmdStr;
  char strBuffer[1024],strLine[500];

  while (true)
  {
    memset(strBuffer,0,sizeof(strBuffer));

    if (FGETS(fp,strBuffer,500) == false) break;

    // 如果没有找到“%”，就再读取一行，与strBuffer拼起来
    if (strstr(strBuffer,"%") == 0)
    {
      memset(strLine,0,sizeof(strLine));
      if (FGETS(fp,strLine,500) == false) break;
      strcat(strBuffer," "); strcat(strBuffer,strLine);
    }

    // 删除字符串前后的空格和换行符
    DeleteLRChar(strBuffer,' '); DeleteLRChar(strBuffer,'\n');

    // 把字符串中间的多个空格全部转换为一个空格
    UpdateStr(strBuffer,"  "," ");

    // 把全内容全部转换为小写
    ToLower(strBuffer);

    // 除了磁盘信息，还有可能是内存，SMB等其它文件，都丢弃掉
    if (strncmp(strBuffer,"/dev",4) != 0) continue;

    CmdStr.SplitToCmd(strBuffer," ");

    if (CmdStr.CmdCount() != 6) continue;

    char strusep[21];
    memset(strusep,0,sizeof(strusep));
    strcpy(strusep,CmdStr.m_vCmdStr[4].c_str());
    UpdateStr(strusep,"%","");

    char strLocalTime[21];
    memset(strLocalTime,0,sizeof(strLocalTime));
    LocalTime(strLocalTime,"yyyymmddhh24miss");
    XMLFile.Fprintf(\
            "<nodip>%s</nodip>"\
            "<crttime>%s</crttime>"\
            "<filesystem>%s</filesystem>"\
            "<total>%0.02f</total>"\
            "<used>%0.02f</used>"\
            "<available>%0.02f</available>"\
            "<usep>%0.02f</usep>"\
            "<mount>%s</mount><endl/>\n",
             argv[1],
             strLocalTime,
             CmdStr.m_vCmdStr[0].c_str(),
             atof(CmdStr.m_vCmdStr[1].c_str())/1024.0,
             atof(CmdStr.m_vCmdStr[2].c_str())/1024.0,
             atof(CmdStr.m_vCmdStr[3].c_str())/1024.0,
             (atof(CmdStr.m_vCmdStr[2].c_str())/atof(CmdStr.m_vCmdStr[1].c_str()))*100.0,
             CmdStr.m_vCmdStr[5].c_str());
  }

  XMLFile.Fprintf("</data>\n");

  pclose(fp);

  XMLFile.CloseAndRename();

  logfile.Write("create %s ok.\n",strXMLFileName);

  exit(0);
}

void EXIT(int sig)
{
  if (sig > 0) signal(sig,SIG_IGN);

  logfile.Write("catching the signal(%d).\n",sig);

  logfile.Write("diskinfo exit.\n");

  exit(0);
}


