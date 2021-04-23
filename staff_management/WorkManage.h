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

  //��ʾĿ¼
  void Show_Menu();

  //�������ְԱ
  void add_manger();

  //��ʾְԱ��Ϣ
  void show_manger();

  //ɾ��ְԱ
  void delete_manger();

  //�޸�ְԱ��Ϣ
  void chage_manger();

  //����ְԱ��Ϣ
  void search_manger();
  
  //��ְԱ���������
  void sort();

  //��������ĵ�
  void clear();

  //�˳�
  void Quit_System();


  //�����ļ�
  void save();

  //ȡ��ְԱ����
  int get_manger_number();

  //��ʼ��ְԱ����
  void init_Emp();

  public:
  int m_MangerNumber; //ְԱ��Ŀ
  Work* *m_ManArry; //ְԱ�洢������
  bool IsEmpFile; //�ļ��Ƿ�Ϊ��
  
};