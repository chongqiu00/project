#include "_public.h"

//文件操作
CFile::CFile()  //构造函数进行初始化
{
  m_fp = NULL;
  m_bEnBuffer = true;
  memset(m_filename, 0, sizeof(m_filename));
  memset(m_filenametmp, 0, sizeof(m_filenametmp));
}

CFile::~CFile()  //析构函数关闭打开的文件
{
  Close();
}

void CFile::Close()  //关闭文件
 {
   if (m_fp == 0)  return;

   fclose(m_fp);
   m_fp = 0;
   memset(m_filename, 0, sizeof(m_filename));

   if (strlen(m_filenametmp) != 0)  remove(m_filenametmp);//有临时文件时就删除（例如：程序突然终止工作）

   memset(m_filenametmp, 0, sizeof(m_filenametmp));
 }

bool CFile::CloseAndRename()
{
   
   if (m_fp == 0)  return false;

   fclose(m_fp);

   if (rename(m_filenametmp, m_filename) != 0)
   {
     remove(m_filenametmp);
     memset(m_filename, 0, sizeof(m_filename));
     memset(m_filenametmp, 0, sizeof(m_filenametmp));
     return false;
   }
   
   m_fp = 0;
   memset(m_filename, 0, sizeof(m_filename));
   memset(m_filenametmp, 0, sizeof(m_filenametmp));
   return true;
}

bool CFile::Open(const char *filename, const char*openmode,bool bEnBuffer)  //打开文件
 {
   Close(); //需要实现关闭之前打开的文件
   
  if ( (m_fp=FOPEN(filename, openmode)) == 0 )  return false;
  
  memset(m_filename, 0, sizeof(m_filename));
  
  strncpy(m_filename, filename, 300);
   
  m_bEnBuffer = bEnBuffer;
  
  return true;
 }


bool CFile::OpenForRename(const char *filename, const char*openmode,bool bEnBuffer)  //打开文件
{
  Close();

  memset(m_filename, 0, sizeof(m_filename));
  strncpy(m_filename, filename, 300);

  memset(m_filenametmp, 0, sizeof(m_filenametmp));
  SNPRINTF(m_filenametmp, 300, "%s.tmp", m_filename);

  if ( (m_fp=FOPEN(m_filenametmp, openmode)) == 0 )  return false;

  m_bEnBuffer = bEnBuffer;

  return true;
}


 void CFile::Fprintf(const char *fmt,... )
 {
   if (m_fp == 0)  return;

   va_list arg;

   va_start(arg, fmt);
   vfprintf(m_fp, fmt, arg);
   va_end(arg);

   if (m_bEnBuffer == false)  fflush(m_fp);
 }

bool CFile::Fgets(char *strBuffer, const int ReadSize, bool bDelCRT)
{
  if (m_fp==0)  return false;
    
  memset(strBuffer, 0, ReadSize+1);
  
  if (fgets(strBuffer, ReadSize, m_fp) == 0)  return false;
 
  if (bDelCRT == true)//删除每行的换行符（windos，Linux）
  {
    DeleteRChar(strBuffer, '\n');  DeleteRChar(strBuffer, '\r');
  }

  return true;
}


////////////////////////////////////////////////////////////
//字符串拆分
CCmdStr::CCmdStr()
{
  m_vCmdStr.clear();
}

void CCmdStr::SplitToCmd(const string in_string,const char *in_sep,const bool bdeletespace)
{
  // 清除所有的旧数据
  m_vCmdStr.clear();
  
  int iPOS=0;
  string srcstr,substr;

  srcstr=in_string;

  char str[2048];

  while ( (iPOS=srcstr.find(in_sep)) >= 0)
  {
    substr=srcstr.substr(0,iPOS);

    if (bdeletespace == true)
    {
      memset(str,0,sizeof(str));

      strncpy(str,substr.c_str(),2000);

      DeleteLRChar(str,' ');

      substr=str;
    }

    m_vCmdStr.push_back(substr);

    iPOS=iPOS+strlen(in_sep);

    srcstr=srcstr.substr(iPOS,srcstr.size()-iPOS);

  }

  substr=srcstr;

  if (bdeletespace == true)
  {
    memset(str,0,sizeof(str));

    strncpy(str,substr.c_str(),2000);

    DeleteLRChar(str,' ');

    substr=str;
  }

  m_vCmdStr.push_back(substr);

  return;
}

