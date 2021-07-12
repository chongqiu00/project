#include "_public.h"
#include "_ftp.h"

int main()
{
  Cftp ftp;

  // 登录ftp服务器
  if (ftp.login("172.16.0.15:21","test","test1234TEST")==false)
  {
    printf("ftp.login() failed.\n"); return -1;
  }

  // 获取文件
  if (ftp.get("/home/test/data/20191210122500.txt","/tmp/data/dat20191210122500.txt")==false)
  {
    printf("ftp.get() failed.\n"); return -1;
  }

  printf("get /home/test/data/20191210122500.txt ok.\n");

  return 0;
}
