#ifndef _PUBLIC_H
#define _PUBLIC_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//�洢����
struct st_Contact
{
        char id[5];     //id��
        char name[30];  //����
        char sex[10];   //�Ա�
        char age[10];        //����
        char address[100];      //��ַ
        char Tel[11];    //�绰
};

//��ҳ��ʾ
void HomePage(void);

//ѡ���ʵ��
void AddContact(void);  //�����ϵ��
void ShowContact(void); //��ʾ��ϵ��
void DelectContact(void);//ɾ����ϵ��
void SeekContact(void); //������ϵ��
void UpdateContact(void); //�޸���ϵ��
void ClearContact(void); //�����ϵ��
void QuitContact(void); //�˳�ͨѶ¼

// ����XMLBuffer�ĺ���
// in_XMLBuffer��XML��ʽ���ַ��������£�
// <filename>/tmp/readme.txt</filename><mtime>2018-01-01 12:20:35</mtime><size>10241</size>
// in_FieldName���ֶεı�ǩ��
// out_Value����ȡ���ݴ�ŵı�����ָ��
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,char   *out_Value,const int in_StrLen=0);

#endif