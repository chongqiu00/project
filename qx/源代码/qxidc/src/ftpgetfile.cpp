//通过ftp向服务器下载和传输文件

#include "_public.h"
#include "_ftp.h"

struct st_arg
{
 char host[51];         //要连接的ip和端口
 char mode[11];         //传输模式（主动，被动）
 char username[31];     //连接服务器上的哪个用户
 char password[31];     //用户密码
 char localpath[301];   //下载到本地的目录
 char remotepath[301];  //连接远程服务器的目录
 char matchname[301];   //
 int ptype;               //远程服务器文件处理方式
 char remotepathbak[301]; //ptype==3时，备份文件的远程服务器目录
 char listfilename[301];  //下载清单
 char okfilename[301];    //下载完毕清单
} starg;

//ftp操作类
Cftp ftp;

//日志文件
CLogFile logfile;

//将nlist方法获取到的List文件加载到vlistfile容器中
struct st_fileinfo
{
  char filename[301];   //文件名
  char mtime[21];       //文件创建时间
};
vector<st_fileinfo> vlistfile, vlistfile1;
vector<st_fileinfo> vokfilename, vokfilename1;

//把okfilename文件内容加载到vokfilename容器中
bool LoadOKFileName();

//加载文件名到容器中
bool LoadListFile();

//比较两个容器
bool CompVector();

//把vokfilename1容器中的内容先写入okfilename文件中，覆盖之前旧的okfilename文件
bool WriteToFileName();

//如果ptype==1，把采集成功的文件记录追加到okfilename文件中
bool AppendToOKFileName(struct st_fileinfo* stfileinfo);

//程序安全退出
void EXIT(int sig);

//本程序的业务流程主函数
bool _ftpgetfile();

int main (int argc, char* argv[])
{
 
  if ( argc != 3 )
  {
    printf("\n");
    printf("Using:/home/zww/qxidc/src/ftpgetfile logfilename xmlbuffer\n\n");	 
    printf("Sample:/home/zww/qxidc/src/ftpgetfile /home/zww/qxidc/log/ftpgetfile_surfdata.log \"<host>192.168.43.168:21</host><mode>pasv</mode><username>zww</username><password>zww1224</password><localpath>/home/zww/qxidc/data/surfdata</localpath><remotepath>/home/zww/qxidc/data/ftp/surfdata</remotepath><matchname>SURF_*.TXT</matchname><ptype>1</ptype><remotepathbak>/home/zww/qxidc/data/ftp/surfdatabak</remotepathbak><listfilename>/home/zww/qxidc/list/ftpgetfile_surfdata.list</listfilename><okfilename>/home/zww/qxidc/list/ftpgetfile_surfdata.xml</okfilename>\"\n\n\n");
    
    printf("本程序是数据中心的公共功能模块，用于把远程FTP服务器的文件采集到本地目录。\n");
    printf("logfilename是本程序运行的日志文件。\n");
    printf("xmlbuffer为文件传输的参数，如下：\n");
    printf("<host>192.168.43.168:21</host> 远程服务器的IP和端口。\n");
    printf("<mode>pasv</mode> 传输模式， passv和port,可选字段，缺省采用passv模式。\n");
    printf("<username>zww</username> 远程服务器的用户名。\n");
    printf("<password>zww1224</password> 运程服务器FTP的密码。\n");
    printf("<localpath>/home/zww/qxidc/data/surfdata</localpath> 本地文件存放的目录。\n");
    printf("<remotepath>/home/zww/qxidc/data/ftp/surfdata</remotepath> 远程服务器存放文件的目录。\n");
    printf("<mathchname>*.GIF</mathchname> 待采集文件匹配的文件名，采用大写匹配。"\
    "不匹配的文件不会被采集，本字段尽可能设置精确，不允许用*匹配全部的文件。\n");
    printf("<ptype>2</ptype> 文件采集成功后，远程服务器文件的处理方式：1-什么也不做；2-删除；3-备份，如果为3，还要指定备份的目录。\n");
    printf("<remotepathbak>/home/zww/qxidc/data/ftp/surfdatabak</remotepathbak> 文件采集成功后，服务器文件的备份目录，此参数只有当ptype=3时才有效。\n");
    printf("<listfilename>/home/zww/qxidc/list/ftpgetfile_surfdata.list</listfilename> 采集前列出服务器文件名的文件。\n");
    printf("<okfilename>/home/zww/qxidc/list/ftpgetfile_surfdata.xml</okfilename> 已采集成功文件名清单。\n");
    
    return -1;
  }

  //忽略信号和输入输出
  CloseIOAndSignal();

  //处理程序退出的信号
  signal(SIGINT, EXIT);  signal(SIGTERM, EXIT);

  if (logfile.Open(argv[1], "a+")  == false)
  {
    printf("打开日志文件失败（%s）。\n", argv[1]);  return -1;
  }

  memset(&starg, 0, sizeof(struct st_arg));
  GetXMLBuffer(argv[2], "host", starg.host);
  GetXMLBuffer(argv[2], "mode", starg.mode);
  GetXMLBuffer(argv[2], "username", starg.username);
  GetXMLBuffer(argv[2], "password", starg.password);
  GetXMLBuffer(argv[2], "localpath", starg.localpath);
  GetXMLBuffer(argv[2], "remotepath", starg.remotepath);
  GetXMLBuffer(argv[2], "matchname", starg.matchname);
  GetXMLBuffer(argv[2], "ptype", &starg.ptype);
  GetXMLBuffer(argv[2], "remotepathbak", starg.remotepathbak);
  GetXMLBuffer(argv[2], "listfilename", starg.listfilename);
  GetXMLBuffer(argv[2], "okfilename", starg.okfilename);
 
  int mode = FTPLIB_PASSIVE;
  if (strcmp(starg.mode, "port") == 0) mode = FTPLIB_PORT;

  if (ftp.login(starg.host, starg.username, starg.password, mode) == false)
  {
    logfile.Write("ftp.login(%s,%s,%s) failed.\n", starg.host, starg.username, starg.password);  return -1;
  }

  logfile.Write("ftp.login ok.\n");

 //本程序的业务流程主函数。
 _ftpgetfile();

 return 0;
}


