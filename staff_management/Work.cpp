#include "Work.h"

//职员
Employee::Employee(int info, string name, int duty)
{
  this->m_info = info;
  this->m_name = name;
  this->m_duty = duty;
}

void Employee::show_info()
{  
    cout << "职员编号：" << this->m_info 
         << "\t职员姓名：" << this->m_name 
         << "\t职员岗位：" << this->get_duty() 
         << "\t职员职责：" << "完成经理交给的任务。" << endl;
    return;
}

string Employee::get_duty()
{
    return "员工" ;
}

Employee::~Employee()
{

}

//经理
Manager::Manager(int info, string name, int duty)
{
  this->m_info = info;
  this->m_name = name;
  this->m_duty = duty;
}

void Manager::show_info()
{  
    cout << "职员编号：" << this->m_info 
         << "\t职员姓名：" << this->m_name
         << "\t职员岗位：" << this->get_duty() 
         << "\t职员职责：" << "完成老板交给的任务，并下发任务给员工。" << endl;
    return;
}

string Manager::get_duty()
{
    return "经理" ;
}

Manager::~Manager()
{

}

//老板
Boss::Boss(int info, string name, int duty)
{
  this->m_info = info;
  this->m_name = name;
  this->m_duty = duty;
}

void Boss::show_info()
{  
    cout << "职员编号：" << this->m_info 
         << "\t职员姓名：" << this->m_name 
         << "\t职员岗位：" << this->get_duty() 
         << "\t职员职责：" << "管理公司所有事务。" << endl;
    return;
}

string Boss::get_duty()
{
    return "老板" ;
}

Boss::~Boss()
{

}


