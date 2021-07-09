//ͨ��ftp����������غʹ����ļ�

#include "_public.h"
#include "_ftp.h"

struct st_arg
{
 char host[51];         //Ҫ���ӵ�ip�Ͷ˿�
 char mode[11];         //����ģʽ��������������
 char username[31];     //���ӷ������ϵ��ĸ��û�
 char password[31];     //�û�����
 char localpath[301];   //���ص����ص�Ŀ¼
 char remotepath[301];  //����Զ�̷�������Ŀ¼
 char matchname[301];   //
 int ptype;               //Զ�̷������ļ�����ʽ
 char remotepathbak[301]; //ptype==3ʱ�������ļ���Զ�̷�����Ŀ¼
 char listfilename[301];  //�����嵥
 char okfilename[301];    //��������嵥
} starg;

//ftp������
Cftp ftp;

//��־�ļ�
CLogFile logfile;

//��nlist������ȡ����List�ļ����ص�vlistfile������
struct st_fileinfo
{
  char filename[301];   //�ļ���
  char mtime[21];       //�ļ�����ʱ��
};
vector<st_fileinfo> vlistfile, vlistfile1;
vector<st_fileinfo> vokfilename, vokfilename1;

//��okfilename�ļ����ݼ��ص�vokfilename������
bool LoadOKFileName();

//�����ļ�����������
bool LoadListFile();

//�Ƚ���������
bool CompVector();

//��vokfilename1�����е�������д��okfilename�ļ��У�����֮ǰ�ɵ�okfilename�ļ�
bool WriteToFileName();

//���ptype==1���Ѳɼ��ɹ����ļ���¼׷�ӵ�okfilename�ļ���
bool AppendToOKFileName(struct st_fileinfo* stfileinfo);

//����ȫ�˳�
void EXIT(int sig);

//�������ҵ������������
bool _ftpgetfile();

