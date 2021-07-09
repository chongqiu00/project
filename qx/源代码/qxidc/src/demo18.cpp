//
// ��������ʾCftp�ͻ����࣬����ftpЭ��ӷ������ϻ�ȡ�ļ�
//

#include "_ftp.h"

int main(int argc,char *argv[])
{
  Cftp ftp;

  // ��¼Զ��FTP������
  if (ftp.login("192.168.43.168:21","zww","zww1224",FTPLIB_PASSIVE) == false)
  {
    printf("ftp.login() failed.\n"); return -1;
  }
  
  //�ı䱾��Ŀ¼
  chdir("/tmp");

  // ����ftp���������ļ���ŵ�Ŀ¼
  if (ftp.chdir("/home/zww/qxidc/data/ftp/surfdata") == false)
  {
    printf("ftp.chdir() failed.\n"); return -1;
  }

  // ��ȡ�Է�Ŀ¼�ļ����б������"/tmp/ftp.list"�ļ���
  if (ftp.nlist("*.txt","/tmp/ftp.list")== false)
  {
    printf("ftp.nlist() failed.\n"); return -1;
  }

  CFile File;

  File.Open("/tmp/ftp.list","r");

  char strFileName[101];

  // ���ж�ȡ�ļ������ݣ������ļ�get������
  while (true)
  {
    // ���ļ��ж�ȡһ��
    memset(strFileName,0,sizeof(strFileName));
    if (File.Fgets(strFileName,100) == false) break;

    strFileName[strlen(strFileName) - 1] = 0;

    printf("get %s ...",strFileName);

    // ��Զ��ȡ�ļ�
    if (ftp.get(strFileName,strFileName)==false)
    {
      printf("ftp.get(%s) failed.\n",strFileName); break;
    }
    
    printf("ok.\n");

    //��ȡһ���ļ���ɾ��һ���ļ�
    ftp.ftpdelete(strFileName);

  }

  File.Close();

  ftp.logout();

  return 0;
}

