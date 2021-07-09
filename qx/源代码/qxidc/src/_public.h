#ifndef _PUBLIC_H
#define _PUBLIC_H

//ͷ�ļ�
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <utime.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include<sys/stat.h>
#include<unistd.h>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
//�ļ�����������
class CFile
{
private:
	FILE *m_fp;	//�ļ�ָ��
	bool m_bEnBuffer;//�Ƿ����û�������ture-������false-������
	char m_filename[301]; //�ļ���
	char m_filenametmp[301];//��ʱ�ļ���
public:
	CFile();	//��Ĺ��캯��
	~CFile();	//�����������

	//���ļ���������fopen��ͬ���򿪳ɹ�true,ʧ�ܷ���false
	bool Open(const char *filename, const char*openmode, bool bEnBuffer=true);
        bool OpenForRename(const char *filename, const char*openmode,bool bEnBuffer=true);//���ļ�

	void Fprintf(const char *fmt,... );

	//����fgets���ļ��ж�ȡһ��,bDelCRT�Ƿ�ɾ��ĩβ���з�����windos,linux����true-�ǣ�false-��
	bool Fgets(char *strBuffer, const int ReadSize, bool bDelCRT=false);

	//�ر��ļ�ָ��
	void Close();

	//�ر��ļ������Ҹ���
	bool CloseAndRename();
};

//�ַ����Ĳ��
class CCmdStr
{
  public:
  vector<string> m_vCmdStr;  // ��Ų�ֺ���ֶ����ݡ�

  CCmdStr();

  // ����ַ�����������
  void SplitToCmd(const string in_string,const char *in_sep,const bool bdeletespace=true);

  int CmdCount();

  // ��ȡ�ֶε�ֵ��ȡÿ���ֶε�ֵinum��0��ʼ
  bool GetValue(const int inum,char   *in_return);
  bool GetValue(const int inum,char   *in_return,const int in_len);
  bool GetValue(const int inum,int    *in_return);
  bool GetValue(const int inum,long   *in_return);
  bool GetValue(const int inum,double *in_return);

  ~CCmdStr();
};

// ��־�ļ�������
class CLogFile 
{
public:
  FILE   *m_tracefp;           // ��־�ļ�ָ��
  char    m_filename[301];     // ��־�ļ�ȫ��
  char    m_openmode[11];      // ��־�ļ��Ĵ򿪷�ʽ
  char    m_stime[20];         // ��־�ļ�д��ʱ�ĵ�ǰʱ�����
  char    m_message[10240];    // ��д�����־����
  bool    m_bBackup;           // ��־�ļ�����100M���Ƿ��Զ�����
  bool    m_bEnBuffer;         // д����־ʱ���Ƿ����ò���ϵͳ�Ļ������
  va_list m_ap;

  CLogFile();

  // filename��־�ļ���
  // openmode���ļ��ķ�ʽ��������־�ļ���Ȩ��,ͬ���ļ�����(FOPEN)ʹ�÷���һ�£�һ�����"a+"
  // bBackup��true-���ݣ�false-�����ݣ��ڶ���̵ķ�������У����������й���һ����־�ļ���bBackup����Ϊfalse
  // bEnBuffer:true-���û�������false-�����û�������������û���������ôд����־�ļ��е����ݲ�������д���ļ�
  bool Open(const char *in_filename,const char *in_openmode,bool bBackup=true,bool bEnBuffer=false);

  // �����־�ļ�����100M���ͱ�����
  // ���ݺ���ļ�����Դ�ļ������������ʱ��
  bool BackupLogFile();

  // д��־�ļ�,���Ǹ��ɱ�����ķ���,ͬprintf������
  // Write()������д��ʱ�䣬WriteEx()������дʱ�䡣
  bool Write(const char *fmt,...);
  bool WriteEx(const char *fmt,...);

  // �ر���־�ļ�
  void Close();

  ~CLogFile();
};

// ɾ���ַ������ָ�����ַ�
void DeleteLChar(char *in_string,const char in_char);

// ɾ���ַ����ұ�ָ�����ַ�
void DeleteRChar(char *in_string,const char in_char);

