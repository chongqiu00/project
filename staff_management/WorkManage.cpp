#include "WorkManage.h"

WorkManger::WorkManger()
{
    ifstream ifs;
    int num;
    char ch;

    //1.文件不存在
    ifs.open(FILENAME, ios::in); //读文件

    if ( !ifs.is_open() )
    {
        this->m_MangerNumber = 0;
        this->m_ManArry = NULL;
        this->IsEmpFile = true;
        ifs.close();
        return;
    }

    //2.文件存在 数据为空
    ifs >> ch;

    if (ifs.eof())
    {
        this->m_MangerNumber = 0;
        this->m_ManArry = NULL;
        this->IsEmpFile = true;
        ifs.close();
        return;
    }

    //3.文件存在，且记录数据。
    num = this->get_manger_number();
    this->m_MangerNumber = num;
    cout << "该公司共有" << num << "个职员。" << endl;
    this->m_ManArry = new Work* [this->m_MangerNumber]; 
    this->init_Emp(); //初始化数组。
    this->IsEmpFile = false; //设置表不为空。
}

WorkManger::~WorkManger()
{
    if (this->m_ManArry != NULL)
    {
        for ( int i=0; i<this->m_MangerNumber; i++)
        {
            delete m_ManArry[i];
        }
        
        this->m_ManArry = NULL;
        this->m_MangerNumber = 0;
        this->IsEmpFile = true;
    }

}


//显示
void WorkManger::Show_Menu()
{
    cout << "*************************************************" << endl;
    cout << "***************欢迎使用职工管理系统**************" << endl;
    cout << "************** 0. 退出管理系统 ******************" << endl;
    cout << "************** 1. 增加职工信息 ******************" << endl;
    cout << "************** 2. 显示职工信息 ******************" << endl;
    cout << "************** 3. 删除离职职工 ******************" << endl;
    cout << "************** 4. 修改职工信息 ******************" << endl;
    cout << "************** 5. 查找职工信息 ******************" << endl;
    cout << "************** 6. 按照编号排序 ******************" << endl;
    cout << "************** 7. 清空所有文档 ******************" << endl;
    cout << "*************************************************" << endl;

    return;
}

//批量添加职员
void WorkManger::add_manger()
{
    int info;
    string name;
    int duty;
    Work *worker = NULL;

    cout << "请输入将要添加的职工数目:" << endl;
    int mangernumber;
    cin >> mangernumber;

    //开辟新内存大小
    int newsize = this->m_MangerNumber + mangernumber;
    Work* *newspace = new Work*[newsize];

    //将原来的职工信息录入内存中
    if (this->m_ManArry != NULL)
    {
       for (int i = 0; i<m_MangerNumber; i++)
       {
           newspace[i] = m_ManArry[i];
       }
    }

   if (mangernumber > 0)
   {

    //将之后添加的信息录入内存中
    for (int j=0; j<mangernumber; j++)
    {
        cout << "第" << j+1 << "位职员信息录入" << endl;
        cout << "职员编号:" << endl;
        cin >> info;

        cout << "职员姓名:" << endl;
        cin >> name;

        cout << "请选择该职员的职位:" << endl;
        cout << "1. 普通职员" << endl;
        cout << "2. 经理" << endl;
        cout << "3. 老板" << endl;
        cin >> duty;

        switch (duty)
        {
            case 1: worker = new Employee(info, name, duty); break;
            case 2: worker = new Manager(info, name, duty); break;
            case 3: worker = new Boss(info, name, duty); break;
            default: break;
        }

        newspace[m_MangerNumber+j] =  worker;
    }

   //释放原来内存空间
   delete[] this->m_ManArry;

   //更新信息
   this->m_ManArry = newspace;
   this->m_MangerNumber = newsize;

   cout << "成功添加" << mangernumber << endl;
   this->IsEmpFile = false;
   }
   else
   {
       cout << "输入有误！" << endl;
   }
   
   //保存到文件中
   this->save();
   return;
}

//显示职员信息
void WorkManger::show_manger()
{
    if ( this->IsEmpFile == true )
    {
        cout << "文件不存在或记录为空！" << endl;
    }
    else
    {
        for (int i=0; i<this->m_MangerNumber; i++)
        {
             m_ManArry[i]->show_info();
        }
    }
    return;
}

