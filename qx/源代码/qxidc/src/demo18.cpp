//
// 本程序演示Cftp客户端类，采用ftp协议从服务器上获取文件
//

#include "_ftp.h"

int main(int argc,char *argv[])
{
  Cftp ftp;

  // 登录远程FTP服务器
  if (ftp.login("192.168.43.168:21","zww","zww1224",FTPLIB_PASSIVE) == false)
  {
    printf("ftp.login() failed.\n"); return -1;
  }
  
  //改变本地目录
  chdir("/tmp");

  // 进入ftp服务器上文件存放的目录
  if (ftp.chdir("/home/zww/qxidc/data/ftp/surfdata") == false)
  {
    printf("ftp.chdir() failed.\n"); return -1;
  }

  // 获取对方目录文件的列表，存放在"/tmp/ftp.list"文件中
  if (ftp.nlist("*.txt","/tmp/ftp.list")== false)
  {
    printf("ftp.nlist() failed.\n"); return -1;
  }

  CFile File;

  File.Open("/tmp/ftp.list","r");

  char strFileName[101];

  // 逐行读取文件的内容，并把文件get到本地
  while (true)
  {
    // 从文件中读取一行
    memset(strFileName,0,sizeof(strFileName));
    if (File.Fgets(strFileName,100) == false) break;

    strFileName[strlen(strFileName) - 1] = 0;

    printf("get %s ...",strFileName);

    // 从远程取文件
    if (ftp.get(strFileName,strFileName)==false)
    {
      printf("ftp.get(%s) failed.\n",strFileName); break;
    }
    
    printf("ok.\n");

    //读取一个文件就删除一个文件
    ftp.ftpdelete(strFileName);

  }

  File.Close();

  ftp.logout();

  return 0;
}