int CCmdStr::CmdCount()
{
  return m_vCmdStr.size();
}

bool CCmdStr::GetValue(const int inum,char *in_return)
{
  if (inum >= m_vCmdStr.size()) return false;

  strcpy(in_return,m_vCmdStr[inum].c_str());

  return true;
}

bool CCmdStr::GetValue(const int inum,char *in_return,const int in_len)
{
  memset(in_return,0,in_len+1);

  if (inum >= m_vCmdStr.size()) return false;

  if (m_vCmdStr[inum].length() > (unsigned int)in_len)
  {
    strncpy(in_return,m_vCmdStr[inum].c_str(),in_len);
  }
  else
  {
    strcpy(in_return,m_vCmdStr[inum].c_str());
  }

  return true;
}

bool CCmdStr::GetValue(const int inum,int *in_return)
{
  (*in_return) = 0;

  if (inum >= m_vCmdStr.size()) return false;

  (*in_return) = atoi(m_vCmdStr[inum].c_str());

  return true;
}

bool CCmdStr::GetValue(const int inum,long *in_return)
{
  (*in_return) = 0;

  if (inum >= m_vCmdStr.size()) return false;

  (*in_return) = atol(m_vCmdStr[inum].c_str());

  return true;
}

bool CCmdStr::GetValue(const int inum,double *in_return)
{
  if (inum >= m_vCmdStr.size()) return false;

  (*in_return) = (double)atof(m_vCmdStr[inum].c_str());

  return true;
}

CCmdStr::~CCmdStr()
{
  m_vCmdStr.clear();
}

CLogFile::CLogFile() 
{ 
  m_tracefp = 0;
  memset(m_filename,0,sizeof(m_filename));
  memset(m_openmode,0,sizeof(m_openmode));
  m_bBackup=true;
  m_bEnBuffer=false;
}

CLogFile::~CLogFile() 
{ 
  Close();
}

void CLogFile::Close() 
{ 
  if (m_tracefp != 0) 
  {
    fclose(m_tracefp); m_tracefp=0;
  }
}

/*
  有关文件定位的函数的用法:
  rewind(FILE *fp),使位置指针重返文件的开头,函数无返回值.
  fseek(FILE *fp,位移量,起始点),起始点的取值是:SEEK_SET(0)-文件开始;
  SEEK_CUR(1)-当前位置;SEEK_END(2)-文件未尾,位移量指以起始点为基点,
  移动的字节数(正值往后移,负值往前移),为long数据类型,ANSI C标准规定
  在数字的未尾加上字母L,就表示是long型.
  ftell(FILE *fp),取得文件中的当前位置,返回-1L表示出错.
  ferror(FILE *fp),在调用各种输入输出函数(如putc,getc,fread,fwrite)时,
  如果出现了错误,除了函数返回值有所反应外,还可用ferror函数检查,如果返
  回0表示未出错,非0表示出错,但在调用clearerr(FILE *fp)后,ferror返回0.

*/
// filename日志文件名
// openmode打开文件的方式，操作日志文件的权限,同打开文件函数(FOPEN)使用方法一致
// bBackup，true-备份，false-不备份，在多进程的服务程序中，如果多个进行共用一个日志文件，bBackup必须为false
// bEnBuffer:true-启用缓冲区，false-不启用缓冲区，如果启用缓冲区，那么写进日志文件中的内容不会立即写入文件是
bool CLogFile::Open(const char *in_filename,const char *in_openmode,bool bBackup,bool bEnBuffer)
{
  if (m_tracefp != 0) { fclose(m_tracefp); m_tracefp=0; }

  m_bEnBuffer=bEnBuffer;

  memset(m_filename,0,sizeof(m_filename));
  strcpy(m_filename,in_filename);

  memset(m_openmode,0,sizeof(m_openmode));
  strcpy(m_openmode,in_openmode);

  if ((m_tracefp=FOPEN(m_filename,m_openmode)) == NULL) return false;

  m_bBackup=bBackup;

  return true;
}

