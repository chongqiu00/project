#include "_public.h"

CLogFile logfile;

int main(int argc,char *argv[])
{
  logfile.Open("/tmp/demo21.log","w",false,true);

  for (int ii=0;ii<50;ii++)
  {
    if (fork()>0) continue;

    for (int jj=0;jj<200000;jj++)
    {
      logfile.Write("pid=%10d,jj=%10d\n",getpid(),jj);
    }

    exit(0);
  }
  
  return 0;
}

