#include "Work.h"

//ְԱ
Employee::Employee(int info, string name, int duty)
{
  this->m_info = info;
  this->m_name = name;
  this->m_duty = duty;
}

void Employee::show_info()
{  
    cout << "ְԱ��ţ�" << this->m_info 
         << "\tְԱ������" << this->m_name 
         << "\tְԱ��λ��" << this->get_duty() 
         << "\tְԱְ��" << "��ɾ�����������" << endl;
    return;
}

string Employee::get_duty()
{
    return "Ա��" ;
}

Employee::~Employee()
{

}

//����
Manager::Manager(int info, string name, int duty)
{
  this->m_info = info;
  this->m_name = name;
  this->m_duty = duty;
}

void Manager::show_info()
{  
    cout << "ְԱ��ţ�" << this->m_info 
         << "\tְԱ������" << this->m_name
         << "\tְԱ��λ��" << this->get_duty() 
         << "\tְԱְ��" << "����ϰ彻�������񣬲��·������Ա����" << endl;
    return;
}

string Manager::get_duty()
{
    return "����" ;
}

Manager::~Manager()
{

}

//�ϰ�
Boss::Boss(int info, string name, int duty)
{
  this->m_info = info;
  this->m_name = name;
  this->m_duty = duty;
}

void Boss::show_info()
{  
    cout << "ְԱ��ţ�" << this->m_info 
         << "\tְԱ������" << this->m_name 
         << "\tְԱ��λ��" << this->get_duty() 
         << "\tְԱְ��" << "����˾��������" << endl;
    return;
}

string Boss::get_duty()
{
    return "�ϰ�" ;
}

Boss::~Boss()
{

}