//程序安全退出
void EXIT(int sig)
{
  logfile.Write("程序退出，sig=%d\n", sig);
  exit(0);
}

//本程序的业务流程主函数
bool _ftpgetfile()
{
  //获取服务器文件列表
  if (ftp.chdir(starg.remotepath) == false)
  {
    logfile.Write("ftp.chdir(%s) failed.\n", starg.remotepath); return false;
  }

  logfile.Write("chdir ok.\n");

  //列出服务器目录文件
  if (ftp.nlist(".", starg.listfilename) == false)
  {
    logfile.Write("ftp.nlist(%s) failed.\n", starg.remotepath);  return false;
  }

  logfile.Write("nlist ok.\n");
  
  //将nlist方法获取到的List文件加载到vlistfile容器中（文件名，文件创建时间）
  if ( LoadListFile() == false )  
  {
    logfile.Write("LoadListFile failed.\n"); return false;
  }

  if (starg.ptype==1)
  {
    //okfilename:<filename>SURF_ZH_20210401104249_13285.txt</filename>
    //加载okfilename文件中的内容到容器vokfilename中
    LoadOKFileName();
    //把vlistfile容器中的文件与容器vokfile中文件对比，得到两个容器
    //一，在vlistfile中存在，并已经采集成功的文件vokfilenae1
    //二，在vlistfile中存在，新文件需要重新采集的文件vlistfile1
    CompVector();
    //把vokfilename1容器中的内容先写入okfilename文件中，覆盖之前的旧okfilename文件
    WriteVector();
    //把vlistfile1容器中的内容复制到vlistfile容器中
  }

  char strremotefilename[301], strlocalfilename[301];

  for (int ii=0; ii<vlistfile.size(); ii++)
  {
    SNPRINTF(strlocalfilename,300, "%s/%s", starg.localpath, vlistfile[ii].filename);
    SNPRINTF(strremotefilename,300, "%s/%s", starg.remotepath, vlistfile[ii].filename);
    logfile.Write("get %s ...", strremotefilename);

    //获取文件
    if (ftp.get(strremotefilename, strlocalfilename, true) == false)  
    {
      logfile.WriteEx("failed.\n");  break;
    }
    
    logfile.WriteEx("ok.\n");

    //删除文件
    if (starg.ptype==2) ftp.ftpdelete(strremotefilename);

    //转存到备份目录
    if (starg.ptype == 3)
    {
      char strremotefilenamebak[301];
      SNPRINTF(strremotefilenamebak, 300, "%s/%s", starg.remotepathbak, vlistfile[ii].filename);
      ftp.ftprename(strremotefilename, strremotefilenamebak);
    }
 

  }
  
  return true;
}