//删除职员
void WorkManger::delete_manger()
{
    int index;
    int i = this->m_MangerNumber;
    int ii;
    int j;
    int k; //存在

    if ( this->IsEmpFile == true )
    {
        cout << "文件不存在或记录为空！" << endl;
    }

    cout << "请输入将要删除职员的编号：" << endl;
    cin >> index;

    for ( j=0; j<this->m_MangerNumber; j++)
    {
        if ( this->m_ManArry[j]->m_info == index )
        {
            k = 1;
            break;
        }
    }
    if ( j == this->m_MangerNumber )
    {
        k = 0;
    }

   if ( k == 1 )
   {
        for (i = 0; i < this->m_MangerNumber; i++)
			{
				if ( m_ManArry[i]->m_info == index)
                {
                    for ( ii=i; ii<this->m_MangerNumber-1; ii++)
                    {
                        m_ManArry[ii] = m_ManArry[ii+1];
                    }
                    break;
                }
			}

       cout << "删除成功！" << endl;
       --this->m_MangerNumber;
       save();//保存文件

   }
   else if ( k == 0 )
   {
       cout << "您要删除的职员不存在！" << endl;
   }

   system("pause");
   system("cls");
    return;
}

//查找职员信息
void WorkManger::search_manger()
{
    int i;
    int choice;
    int search_info;
    string search_name;

    if ( this->IsEmpFile == true )
    {
        cout << "文件不存在或记录为空！" << endl;
    }

    cout << "请选择查询方式：" << endl;
    cout << "1. 职员的编号" << endl;
    cout << "2. 职员的姓名" << endl;

    cin >> choice;

    if (choice == 1)
    {
        cout << "请输入您要查询的编号：" << endl;
        cin >> search_info;
        cout << "查询结果为：" << endl;

        for ( i=0; i<this->m_MangerNumber; i++)
        {
            if ( this->m_ManArry[i]->m_info == search_info )
            {
                m_ManArry[i]->show_info();
                break;
            }
        }
        if ( i == this->m_MangerNumber )
        {
            cout << "查询为空！" << endl;
        }
    }
    else if (choice == 2)
    {
       cout << "请输入您要查询的姓名 ：" << endl;
       cin >> search_name;
       cout << "查询结果为：" << endl;

       for ( i=0; i<this->m_MangerNumber; i++)
       {
           if ( this->m_ManArry[i]->m_name == search_name )
           {
                m_ManArry[i]->show_info();
                break;
           }
       }
       if ( i == this->m_MangerNumber )
       {
          cout << "查询为空！" << endl;
       }
    }
}

//修改职员信息
void WorkManger::chage_manger()
{
    int choice;
    int i;
    int k;

    int info;
    string name;
    int duty;

    if ( this->IsEmpFile )
    {
        cout << "文件不存在或记录为空！" << endl;
    }
    cout << "请输入带修改职员的编号：" << endl;
    cin >> choice;

    //判断编号是否存在。
    for ( i=0; i<this->m_MangerNumber; i++)
    {
        if ( this->m_ManArry[i]->m_info == choice )
        {
            cout << "请输入修改的信息：" << endl;
            cout << "编号：" << endl;
            cin >> info;
            cout << "姓名:" << endl;
            cin >> name;
            cout << "职位:" << endl;
            cout << "1. 普通员工" << endl;
            cout << "2. 经理" << endl;
            cout << "3. 老板" << endl;
            cin >> duty;

            this->m_ManArry[i]->m_info = info;
            this->m_ManArry[i]->m_name = name;
            this->m_ManArry[i]->m_duty = duty;

            save();//保存修改文件。
            cout << "该职员信息修改成功！" << endl;
            break;
            
        }
    }
    if ( i == this->m_MangerNumber )
    {
       cout << "您要修改的职员不存在！" << endl;
    }
    
    
    return;
}