// 如果日志文件大于100M，就备份它
bool CLogFile::BackupLogFile()
{
  // 不备份
  if (m_bBackup == false) return true;

  if (m_tracefp == 0) return true;

  fseek(m_tracefp,0L,2); 

  if (ftell(m_tracefp) > 100*1024*1024) 
  {
    fclose(m_tracefp); m_tracefp=0;

    char strLocalTime[21];
    memset(strLocalTime,0,sizeof(strLocalTime));
    LocalTime(strLocalTime,"yyyymmddhh24miss");

    char bak_filename[301]; 
    memset(bak_filename,0,sizeof(bak_filename));
    snprintf(bak_filename,300,"%s.%s",m_filename,strLocalTime);
    rename(m_filename,bak_filename);

    if ((m_tracefp=FOPEN(m_filename,m_openmode)) == NULL) return false;
  }

  return true;
}

bool CLogFile::Write(const char *fmt,...)
{
  if (BackupLogFile() == false) return false;

  memset(m_stime,0,sizeof(m_stime));
  memset(m_message,0,sizeof(m_message));

  LocalTime(m_stime);

  va_start(m_ap,fmt);
  vsnprintf(m_message,10000,fmt,m_ap);
  va_end(m_ap);

  if (m_tracefp == 0) 
  {
    fprintf(stdout,"%s %s",m_stime,m_message);
    if (m_bEnBuffer==false) fflush(stdout);
  }
  else
  {
    fprintf(m_tracefp,"%s %s",m_stime,m_message);
    if (m_bEnBuffer==false) fflush(m_tracefp);
  }

  return true;  
}

bool CLogFile::WriteEx(const char *fmt,...)
{
  memset(m_stime,0,sizeof(m_stime));
  memset(m_message,0,sizeof(m_message));

  LocalTime(m_stime);

  va_start(m_ap,fmt);
  vsnprintf(m_message,10000,fmt,m_ap);
  va_end(m_ap);

  if (m_tracefp == 0) 
  {
    fprintf(stdout,"%s",m_message);
    if (m_bEnBuffer==false) fflush(stdout);
  }
  else
  {
    fprintf(m_tracefp,"%s",m_message);
    if (m_bEnBuffer==false) fflush(m_tracefp);
  }

  return true;  
}

/* 删除字符串左边指定的字符 */
void DeleteLChar(char *in_string,const char in_char)
{
  if (in_string == 0) return;

  if (strlen(in_string) == 0) return;

  char strTemp[strlen(in_string)+1];

  int iTemp=0;

  memset(strTemp,0,sizeof(strTemp));
  strcpy(strTemp,in_string);

  while ( strTemp[iTemp] == in_char )  iTemp++;

  memset(in_string,0,strlen(in_string)+1);

  strcpy(in_string,strTemp+iTemp);

  return;
}

/* 删除字符串右边指定的字符 */
void DeleteRChar(char *in_string,const char in_char)
{
  if (in_string == 0) return;

  int istrlen = strlen(in_string);

  while (istrlen>0)
  {
    if (in_string[istrlen-1] != in_char) break;

    in_string[istrlen-1]=0;

    istrlen--;
  }
}

/* 删除字符串两边指定的字符 */
void DeleteLRChar(char *in_string,const char in_char)
{
  DeleteLChar(in_string,in_char);
  DeleteRChar(in_string,in_char);
}

