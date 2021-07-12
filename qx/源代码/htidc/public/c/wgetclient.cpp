#include "_public.h"

void EXIT(int sig);

CLogFile       logfile;

int main(int argc, char *argv[])
{
  if(argc!=6)
  {
    printf("Usage:%s weburl tmpfilename outputfilename logfilename charset\n",argv[0]); 
    printf("���������ڻ�ȡWEB��ҳ�����ݡ�\n");
    printf("weburl ��ҳWEB�ĵ�ַ��\n");
    printf("tmpfilename ��ȡ������ҳ�����ݴ�ŵ�ȫ·������ʱ�ļ��������ļ�������utf-8���������롣\n");
    printf("outputfilename ���յ�����ļ�ȫ·���ļ��������ļ���gb18030���룬ע��tmpfilename��ת��Ϊoutputfilename��tmpfilename�ļ����Զ�ɾ����\n");
    printf("logfilename ����������в�������־�ļ�����\n");
    printf("charset ��ҳ���ַ�������utf-8\n\n");
    exit(1);
  }

  // �ر�ȫ�����źź��������
  // �����ź�,��shell״̬�¿��� "kill + ���̺�" ������ֹЩ����
  // ���벻Ҫ�� "kill -9 +���̺�" ǿ����ֹ
  CloseIOAndSignal(); signal(SIGINT,EXIT); signal(SIGTERM,EXIT);

  // ����־�ļ�
  if (logfile.Open(argv[4],"a+") == false)
  {
    printf("logfile.Open(%s) failed.\n",argv[4]); return -1;
  }

  MKDIR(argv[2],true); MKDIR(argv[3],true);

  char strweburl[3001];
  memset(strweburl,0,sizeof(strweburl));

  strncpy(strweburl,argv[1],3000);
 
  char strcmd[3024];
  memset(strcmd,0,sizeof(strcmd));
  snprintf(strcmd,3000,"/usr/bin/wget -c -q -O %s \"%s\" 1>>/dev/null 2>>/dev/null",argv[2],strweburl);
  system(strcmd);
  logfile.Write("%s\n",strcmd);

  char strfilenametmp[301];
  memset(strfilenametmp,0,sizeof(strfilenametmp));
  snprintf(strfilenametmp,300,"%s.tmp",argv[3]);

   // �ѻ�ȡ������ҳת��Ϊ����
  memset(strcmd,0,sizeof(strcmd));
  snprintf(strcmd,256,"iconv -c -f %s -t gb18030 %s -o %s",argv[5],argv[2],strfilenametmp);
  system(strcmd);
  logfile.Write("%s\n",strcmd);

  REMOVE(argv[2]);   // ɾ����ʱ�ļ� 

  RENAME(strfilenametmp,argv[3]);

  return 0;
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

