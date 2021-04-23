#pragma once

#include <iostream>
#include <string>
using namespace std;

//职员抽象类
class Work
{
  public:
  //Work();
  //~Work();
  virtual void show_info() = 0; //显示职员的信息
  virtual string get_duty() = 0; //获取岗位名称

  
  int m_info; //职工编号
  string m_name; //职员姓名
  int m_duty; //职员所在部门名称编号
};

//普通职员
class Employee : public Work
{
  public:
  Employee(int info, string name, int duty);
  ~Employee();

  virtual void show_info();
  virtual string get_duty();

};

//经理
class Manager : public Work
{
  public:
  Manager(int info, string name, int duty);
  ~Manager();

  virtual void show_info();
  virtual string get_duty();
};

//老板
class Boss : public Work
{
  public:
  Boss(int info, string name, int duty);
  ~Boss();

  virtual void show_info();
  virtual string get_duty();
};