int main (int argc, char* argv[])
{
 
  if ( argc != 3 )
  {
    printf("\n");
    printf("Using:/home/zww/qxidc/src/ftpgetfile logfilename xmlbuffer\n\n");	 
    printf("Sample:/home/zww/qxidc/src/ftpgetfile /home/zww/qxidc/log/ftpgetfile_surfdata.log \"<host>192.168.43.168:21</host><mode>pasv</mode><username>zww</username><password>zww1224</password><localpath>/home/zww/qxidc/data/surfdata</localpath><remotepath>/home/zww/qxidc/data/ftp/surfdata</remotepath><matchname>SURF_*.TXT</matchname><ptype>1</ptype><remotepathbak>/home/zww/qxidc/data/ftp/surfdatabak</remotepathbak><listfilename>/home/zww/qxidc/list/ftpgetfile_surfdata.list</listfilename><okfilename>/home/zww/qxidc/list/ftpgetfile_surfdata.xml</okfilename>\"\n\n\n");
    
    printf("���������������ĵĹ�������ģ�飬���ڰ�Զ��FTP���������ļ��ɼ�������Ŀ¼��\n");
    printf("logfilename�Ǳ��������е���־�ļ���\n");
    printf("xmlbufferΪ�ļ�����Ĳ��������£�\n");
    printf("<host>192.168.43.168:21</host> Զ�̷�������IP�Ͷ˿ڡ�\n");
    printf("<mode>pasv</mode> ����ģʽ�� passv��port,��ѡ�ֶΣ�ȱʡ����passvģʽ��\n");
    printf("<username>zww</username> Զ�̷��������û�����\n");
    printf("<password>zww1224</password> �˳̷�����FTP�����롣\n");
    printf("<localpath>/home/zww/qxidc/data/surfdata</localpath> �����ļ���ŵ�Ŀ¼��\n");
    printf("<remotepath>/home/zww/qxidc/data/ftp/surfdata</remotepath> Զ�̷���������ļ���Ŀ¼��\n");
    printf("<mathchname>*.GIF</mathchname> ���ɼ��ļ�ƥ����ļ��������ô�дƥ�䡣"\
    "��ƥ����ļ����ᱻ�ɼ������ֶξ��������þ�ȷ����������*ƥ��ȫ�����ļ���\n");
    printf("<ptype>2</ptype> �ļ��ɼ��ɹ���Զ�̷������ļ��Ĵ���ʽ��1-ʲôҲ������2-ɾ����3-���ݣ����Ϊ3����Ҫָ�����ݵ�Ŀ¼��\n");
    printf("<remotepathbak>/home/zww/qxidc/data/ftp/surfdatabak</remotepathbak> �ļ��ɼ��ɹ��󣬷������ļ��ı���Ŀ¼���˲���ֻ�е�ptype=3ʱ����Ч��\n");
    printf("<listfilename>/home/zww/qxidc/list/ftpgetfile_surfdata.list</listfilename> �ɼ�ǰ�г��������ļ������ļ���\n");
    printf("<okfilename>/home/zww/qxidc/list/ftpgetfile_surfdata.xml</okfilename> �Ѳɼ��ɹ��ļ����嵥��\n");
    
    return -1;
  }

  //�����źź��������
  CloseIOAndSignal();

  //��������˳����ź�
  signal(SIGINT, EXIT);  signal(SIGTERM, EXIT);

  if (logfile.Open(argv[1], "a+")  == false)
  {
    printf("����־�ļ�ʧ�ܣ�%s����\n", argv[1]);  return -1;
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

 //�������ҵ��������������
 _ftpgetfile();

 return 0;
}


//����ȫ�˳�
void EXIT(int sig)
{
  logfile.Write("�����˳���sig=%d\n", sig);
  exit(0);
}

//�������ҵ������������
bool _ftpgetfile()
{
  //��ȡ�������ļ��б�
  if (ftp.chdir(starg.remotepath) == false)
  {
    logfile.Write("ftp.chdir(%s) failed.\n", starg.remotepath); return false;
  }

  logfile.Write("chdir ok.\n");

  //�г�������Ŀ¼�ļ�
  if (ftp.nlist(".", starg.listfilename) == false)
  {
    logfile.Write("ftp.nlist(%s) failed.\n", starg.remotepath);  return false;
  }

  logfile.Write("nlist ok.\n");
  
  //��nlist������ȡ����List�ļ����ص�vlistfile�����У��ļ������ļ�����ʱ�䣩
  if ( LoadListFile() == false )  
  {
    logfile.Write("LoadListFile failed.\n"); return false;
  }

  if (starg.ptype==1)
  {
    //okfilename:<filename>SURF_ZH_20210401104249_13285.txt</filename>
    //����okfilename�ļ��е����ݵ�����vokfilename��
    LoadOKFileName();
    //��vlistfile�����е��ļ�������vokfile���ļ��Աȣ��õ���������
    //һ����vlistfile�д��ڣ����Ѿ��ɼ��ɹ����ļ�vokfilenae1
    //������vlistfile�д��ڣ����ļ���Ҫ���²ɼ����ļ�vlistfile1
    CompVector();
    //��vokfilename1�����е�������д��okfilename�ļ��У�����֮ǰ�ľ�okfilename�ļ�
    WriteVector();
    //��vlistfile1�����е����ݸ��Ƶ�vlistfile������
  }

  char strremotefilename[301], strlocalfilename[301];

  for (int ii=0; ii<vlistfile.size(); ii++)
  {
    SNPRINTF(strlocalfilename,300, "%s/%s", starg.localpath, vlistfile[ii].filename);
    SNPRINTF(strremotefilename,300, "%s/%s", starg.remotepath, vlistfile[ii].filename);
    logfile.Write("get %s ...", strremotefilename);

    //��ȡ�ļ�
    if (ftp.get(strremotefilename, strlocalfilename, true) == false)  
    {
      logfile.WriteEx("failed.\n");  break;
    }
    
    logfile.WriteEx("ok.\n");

    //ɾ���ļ�
    if (starg.ptype==2) ftp.ftpdelete(strremotefilename);

    //ת�浽����Ŀ¼
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
      //��ȡ�Է��������ļ�ʱ��
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


//��okfilename�ļ����ݼ��ص�vokfilenae������
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


//��vlistfile�����е��ļ�������vokfilename���ļ��Աȣ��õ���������
//һ����vlistfile�д��ڣ����Ѿ��ɼ��ɹ����ļ�vokfilename1
//������vlistfile�д��ڣ����ļ���Ҫ���²ɼ����ļ�vlistfile1
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