//将职员按编号排序
void WorkManger::sort()
{
    int i, j;
    int choice;

    if ( this->IsEmpFile == true )
    {
        cout << "文件不存在或记录为空！" << endl;
    }

    cout << "请选择排序方式：" << endl;
    cout << "1. 编号从小到大" << endl;
    cout << "2. 编号从大到小" << endl;
    cin >> choice;

   if ( choice == 1 )
   {
       for (i=0; i<this->m_MangerNumber-1; i++)
       {
         for (j=0; j<this->m_MangerNumber-1-i; j++)
         {
             if ( m_ManArry[j]->m_info > m_ManArry[j+1]->m_info )
             {
                 m_ManArry[j]->m_info = m_ManArry[j]->m_info ^ m_ManArry[j+1]->m_info;
                 m_ManArry[j+1]->m_info = m_ManArry[j+1]->m_info ^ m_ManArry[j]->m_info;
                 m_ManArry[j]->m_info = m_ManArry[j]->m_info ^ m_ManArry[j+1]->m_info;

             }
         }
      }

   }
   else if ( choice ==2 )
   {
       for (i=0; i<this->m_MangerNumber-1; i++)
       {
         for (j=0; j<this->m_MangerNumber-1-i; j++)
         {
             if ( m_ManArry[j]->m_info < m_ManArry[j+1]->m_info )
             {
                 m_ManArry[j]->m_info = m_ManArry[j]->m_info ^ m_ManArry[j+1]->m_info;
                 m_ManArry[j+1]->m_info = m_ManArry[j+1]->m_info ^ m_ManArry[j]->m_info;
                 m_ManArry[j]->m_info = m_ManArry[j]->m_info ^ m_ManArry[j+1]->m_info;

             }
         }
      }

   }

   show_manger();
   return;
}

//清空所有文档
void WorkManger::clear()
{
    int choice;
    if ( this->IsEmpFile == true )
    {
        cout << "文件不存在或记录为空！" << endl;
    }

    cout << "您确定要清空吗？" << endl;
    cout << "1. 是" << endl;
    cout << "2.否" << endl;
    cin >> choice;

    if (choice == 1)
    {
        ofstream ofs(FILENAME, ios::trunc); //删除文件后创建。
        ofs.close();

        if ( this->m_ManArry != NULL )
        {
            for ( int i=0; i<this->m_MangerNumber; i++)
            {
                delete this->m_ManArry[i];
                this->m_ManArry[i] = NULL;
            }

            delete[] this->m_ManArry;
            this->m_MangerNumber = 0;
            this->m_ManArry = NULL;
            this->IsEmpFile = true;
        }

        cout << "清空成功！" << endl;
    }

    system("pause");
    system("cls");
    return;   
}

//退出
void WorkManger::Quit_System()
{
    cout << "您即将退出此程序..." << endl;
    cout << "欢迎下次使用，拜拜>_<..." << endl;
    system("pause");
    exit(0);

    return;
}

//保存职员信息
void WorkManger::save()
{
    ofstream ofs;

    ofs.open(FILENAME, ios::out);
    
    for (int i=0; i<this->m_MangerNumber; i++)
    {
        ofs << this->m_ManArry[i]->m_info << " "
            << this->m_ManArry[i]->m_name << " "
            << this->m_ManArry[i]->m_duty << endl;
    }

    ofs.close();

    return;
}

//取得职员数量
int WorkManger::get_manger_number()
{
    ifstream ifs;

    ifs.open(FILENAME, ios::in);

    int id;
    string name;
    int duty;

    int num = 0;
    while ( ifs >> id && ifs >> name && ifs >> id )
    {
        num++;
    }

    return num;
}

//初始化职员数组
void WorkManger::init_Emp()
{
    ifstream ifs;
    int id;
    string name;
    int duty;
    Work *worker = NULL;

    ifs.open(FILENAME, ios::in);

    for (int i=0; i<this->m_MangerNumber; i++)
    {
        ifs >> id;
        ifs >> name;
        ifs >> duty;

        if (duty == 1)
        {
            worker = new Employee(id, name, duty);
        }
        else if (duty == 2)
        {
            worker = new Manager(id, name, duty);
        }
        else if (duty == 3)
        {
            worker = new Boss(id, name, duty);
        }

        this->m_ManArry[i] = worker;
    }

    ifs.close();
}

