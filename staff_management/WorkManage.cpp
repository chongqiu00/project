#include "WorkManage.h"

WorkManger::WorkManger()
{
    ifstream ifs;
    int num;
    char ch;

    //1.�ļ�������
    ifs.open(FILENAME, ios::in); //���ļ�

    if ( !ifs.is_open() )
    {
        this->m_MangerNumber = 0;
        this->m_ManArry = NULL;
        this->IsEmpFile = true;
        ifs.close();
        return;
    }

    //2.�ļ����� ����Ϊ��
    ifs >> ch;

    if (ifs.eof())
    {
        this->m_MangerNumber = 0;
        this->m_ManArry = NULL;
        this->IsEmpFile = true;
        ifs.close();
        return;
    }

    //3.�ļ����ڣ��Ҽ�¼���ݡ�
    num = this->get_manger_number();
    this->m_MangerNumber = num;
    cout << "�ù�˾����" << num << "��ְԱ��" << endl;
    this->m_ManArry = new Work* [this->m_MangerNumber]; 
    this->init_Emp(); //��ʼ�����顣
    this->IsEmpFile = false; //���ñ�Ϊ�ա�
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


//��ʾ
void WorkManger::Show_Menu()
{
    cout << "*************************************************" << endl;
    cout << "***************��ӭʹ��ְ������ϵͳ**************" << endl;
    cout << "************** 0. �˳�����ϵͳ ******************" << endl;
    cout << "************** 1. ����ְ����Ϣ ******************" << endl;
    cout << "************** 2. ��ʾְ����Ϣ ******************" << endl;
    cout << "************** 3. ɾ����ְְ�� ******************" << endl;
    cout << "************** 4. �޸�ְ����Ϣ ******************" << endl;
    cout << "************** 5. ����ְ����Ϣ ******************" << endl;
    cout << "************** 6. ���ձ������ ******************" << endl;
    cout << "************** 7. ��������ĵ� ******************" << endl;
    cout << "*************************************************" << endl;

    return;
}

//�������ְԱ
void WorkManger::add_manger()
{
    int info;
    string name;
    int duty;
    Work *worker = NULL;

    cout << "�����뽫Ҫ��ӵ�ְ����Ŀ:" << endl;
    int mangernumber;
    cin >> mangernumber;

    //�������ڴ��С
    int newsize = this->m_MangerNumber + mangernumber;
    Work* *newspace = new Work*[newsize];

    //��ԭ����ְ����Ϣ¼���ڴ���
    if (this->m_ManArry != NULL)
    {
       for (int i = 0; i<m_MangerNumber; i++)
       {
           newspace[i] = m_ManArry[i];
       }
    }

   if (mangernumber > 0)
   {

    //��֮����ӵ���Ϣ¼���ڴ���
    for (int j=0; j<mangernumber; j++)
    {
        cout << "��" << j+1 << "λְԱ��Ϣ¼��" << endl;
        cout << "ְԱ���:" << endl;
        cin >> info;

        cout << "ְԱ����:" << endl;
        cin >> name;

        cout << "��ѡ���ְԱ��ְλ:" << endl;
        cout << "1. ��ְͨԱ" << endl;
        cout << "2. ����" << endl;
        cout << "3. �ϰ�" << endl;
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

   //�ͷ�ԭ���ڴ�ռ�
   delete[] this->m_ManArry;

   //������Ϣ
   this->m_ManArry = newspace;
   this->m_MangerNumber = newsize;

   cout << "�ɹ����" << mangernumber << endl;
   this->IsEmpFile = false;
   }
   else
   {
       cout << "��������" << endl;
   }
   
   //���浽�ļ���
   this->save();
   return;
}

//��ʾְԱ��Ϣ
void WorkManger::show_manger()
{
    if ( this->IsEmpFile == true )
    {
        cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
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

//ɾ��ְԱ
void WorkManger::delete_manger()
{
    int index;
    int i = this->m_MangerNumber;
    int ii;
    int j;
    int k; //����

    if ( this->IsEmpFile == true )
    {
        cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
    }

    cout << "�����뽫Ҫɾ��ְԱ�ı�ţ�" << endl;
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

       cout << "ɾ���ɹ���" << endl;
       --this->m_MangerNumber;
       save();//�����ļ�

   }
   else if ( k == 0 )
   {
       cout << "��Ҫɾ����ְԱ�����ڣ�" << endl;
   }

   system("pause");
   system("cls");
    return;
}

//����ְԱ��Ϣ
void WorkManger::search_manger()
{
    int i;
    int choice;
    int search_info;
    string search_name;

    if ( this->IsEmpFile == true )
    {
        cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
    }

    cout << "��ѡ���ѯ��ʽ��" << endl;
    cout << "1. ְԱ�ı��" << endl;
    cout << "2. ְԱ������" << endl;

    cin >> choice;

    if (choice == 1)
    {
        cout << "��������Ҫ��ѯ�ı�ţ�" << endl;
        cin >> search_info;
        cout << "��ѯ���Ϊ��" << endl;

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
            cout << "��ѯΪ�գ�" << endl;
        }
    }
    else if (choice == 2)
    {
       cout << "��������Ҫ��ѯ������ ��" << endl;
       cin >> search_name;
       cout << "��ѯ���Ϊ��" << endl;

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
          cout << "��ѯΪ�գ�" << endl;
       }
    }
}

//�޸�ְԱ��Ϣ
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
        cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
    }
    cout << "��������޸�ְԱ�ı�ţ�" << endl;
    cin >> choice;

    //�жϱ���Ƿ���ڡ�
    for ( i=0; i<this->m_MangerNumber; i++)
    {
        if ( this->m_ManArry[i]->m_info == choice )
        {
            cout << "�������޸ĵ���Ϣ��" << endl;
            cout << "��ţ�" << endl;
            cin >> info;
            cout << "����:" << endl;
            cin >> name;
            cout << "ְλ:" << endl;
            cout << "1. ��ͨԱ��" << endl;
            cout << "2. ����" << endl;
            cout << "3. �ϰ�" << endl;
            cin >> duty;

            this->m_ManArry[i]->m_info = info;
            this->m_ManArry[i]->m_name = name;
            this->m_ManArry[i]->m_duty = duty;

            save();//�����޸��ļ���
            cout << "��ְԱ��Ϣ�޸ĳɹ���" << endl;
            break;
            
        }
    }
    if ( i == this->m_MangerNumber )
    {
       cout << "��Ҫ�޸ĵ�ְԱ�����ڣ�" << endl;
    }
    
    
    return;
}

//��ְԱ���������
void WorkManger::sort()
{
    int i, j;
    int choice;

    if ( this->IsEmpFile == true )
    {
        cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
    }

    cout << "��ѡ������ʽ��" << endl;
    cout << "1. ��Ŵ�С����" << endl;
    cout << "2. ��ŴӴ�С" << endl;
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

//��������ĵ�
void WorkManger::clear()
{
    int choice;
    if ( this->IsEmpFile == true )
    {
        cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
    }

    cout << "��ȷ��Ҫ�����" << endl;
    cout << "1. ��" << endl;
    cout << "2.��" << endl;
    cin >> choice;

    if (choice == 1)
    {
        ofstream ofs(FILENAME, ios::trunc); //ɾ���ļ��󴴽���
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

        cout << "��ճɹ���" << endl;
    }

    system("pause");
    system("cls");
    return;   
}

//�˳�
void WorkManger::Quit_System()
{
    cout << "�������˳��˳���..." << endl;
    cout << "��ӭ�´�ʹ�ã��ݰ�>_<..." << endl;
    system("pause");
    exit(0);

    return;
}

//����ְԱ��Ϣ
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

//ȡ��ְԱ����
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

//��ʼ��ְԱ����
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

