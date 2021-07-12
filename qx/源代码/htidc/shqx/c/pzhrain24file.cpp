/*
 *  ���������ڴ���ȫ����Сʱ����ʵ��ͼƬ�ļ���
 *  ���ߣ���ũ�е� ���ڣ�20191105
*/
#include "_public.h"
#include "_ooci.h"
#include "_shqx.h"

CLogFile logfile;

CDir Dir;

// ���������ļ�
bool _pzhrain24file(char *strargv2,char *strargv4,char *strargv5);

connection conn;

void EXIT(int sig);

int main(int argc,char *argv[])
{
  if (argc!=7)
  {
    printf("\n���������ڴ���ȫ����Сʱ����ʵ��ͼƬ�ļ���\n\n");
    printf("/htidc/shqx/bin/pzhrain24file logfilename connstr srcpathname dstpathname tname timetvl\n");
    printf("���磺/htidc/shqx/bin/pzhrain24file /log/shqx/pzhrain24file.log shqx/pwdidc@snorcl11g_198 /data/wfile/zhrain24 /qxfile/zhrain24 T_ZHRAIN24 30\n");
    printf("logfilename ���������е���־�ļ�����\n");
    printf("connstr ���ݿ�����Ӳ�����\n");
    printf("srcpathname ԭʼ�ļ���ŵ�Ŀ¼���ļ�������PWCP_TWC_WEAP_SFER_ER1_TWC_L88_P9_20191101070000000.JPG��\n");
    printf("dstpathname Ŀ���ļ���ŵ�Ŀ¼���ļ���yyyy/mm/dd��֯Ŀ¼��������Ϊzhrain24_yyyymmddhh24miss.jpg��\n");
    printf("tname ���ݴ�ŵı�����\n");
    printf("timetvl ���������е�ʱ��������λ���롣\n");
    return -1;
  }

  // �ر�ȫ�����źź��������
  CloseIOAndSignal();

  // ��������˳����ź�
  signal(SIGINT,EXIT); signal(SIGTERM,EXIT);

  if (logfile.Open(argv[1],"a+")==false)
  {
    printf("����־�ļ�ʧ�ܣ�%s����\n",argv[1]); return -1;
  }

  logfile.Write("����������\n");

  while (true)
  {
    // logfile.Write("��ʼɨ��Ŀ¼��\n");

    // ɨ�������ļ���ŵ�Ŀ¼��ֻƥ��"PWCP_TWC_WEAP_SFER_ER1_TWC_L88_P9_20*.JPG"
    if (Dir.OpenDir(argv[3],"PWCP_TWC_WEAP_SFER_ER1_TWC_L88_P9_20*.JPG",1000,true,true)==false)
    {
      logfile.Write("Dir.OpenDir(%s) failed.\n",argv[3]); sleep(atoi(argv[6])); continue;
    }

    // �������Ŀ¼�е������ļ�
    while (true)
    {
      if (Dir.ReadDir()==false) break;
  
      // ���������ļ�
      if (_pzhrain24file(argv[2],argv[4],argv[5])==false) 
      {
        logfile.WriteEx("ʧ�ܡ�\n"); continue;
      }
    }

    // �Ͽ������ݿ������
    if (conn.m_state==1) conn.disconnect(); 

    sleep(atoi(argv[6]));
  }

  return 0;
}

void EXIT(int sig)
{
  logfile.Write("�����˳���sig=%d\n\n",sig);

  exit(0);
}
     
// ���������ļ�
bool _pzhrain24file(char *strargv2,char *strargv4,char *strargv5)
{
  char strddatetime[21];   // �ļ�������ʱ�䣬��ʽyyyymmddhh24miss
  memset(strddatetime,0,sizeof(strddatetime));
  strncpy(strddatetime,strstr(Dir.m_FileName,"20"),14);

  char strdstfilename[301];  // Ŀ���ļ���������·��
  memset(strdstfilename,0,sizeof(strdstfilename));
  snprintf(strdstfilename,300,"zhrain24_%s.jpg",strddatetime);
  
  char strdstfilepath[301];  // Ŀ���ļ���ŵ�Ŀ¼
  memset(strdstfilepath,0,sizeof(strdstfilepath));
  snprintf(strdstfilepath,300,"%s/",strargv4);
  strncat(strdstfilepath,strddatetime,4);     strcat(strdstfilepath,"/");  // �����Ŀ¼
  strncat(strdstfilepath,strddatetime+4,2);   strcat(strdstfilepath,"/");  // �µ���Ŀ¼
  strncat(strdstfilepath,strddatetime+6,2);   strcat(strdstfilepath,"/");  // �յ���Ŀ¼

  char strfulldstfilename[301]; // Ŀ���ļ�����ȫ·��
  memset(strfulldstfilename,0,sizeof(strfulldstfilename));
  snprintf(strfulldstfilename,300,"%s%s",strdstfilepath,strdstfilename);

  // ����ļ��Ѵ���(Ŀ���ļ��Ѵ���)��ֱ�ӷ��سɹ���
  if (access(strfulldstfilename,F_OK) == 0) return true;

  if (conn.m_state==0)
  {
    if (conn.connecttodb(strargv2,"Simplified Chinese_China.ZHS16GBK")!=0)
    {
      logfile.Write("connect database(%s) failed.\n%s\n",strargv2,conn.m_cda.message); return false;
    }
    // logfile.Write("�������ݿ�ɹ���\n");
  }
  
  // ��Դ�ļ����Ƶ�Ŀ���ļ�
  if (COPY(Dir.m_FullFileName,strfulldstfilename)==false) 
  {
    logfile.Write("�����ļ�COPY(%s,%s)...failed.\n",Dir.m_FullFileName,strfulldstfilename); return false;
  }

  // �ѷǽṹ�������ļ�д��oracle���ݿ�ı���
  if (FileToTable(&conn,&logfile,strargv5,strfulldstfilename,strddatetime)!=0)
  {
    logfile.Write("���ļ�%s����%s...failed.\n",strfulldstfilename,strargv5);  return false;
  }

  logfile.Write("���ļ�%s����%s...ok.\n",strfulldstfilename,strargv5);  

  return true;
}

