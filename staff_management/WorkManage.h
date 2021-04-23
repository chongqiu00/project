#pragma once

#include <string.h>
#include "Work.h"
#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

#define FILENAME "empFile.txt"

class WorkManger
{
  public:
  WorkManger();
  ~WorkManger();

  //显示目录
  void Show_Menu();

  //批量添加职员
  void add_manger();

  //显示职员信息
  void show_manger();

  //删除职员
  void delete_manger();

  //修改职员信息
  void chage_manger();

  //查找职员信息
  void search_manger();
  
  //将职员按编号排序
  void sort();

  //清空所有文档
  void clear();

  //退出
  void Quit_System();


  //保存文件
  void save();

  //取得职员数量
  int get_manger_number();

  //初始化职员数组
  void init_Emp();

  public:
  int m_MangerNumber; //职员数目
  Work* *m_ManArry; //职员存储的数组
  bool IsEmpFile; //文件是否为空
  
};