// ɾ���ַ�������ָ�����ַ�
void DeleteLRChar(char *in_string,const char in_char);
/*
  ȡ����ϵͳ��ʱ��
  out_stime��������
  in_interval��ƫ�Ƴ�������λ����
  ���صĸ�ʽ��fmt������fmtĿǰ��ȡֵ���£������Ҫ���������ӣ�
  yyyy-mm-dd hh:mi:ss���˸�ʽ��ȱʡ��ʽ
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
void LocalTime(char *out_stime,const char *in_fmt=0,const int in_interval=0);
//��д�⺯��
//������⣺1��������ʼ����2���ڴ������3���޸�strncpy��ȱ��
//�����ĵڶ��������ǵ�һ������dest��Ч���ȣ���ռ���ڴ���ֽ�����
char* STRCPY(char* dest,const size_t destlen,const char* src);
char* STRNCPY(char* dest, const size_t destlen, const char* src, size_t n);
char* STRCAT(char* dest, const size_t destlen, const char* src);
char* STRNCAT(char* dest, const size_t destlen, const char* src, size_t n);

int SNPRINTF(char* str, size_t size, const char* fmt, ...);

//�ر�ȫ�����źź��������
void CloseIOAndSignal();

// ��ĳ�ļ���Ŀ¼��ȫ·���е�Ŀ¼����Ŀ¼���Լ���Ŀ¼�µĸ�����Ŀ¼
// pathorfilename Ŀ¼�����ļ���
// bisfilename true-pathorfilename���ļ�����������Ŀ¼��
bool MKDIR(const char *pathorfilename,bool bisfilename=true);

// ����fopen�������ļ�������ļ����а�����Ŀ¼�����ڣ��ʹ���Ŀ¼
FILE *FOPEN(const char *filename,const char *mode);

// �����ļ����޸�ʱ�����ԣ�mtimeָ����ʱ�䣬��ʽ���ޣ�ֻҪ������yyyy,mm,dd,hh24,mi,ss���ɡ�
int UTime(const char *filename,const char *mtime);

// ���ַ�����ʽ��ʱ��ת��Ϊtime_t
// stimeΪ�����ʱ�䣬��ʽ���ޣ���һ��Ҫ����yyyymmddhh24miss
time_t UTCTime(const char *stime);

// ��һ���ַ�������ȡ���֡����ź�С���㣬���ж��Ƿ���һ���Ϸ�����
// ������Ϸ��������ؿ��ַ���
// bWithSign==true��ʾ�������ţ�bWithDOT==true��ʾ����Բ��
void PickNumber(const char *strSrc,char *strDst,const bool bWithSign,const bool bWithDOT);

// �ж��ַ����еĸ��ź�Բ���Ƿ�Ϸ�
bool JudgeSignDOT(const char *strSrc,const char *strBZ);

// ��ȡ�ļ��Ĵ�С�������ֽ���
int FileSize(const char *in_FullFileName);

/*
  ��һ���ַ�������ʱ�����һ��ƫ�������õ�ƫ�ƺ��ʱ��
  in_stime�Ǵ����ʱ�䣬�����ʽ������һ��Ҫ����yyyymmddhh24miss���Ƿ��зָ���û�й�ϵ��
  ��yyyy-mm-dd hh24:mi:ssƫ��in_interval��
  �����ĸ�ʽ��fmt������fmtĿǰ��ȡֵ���£������Ҫ���������ӣ�
  yyyy-mm-dd hh24:mi:ss���˸�ʽ��ȱʡ��ʽ��
  yyyymmddhh24miss
  yyyymmddhh24miss
  yyyy-mm-dd
  yyyymmdd
  hh24:mi:ss
  hh24miss
  hh24:mi
  hh24mi
  ����ֵ��0-�ɹ���-1-ʧ�ܡ�
*/
int AddTime(const char *in_stime,char *out_stime,const int in_interval,const char *in_fmt=0);

// ������XML��ʽ�ַ�������ز�����������

// ����XMLBuffer�ĺ���
// in_XMLBuffer��XML��ʽ���ַ��������£�
// <filename>/tmp/readme.txt</filename><mtime>2018-01-01 12:20:35</mtime><size>10241</size>
// in_FieldName���ֶεı�ǩ��
// out_Value����ȡ���ݴ�ŵı�����ָ��
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,bool   *out_Value);
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,int    *out_Value);
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,unsigned int *out_Value);
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,long   *out_Value);
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,unsigned long *out_Value);
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,double *out_Value);
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,char   *out_Value,const int in_StrLen=0);

#endif