/*
   取操作系统的时间
   out_stime是输出结果
   in_interval是偏移常量，单位是秒
   返回的格式由fmt决定，fmt目前的取值如下，如果需要，可以增加：
   yyyy-mm-dd hh:mi:ss，此格式是缺省格式
   yyyymmddhhmiss
   yyyy-mm-dd
   yyyymmdd
   hh:mi:ss
   hhmiss
   hh:mi
   hhmi
   hh
   mi
*/
void LocalTime(char *out_stime,const char *in_fmt,const int in_interval)
{
  if (in_fmt == 0) memset(out_stime, 0, 19+1);
  else memset(out_stime, 0, strlen(in_fmt)+1);

  time_t  timer;
  struct tm nowtimer;

  time( &timer ); timer=timer+in_interval;
  nowtimer = *localtime ( &timer ); nowtimer.tm_mon++;

  if (in_fmt==0)
  {
    snprintf(out_stime,20,"%04u-%02u-%02u %02u:%02u:%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min,nowtimer.tm_sec);
    return;
  }

  if (strcmp(in_fmt,"yyyy-mm-dd hh:mi:ss") == 0)
  {
    snprintf(out_stime,20,"%04u-%02u-%02u %02u:%02u:%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min,nowtimer.tm_sec);
    return;
  }

  if (strcmp(in_fmt,"yyyy-mm-dd hh:mi") == 0)
  {
    snprintf(out_stime,17,"%04u-%02u-%02u %02u:%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min);
    return;
  }

  if (strcmp(in_fmt,"yyyy-mm-dd hh") == 0)
  {
    snprintf(out_stime,14,"%04u-%02u-%02u %02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour);
    return;
  }

  if (strcmp(in_fmt,"yyyy-mm-dd") == 0)
  {
    snprintf(out_stime,11,"%04u-%02u-%02u",nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday); return;
  }

  if (strcmp(in_fmt,"yyyy-mm") == 0)
  {
    snprintf(out_stime,8,"%04u-%02u",nowtimer.tm_year+1900,nowtimer.tm_mon); return;
  }

  if (strcmp(in_fmt,"yyyymmddhhmiss") == 0)
  {
    snprintf(out_stime,15,"%04u%02u%02u%02u%02u%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min,nowtimer.tm_sec);
    return;
  }

  if (strcmp(in_fmt,"yyyymmddhhmi") == 0)
  {
    snprintf(out_stime,13,"%04u%02u%02u%02u%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min);
    return;
  }

  if (strcmp(in_fmt,"yyyymmddhh") == 0)
  {
    snprintf(out_stime,11,"%04u%02u%02u%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour);
    return;
  }

  if (strcmp(in_fmt,"yyyymmdd") == 0)
  {
    snprintf(out_stime,9,"%04u%02u%02u",nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday); return;
  }

  if (strcmp(in_fmt,"hhmiss") == 0)
  {
    snprintf(out_stime,7,"%02u%02u%02u",nowtimer.tm_hour,nowtimer.tm_min,nowtimer.tm_sec); return;
  }

  if (strcmp(in_fmt,"hhmi") == 0)
  {
    snprintf(out_stime,5,"%02u%02u",nowtimer.tm_hour,nowtimer.tm_min); return;
  }

  if (strcmp(in_fmt,"hh") == 0)
  {
    snprintf(out_stime,3,"%02u",nowtimer.tm_hour); return;
  }

  if (strcmp(in_fmt,"mi") == 0)
  {
    snprintf(out_stime,3,"%02u",nowtimer.tm_min); return;
  }
}

// 安全的strcpy函数。
// dest：目标字符串，不需要初始化，在STRCPY函数中有初始化代码。
// destlen：目标字符串dest占用内存的大小。
// src：原字符串。
// 返回值：目标字符串dest的地址。
char *STRCPY(char* dest,const size_t destlen,const char* src)
{
  if (dest==0) return 0;
  memset(dest,0,destlen);   // 初始化dest。
  if (src==0) return dest;

  if (strlen(src)>destlen-1) strncpy(dest,src,destlen-1); 
  else strcpy(dest,src);

  return dest;
}

int SNPRINTF(char* str, size_t size, const char* fmt, ...)
{
  memset(str, 0, size);
  va_list arg;

  va_start(arg, fmt);
  vsnprintf(str, size, fmt, arg);
  va_end(arg);
}

//关闭全部的信号和输入输出
void CloseIOAndSignal()
{
  int ii;

  for (ii=0; ii<50; ii++)
  {
    signal(ii, SIG_IGN);
    close(ii);
  }
}

// 用某文件或目录的全路径中的目录创建目录，以级该目录下的各级子目录
bool MKDIR(const char *filename,bool bisfilename)
{
  // 检查目录是否存在，如果不存在，逐级创建子目录
  char strPathName[301];

  for (int ii=1; ii<strlen(filename);ii++)
  {
    if (filename[ii] != '/') continue;

    memset(strPathName,0,sizeof(strPathName));
    strncpy(strPathName,filename,ii);

    if (access(strPathName,F_OK) == 0) continue;

    if (mkdir(strPathName,00755) != 0) return false;
  }

  if (bisfilename==false)
  {
    if (access(filename,F_OK) != 0)
    {
      if (mkdir(filename,00755) != 0) return false;
    }
  }

  return true;
}

// 调用FOPEN函数打开文件，如果文件名中包含的目录不存在，就创建目录
FILE *FOPEN(const char *filename,const char *mode)
{
  if (MKDIR(filename) == false) return NULL;

  return fopen(filename,mode);
}

// 更改文件的修改时间属性
int UTime(const char *filename,const char *mtime)
{
  struct utimbuf stutimbuf;

  stutimbuf.actime=stutimbuf.modtime=UTCTime(mtime);

  return utime(filename,&stutimbuf);
}

// 把字符串格式的时间转换为time_t
// stime为输入的时间，格式不限，但一定要包括yyyymmddhh24miss
time_t UTCTime(const char *stime)
{
  char strtime[21],yyyy[5],mm[3],dd[3],hh[3],mi[3],ss[3];
  memset(strtime,0,sizeof(strtime));
  memset(yyyy,0,sizeof(yyyy));
  memset(mm,0,sizeof(mm));
  memset(dd,0,sizeof(dd));
  memset(hh,0,sizeof(hh));
  memset(mi,0,sizeof(mi));
  memset(ss,0,sizeof(ss));

  PickNumber(stime,strtime,false,false);

  if (strlen(strtime) != 14) return -1;

  strncpy(yyyy,strtime,4);
  strncpy(mm,strtime+4,2);
  strncpy(dd,strtime+6,2);
  strncpy(hh,strtime+8,2);
  strncpy(mi,strtime+10,2);
  strncpy(ss,strtime+12,2);

  struct tm time_str;

  time_str.tm_year = atoi(yyyy) - 1900;
  time_str.tm_mon = atoi(mm) - 1;
  time_str.tm_mday = atoi(dd);
  time_str.tm_hour = atoi(hh);
  time_str.tm_min = atoi(mi);
  time_str.tm_sec = atoi(ss);
  time_str.tm_isdst = 0;

  return mktime(&time_str);
}

// 从一个字符串中提取数字，bWithSign==true表示包括负号，bWithDOT==true表示包括圆点
void PickNumber(const char *strSrc,char *strDst,const bool bWithSign,const bool bWithDOT)
{
  char strtemp[1024];
  memset(strtemp,0,sizeof(strtemp));
  strncpy(strtemp,strSrc,1000);
  DeleteLRChar(strtemp,' ');

  // 为了防止strSrc和strDst为同一变量的情况，所以strDst不能初始化

  // 判断字符串中的负号是否合法
  if ( (bWithSign==true) && (JudgeSignDOT(strtemp,"-") == false) )
  {
    strcpy(strDst,""); return;
  }

  // 判断字符串中的正号是否合法
  if ( (bWithSign==true) && (JudgeSignDOT(strtemp,"+") == false) )
  {
    strcpy(strDst,""); return;
  }

  // 判断字符串中的圆点是否合法
  if ( (bWithDOT==true) && (JudgeSignDOT(strtemp,".") == false) )
  {
    strcpy(strDst,""); return;
  }

  int iPosSrc,iPosDst,iLen;
  iPosSrc=iPosDst=iLen=0;

  iLen=strlen(strtemp);

  for (iPosSrc=0;iPosSrc<iLen;iPosSrc++)
  {
    if ( (bWithSign==true) && (strtemp[iPosSrc] == '+') )
    {
      strDst[iPosDst++]=strtemp[iPosSrc]; continue;
    }

    if ( (bWithSign==true) && (strtemp[iPosSrc] == '-') )
    {
      strDst[iPosDst++]=strtemp[iPosSrc]; continue;
    }

    if ( (bWithDOT==true) && (strtemp[iPosSrc] == '.') )
    {
      strDst[iPosDst++]=strtemp[iPosSrc]; continue;
    }

    if (isdigit(strtemp[iPosSrc])) strDst[iPosDst++]=strtemp[iPosSrc];
  }

  strDst[iPosDst]=0;

  return;
}

// 判断字符串中的负号和圆点是否合法
bool JudgeSignDOT(const char *strSrc,const char *strBZ)
{
  char *pos=0;
  pos=(char *)strstr(strSrc,strBZ);

  // 如果没有包括待搜索的字符串，就返回true
  if (pos == 0) return true;

  // 如果strlen(pos)==1，表示结果中只有符号，没有其它字符，返回false
  if (strlen(pos)==1) return false;

  // 如果待搜索的字符串是+号，就一定要是第一个字符
  if ( (strcmp(strBZ,"+") == 0) && (strncmp(strSrc,"+",1) != 0) ) return false;

  // 如果待搜索的字符串是-号，就一定要是第一个字符
  if ( (strcmp(strBZ,"-") == 0) && (strncmp(strSrc,"-",1) != 0) ) return false;

  // 如果包括多个待搜索的字符串，就返回false
  if (strstr(pos+1,strBZ) > 0) return false;

  return true;
}

// 获取文件的大小，返回字节数
int FileSize(const char *in_FullFileName)
{
  struct stat st_filestat;

  if (stat(in_FullFileName,&st_filestat) < 0) return -1;

  return st_filestat.st_size;
}

/*
  把一个字符串表格的时间加上一个偏移量，得到偏移后的时间
  in_stime是传入的时间，任意格式，但是一定要包括yyyymmddhh24miss，是否有分隔符没有关系。
  把yyyy-mm-dd hh24:mi:ss偏移in_interval秒
  传出的格式由fmt决定，fmt目前的取值如下，如果需要，可以增加：
  yyyy-mm-dd hh24:mi:ss（此格式是缺省格式）
  yyyymmddhh24miss
  yyyymmddhh24miss
  yyyy-mm-dd
  yyyymmdd
  hh24:mi:ss
  hh24miss
  hh24:mi
  hh24mi
  返回值：0-成功，-1-失败。
*/
int AddTime(const char *in_stime,char *out_stime,const int in_interval,const char *in_fmt)
{
  time_t  timer;
  struct tm nowtimer;

  timer=UTCTime(in_stime)+in_interval;

  nowtimer = *localtime ( &timer ); nowtimer.tm_mon++;

  // 为了防止in_stime和out_stime为同一变量的情况，所以out_stime在此处初始化，代码不可提前
  out_stime[0]=0;

  if (in_fmt==0)
  {
    snprintf(out_stime,20,"%04u-%02u-%02u %02u:%02u:%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min,nowtimer.tm_sec); return 0;
  }

  if (strcmp(in_fmt,"yyyy-mm-dd hh24:mi:ss") == 0)
  {
    snprintf(out_stime,20,"%04u-%02u-%02u %02u:%02u:%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min,nowtimer.tm_sec); return 0;
  }

  if (strcmp(in_fmt,"yyyymmddhh24miss") == 0)
  {
    snprintf(out_stime,15,"%04u%02u%02u%02u%02u%02u",nowtimer.tm_year+1900,
                    nowtimer.tm_mon,nowtimer.tm_mday,nowtimer.tm_hour,
                    nowtimer.tm_min,nowtimer.tm_sec); return 0;
  }

  if (strcmp(in_fmt,"yyyy-mm-dd") == 0)
  {
    snprintf(out_stime,11,"%04u-%02u-%02u",nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday); return 0;
  }
  if (strcmp(in_fmt,"yyyymmdd") == 0)
  {
    snprintf(out_stime,9,"%04u%02u%02u",nowtimer.tm_year+1900,nowtimer.tm_mon,nowtimer.tm_mday); return 0;
  }

  if (strcmp(in_fmt,"hh24:mi:ss") == 0)
  {
    snprintf(out_stime,9,"%02u:%02u:%02u",nowtimer.tm_hour,nowtimer.tm_min,nowtimer.tm_sec); return 0;
  }

  if (strcmp(in_fmt,"hh24:mi") == 0)
  {
    snprintf(out_stime,9,"%02u:%02u",nowtimer.tm_hour,nowtimer.tm_min); return 0;
  }


  if (strcmp(in_fmt,"hh24mi") == 0)
  {
    snprintf(out_stime,7,"%02u%02u",nowtimer.tm_hour,nowtimer.tm_min); return 0;
  }

  return -1;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 操作XMLBuffer的函数
// in_XMLBuffer，XML格式的字符串
// in_FieldName，字段的标签名
// out_Value，获取内容存放的变量的指针
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,char *out_Value,const int in_Len)
{
  strcpy(out_Value,"");

  char *start=NULL,*end=NULL;
  char m_SFieldName[51],m_EFieldName[51];

  int m_NameLen = strlen(in_FieldName);
  memset(m_SFieldName,0,sizeof(m_SFieldName));
  memset(m_EFieldName,0,sizeof(m_EFieldName));

  snprintf(m_SFieldName,50,"<%s>",in_FieldName);
  snprintf(m_EFieldName,50,"</%s>",in_FieldName);

  start=0; end=0;

  start = (char *)strstr(in_XMLBuffer,m_SFieldName);

  if (start != 0)
  {
    end   = (char *)strstr(start,m_EFieldName);
  }

  if ((start==0) || (end == 0))
  {
    return false;
  }

  int   m_ValueLen = end - start - m_NameLen - 2 + 1 ;

  if ( ((m_ValueLen-1) <= in_Len) || (in_Len == 0) )
  {
    strncpy(out_Value,start+m_NameLen+2,m_ValueLen-1); out_Value[m_ValueLen-1]=0;
  }
  else
  {
    strncpy(out_Value,start+m_NameLen+2,in_Len); out_Value[in_Len]=0;
  }

  DeleteLRChar(out_Value,' ');

  return true;
}

bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,bool *out_Value)
{
  (*out_Value) = false;

  char strTemp[51];

  memset(strTemp,0,sizeof(strTemp));

  if (GetXMLBuffer(in_XMLBuffer,in_FieldName,strTemp,10) == true)
  {
    if ( (strcmp(strTemp,"TRUE")==0) || (strcmp(strTemp,"true")==0) ) (*out_Value)=true; return true;
  }

  return false;
}

bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,int *out_Value)
{
  (*out_Value) = 0;

  char strTemp[51];

  memset(strTemp,0,sizeof(strTemp));

  if (GetXMLBuffer(in_XMLBuffer,in_FieldName,strTemp,50) == true)
  {
    (*out_Value) = atoi(strTemp); return true;
  }

  return false;
}

bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,unsigned int *out_Value)
{
  (*out_Value) = 0;

  char strTemp[51];

  memset(strTemp,0,sizeof(strTemp));

  if (GetXMLBuffer(in_XMLBuffer,in_FieldName,strTemp,50) == true)
  {
    (*out_Value) = (unsigned int)atoi(strTemp); return true;
  }

  return false;
}

bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,long *out_Value)
{
  (*out_Value) = 0;

  char strTemp[51];

  memset(strTemp,0,sizeof(strTemp));

  if (GetXMLBuffer(in_XMLBuffer,in_FieldName,strTemp,50) == true)
  {
    (*out_Value) = atol(strTemp); return true;
  }

  return false;
}

bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,unsigned long *out_Value)
{
  (*out_Value) = 0;

  char strTemp[51];

  memset(strTemp,0,sizeof(strTemp));

  if (GetXMLBuffer(in_XMLBuffer,in_FieldName,strTemp,50) == true)
  {
    (*out_Value) = (unsigned long)atol(strTemp); return true;
  }

  return false;
}

bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,double *out_Value)
{
  (*out_Value) = 0;

  char strTemp[51];

  memset(strTemp,0,sizeof(strTemp));

  if (GetXMLBuffer(in_XMLBuffer,in_FieldName,strTemp,50) == true)
  {
    (*out_Value) = atof(strTemp); return true;
  }

  return false;
}
