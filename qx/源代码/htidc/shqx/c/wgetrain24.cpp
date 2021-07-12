#include "_public.h"

void EXIT(int sig);

CLogFile       logfile;

bool GetURL(char *strBuffer,char *strURL,char *strFileName);

int main(int argc, char *argv[])
{
  if(argc!=4)
  {
    printf("Usage:%s logfilename tmpfilename outputfilename\n",argv[0]); 
    printf("Sample:./wgetrain24 /log/shqx/wgetrain24.log /data/wgettmp /data/wfile/zhrain24\n\n");
    printf("���������ڴ��й���������ȡ��Сʱ������ʵ��ͼ��\n");
    printf("�й���������url��http://products.weather.com.cn/product/Index/index/procode/JC_JSL_ZH.shtml\n");
    printf("����й���������url�ı䣬����Ҳ�����Ķ���\n");
    printf("logfilename ����������в�������־�ļ�����\n");
    printf("tmpfilename ���������в�������ʱ�ļ���ŵ�Ŀ¼��\n");
    printf("��ȡ��Сʱ������ʵ��ͼ��ŵ�Ŀ¼��\n\n");
    exit(1);
  }

  // �ر�ȫ�����źź��������
  // �����ź�,��shell״̬�¿��� "kill + ���̺�" ������ֹЩ����
  // ���벻Ҫ�� "kill -9 +���̺�" ǿ����ֹ
  CloseIOAndSignal(); signal(SIGINT,EXIT); signal(SIGTERM,EXIT);

  // ����־�ļ�
  if (logfile.Open(argv[1],"a+") == false)
  {
    printf("logfile.Open(%s) failed.\n",argv[1]); return -1;
  }

  MKDIR(argv[2],false); MKDIR(argv[3],false);

  while (true)
  {
    // ����wgetclient��ȡ��ҳ����
    char strwgetclient[2001];
    memset(strwgetclient,0,sizeof(strwgetclient));
    snprintf(strwgetclient,2000,"/htidc/public/bin/wgetclient \"http://products.weather.com.cn/product/Index/index/procode/JC_JSL_ZH.shtml\" %s/wgetclient_%d.tmp  %s/wgetclient_%d.html %s/wgetclient.log utf-8",argv[2],getpid(),argv[2],getpid(),argv[2]);
    system(strwgetclient);
    // logfile.Write("%s\n",strwgetclient);
  
    // ����ҳ�����ļ�
    char stroutputfile[301];
    memset(stroutputfile,0,sizeof(stroutputfile));
    snprintf(stroutputfile,300,"%s/wgetclient_%d.html",argv[2],getpid());
    CFile File;
    if (File.Open(stroutputfile,"r")==false)
    {
      logfile.Write("File.Open(%s) failed.\n",stroutputfile); sleep(60); continue;
    }
    
    char strBuffer[1001],strURL[501],strFullFileName[301],strFileName[101];
  
    // �õ�ȫ����ͼƬ�ļ���
    while (true)
    {
      memset(strBuffer,0,sizeof(strBuffer));
      memset(strURL,0,sizeof(strURL));
      memset(strFullFileName,0,sizeof(strFullFileName));
      memset(strFileName,0,sizeof(strFileName));
  
      if (File.Fgets(strBuffer,1000)==false) break;
  
      if (MatchFileName(strBuffer,"*PWCP_TWC_WEAP_SFER_ER1_TWC_L88_P9_20*.JPG*")==false) continue;
  
      // logfile.Write("%s",strBuffer);
  
      // ������url���ļ���
      if (GetURL(strBuffer,strURL,strFileName)==false) continue;
  
      // �ļ��Ѵ��ڣ����ɼ�
      snprintf(strFullFileName,300,"%s/%s",argv[3],strFileName);
      if (access(strFullFileName,F_OK)==0) continue;
  
      // ����wget��ȡ�ļ�
      logfile.Write("download %s ",strFileName);
      memset(strwgetclient,0,sizeof(strwgetclient));
      snprintf(strwgetclient,500,"wget \"%s\" -o %s/wgetrain24.log -O %s",strURL,argv[2],strFullFileName);
      system(strwgetclient);
  
      if (access(strFullFileName,F_OK)==0) logfile.WriteEx("ok.\n");
      else logfile.WriteEx("failed.\n");
    }
  
    File.CloseAndRemove();

    sleep(60);
  }

  return 0;
}

bool GetURL(char *strBuffer,char *strURL,char *strFileName)
{
  char *start,*end;
  start=end=0;

  if ((start=strstr(strBuffer,"http"))==0) return false;
  if ((end=strstr(start,"\""))==0) return false;

  strncpy(strURL,start,end-start);

  strcpy(strFileName,strstr(strURL,"PWCP"));

  return true;
}

void EXIT(int sig)
{
  if (sig > 0) signal(sig,SIG_IGN);

  logfile.Write("catching the signal(%d).\n",sig);

  logfile.Write("wgetclient exit.\n");

  exit(0);
}

/*
������ͨ����ĸ�����֣����ģ����������ַ������ǹ淶��ʹ��Ӧ����ʹ���ַ�ת�塣
ʮ������ֵ 
1. +  URL ��+�ű�ʾ�ո� %2B 
2. �ո� URL�еĿո������+�Ż��߱��� %20 
3. /  �ָ�Ŀ¼����Ŀ¼ %2F  
4. ?  �ָ�ʵ�ʵ� URL �Ͳ��� %3F  
5. % ָ�������ַ� %25  
6. # ��ʾ��ǩ %23  
7. & URL ��ָ���Ĳ�����ķָ��� %26  
8. = URL ��ָ��������ֵ %3D 
*/

