#pragma once

#include <iostream>
#include <string>
using namespace std;

//ְԱ������
class Work
{
  public:
  //Work();
  //~Work();
  virtual void show_info() = 0; //��ʾְԱ����Ϣ
  virtual string get_duty() = 0; //��ȡ��λ����

  
  int m_info; //ְ�����
  string m_name; //ְԱ����
  int m_duty; //ְԱ���ڲ������Ʊ��
};

//��ְͨԱ
class Employee : public Work
{
  public:
  Employee(int info, string name, int duty);
  ~Employee();

  virtual void show_info();
  virtual string get_duty();

};

//����
class Manager : public Work
{
  public:
  Manager(int info, string name, int duty);
  ~Manager();

  virtual void show_info();
  virtual string get_duty();
};

//�ϰ�
class Boss : public Work
{
  public:
  Boss(int info, string name, int duty);
  ~Boss();

  virtual void show_info();
  virtual string get_duty();
};


