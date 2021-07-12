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
    printf("�˳������df�������ѱ��������Ĵ���ʹ������Ϣд��xml�ļ���\n");
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

    // ���û���ҵ���%�������ٶ�ȡһ�У���strBufferƴ����
    if (strstr(strBuffer,"%") == 0)
    {
      memset(strLine,0,sizeof(strLine));
      if (FGETS(fp,strLine,500) == false) break;
      strcat(strBuffer," "); strcat(strBuffer,strLine);
    }

    // ɾ���ַ���ǰ��Ŀո�ͻ��з�
    DeleteLRChar(strBuffer,' '); DeleteLRChar(strBuffer,'\n');

    // ���ַ����м�Ķ���ո�ȫ��ת��Ϊһ���ո�
    UpdateStr(strBuffer,"  "," ");

    // ��ȫ����ȫ��ת��ΪСд
    ToLower(strBuffer);

    // ���˴�����Ϣ�����п������ڴ棬SMB�������ļ�����������
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


