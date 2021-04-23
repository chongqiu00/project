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

//存储个体
struct st_Contact
{
        char id[5];     //id号
        char name[30];  //姓名
        char sex[10];   //性别
        char age[10];        //年龄
        char address[100];      //地址
        char Tel[11];    //电话
};

//首页显示
void HomePage(void);

//选项功能实现
void AddContact(void);  //添加联系人
void ShowContact(void); //显示联系人
void DelectContact(void);//删除联系人
void SeekContact(void); //查找联系人
void UpdateContact(void); //修改联系人
void ClearContact(void); //清空联系人
void QuitContact(void); //退出通讯录

// 操作XMLBuffer的函数
// in_XMLBuffer，XML格式的字符串，如下：
// <filename>/tmp/readme.txt</filename><mtime>2018-01-01 12:20:35</mtime><size>10241</size>
// in_FieldName，字段的标签名
// out_Value，获取内容存放的变量的指针
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,char   *out_Value,const int in_StrLen=0);

#endif