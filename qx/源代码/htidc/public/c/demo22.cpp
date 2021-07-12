#include "_public.h"

CLogFile logfile;

void *pth1_main(void *arg)
{
  for (int jj=0;jj<200000;jj++)
  {
    logfile.Write("pid=%10d,jj=%10d\n",getpid(),jj);
  }

  pthread_exit(0);
}

int main(int argc,char *argv[])
{
  logfile.Open("/tmp/demo22.log","w",true,true);

  for (int ii=0;ii<50;ii++)
  {
    pthread_t pthid;
    pthread_create(&pthid,NULL,pth1_main,NULL);
  }
  
  sleep(60);

  return 0;
}