bool LoadListFile()
{
  CFile File;
  vlistfile.clear();

  if (File.Open(starg.listfilename, "r") == false)
  {
    logfile.Write("File.Open(%s) failed.\n", starg.listfilename);  return false;
  }

  struct st_fileinfo strfileinfo;

  while (true)
  {
    memset(&strfileinfo, 0, sizeof(struct st_fileinfo));

    if (File.Fgets(strfileinfo.filename, 300, true) == false) break;
    
    if (starg.ptype == 1)
    {
      //获取对方服务器文件时间
      if (ftp.mtime(strfileinfo.filename) == false) 
      {
        logfile.Write("ftp.mtime(%s) failed.\n", strfileinfo.filename);  return false;
      }

      strcpy(strfileinfo.mtime, ftp.m_mtime);
    }
    vlistfile.push_back(strfileinfo);
  }
  return true;
}


//把okfilename文件内容加载到vokfilenae容器中
bool LoadOKFileName()
{
  CFile File;

  if (File.Open(starg.okfilename, "r") == false)  return true;

  struct st_fileinfo strfileinfo;

  char strbuffer[301];

  while (true)
  {
    memset(&stfileinfo, 0, sizeof(struct st_fileinfo));

    if (File.Fgets(strbuffer, 300, true) == false) break;

    GetXMLBuffer(strbuffer, "filename", stfileinfo.filename, 300);
    GetXMLBuffer(strbuffer, "mtime", stfileinfo.mtime, 20);

    vokfilename.push_back(strfileinfo);

    logfile.Write("vokfilename filename=%s,mtime=%s\n", stfileinfo.filename, stfileinfo.mtime);
   }
  
  return true;
}


//把vlistfile容器中的文件与容器vokfilename中文件对比，得到两个容器
//一，在vlistfile中存在，并已经采集成功的文件vokfilename1
//二，在vlistfile中存在，新文件需要重新采集的文件vlistfile1
bool CompVector()
{
  vokfilename1.clear(); vlistfile.clear();

  for (int ii=0; vlistfile.size(); ii++)
  {
    for (int jj=0; vokfilename.size(); jj++)
    {
      if ( (strcmp(vlistfile[ii].filename, vokfilename[jj].filename) == 0 ) &&
           (strcmp(vlistfile[ii].mtime, vokfilename[jj].mtime) == 0)  )
	   {
	     vokfilename1.push_back(vlistfile[ii]); break;
	   }
    }
  }

  if ( jj == vokfilename.size())
  {
    vlistfile1.push_back(vlistfile[ii]);
  }

  for (int ii=0; ii<vokfilename1.size(); ii++)
  {
    logfile.Write("vokfilename1 filename=%s,mtime=%s\n", vokfilename1[ii].filename, vokfilename1[ii].mtime); 
  }

  for (int ii=0; ii<vlistfile1.size(); ii++)
  {
    logfile.Write("vlistfile1 filename=%s,mtime=%s\n", vlistfile1[ii].filename, vlistfile1[ii].mtime); 
  }
 return true;
}
