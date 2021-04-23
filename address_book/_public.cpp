#include "_public.h"

void HomePage(void)
{
        printf("***************************************\n");
        printf("************通讯录管理系统*************\n");
        printf("************ 1, 添加联系人 ************\n");
        printf("************ 2, 显示联系人 ************\n");
        printf("************ 3, 删除联系人 ************\n");
        printf("************ 4, 查找联系人 ************\n");
        printf("************ 5, 修改联系人 ************\n");
        printf("************ 6, 清空联系人 ************\n");
        printf("************ 0, 退出通讯录 ************\n");
        printf("***************************************\n");
        printf("请输入对应的数字：\n");
        return;
}

void AddContact(void)
{

        printf("***************************************\n");
        printf("添加联系人：\n");

        FILE* fp;
        char str[301];
        char ch;

        printf("请输入name, sex, age, address, address, Tel:\n");
        printf("例如：<id>1</id><name>zww</name><sex>女</sex><age>22</age><address>河南</address><tel>18379199173</tel>\n");

        memset(str, 0, sizeof(str));
        scanf("%s", str);
        strcat(str, "\n");
        if ( (fp=fopen("./contact.txt", "a+")) == NULL)
        {
                printf("文件打开失败!程序即将退出!\n");
                exit(-1);
        }
        fprintf(fp, str);
        fclose(fp);
        printf("添加联系人完毕！\n");
       // HomePage();
        return;
}

void ShowContact(void)
{
        struct st_Contact contact;
        vector<st_Contact> vcontact;
        vcontact.clear();

        FILE* fp;
        char str[301];
        int ii = 0;

        if ( (fp=fopen("./contact.txt", "r")) == 0)
        {
                printf("文件打开失败！\n");
                exit(-1);
        }

        while (true)
        {
                memset(str, 0, sizeof(str));
                memset(&contact, 0, sizeof(struct st_Contact));
                if ( fgets(str, sizeof(str), fp) == 0 ) break;
                GetXMLBuffer(str, "id", contact.id);
                GetXMLBuffer(str, "name", contact.name);
                GetXMLBuffer(str, "sex", contact.sex);
                GetXMLBuffer(str, "age", contact.age);
                GetXMLBuffer(str, "address", contact.address);
                GetXMLBuffer(str, "tel", contact.Tel);

                vcontact.push_back(contact);

                // printf("%d:姓名：%s     年龄：%s        性别：%s        住址：%s        联系电话：%s\n", \
                // contact.id, contact.name, contact.age, contact.sex, contact.address, contact.Tel);
        }

        fclose(fp);
        fp = NULL;

        for (int ii=0; ii<vcontact.size(); ii++)
        {

                printf("%s:姓名：%s\t年龄：%s\t性别：%s\t住址：%s\t联系电话：%s\n", \
                vcontact[ii].id, vcontact[ii].name, vcontact[ii].age, vcontact[ii].sex, vcontact[ii].address, vcontact[ii].Tel);
        }
        
        printf("显示完毕！\n");
       // HomePage();

        return;
}

void DelectContact(void)
{
        printf("请输入要删除的名字：");
        char strname[10];
        memset(strname, 0, sizeof(strname));
        scanf("%s", strname);
        
        int ii;
        struct st_Contact contact;
        vector<st_Contact> vcontact;
        vcontact.clear();


        FILE* fp;
        char str[301];
        char strcontact[301];
        char strstr[301];
        memset(strcontact, 0, sizeof(strcontact));

        if ( (fp=fopen("./contact.txt", "r")) == 0)
        {
                printf("文件打开失败！\n");
                exit(-1);
        }

        while (true)
        {
                memset(str, 0, sizeof(str));
                memset(&contact, 0, sizeof(struct st_Contact));
                if ( fgets(str, sizeof(str), fp) == 0 ) break;
                GetXMLBuffer(str, "id", contact.id);
                GetXMLBuffer(str, "name", contact.name);
                GetXMLBuffer(str, "sex", contact.sex);
                GetXMLBuffer(str, "age", contact.age);
                GetXMLBuffer(str, "address", contact.address);
                GetXMLBuffer(str, "tel", contact.Tel);

                if (strcmp(contact.name, strname) == 0)
                {
                        continue;   
                }
                vcontact.push_back(contact);
        }
        fclose(fp);
        fp = NULL;
    
        FILE* fpp;
        if ( (fpp=fopen("./contact.txt", "w")) == 0)
        {
                printf("文件打开失败！\n");
                exit(-1);
        }

        //写入文件
        for (ii=0; ii<vcontact.size(); ii++)
        {
                memset(strstr, 0, sizeof(strstr));
                sprintf(strstr, "<id>%s</id><name>%s</name><sex>%s</sex><age>%s</age><address>%s</address><tel>%s</tel>\n", \
                vcontact[ii].id, vcontact[ii].name, vcontact[ii].sex, vcontact[ii].age, vcontact[ii].address, vcontact[ii].Tel);
                fprintf(fpp, strstr);
        }

        //关闭文件
        fclose(fpp);
        fpp = NULL;
        printf("删除完毕！\n");

        return;
}

void SeekContact(void)
{
        printf("请输入查询的名字：");
        char strname[10];
        memset(strname, 0, sizeof(strname));
        scanf("%s", strname);
        
        int ii;
        struct st_Contact contact;
        vector<st_Contact> vcontact;
        vcontact.clear();

        FILE* fp;
        char str[301];

        if ( (fp=fopen("./contact.txt", "r")) == 0)
        {
                printf("文件打开失败！\n");
                exit(-1);
        }

        while (true)
        {
                memset(str, 0, sizeof(str));
                memset(&contact, 0, sizeof(struct st_Contact));
                if ( fgets(str, sizeof(str), fp) == 0 ) break;
                GetXMLBuffer(str, "id", contact.id);
                GetXMLBuffer(str, "name", contact.name);
                GetXMLBuffer(str, "sex", contact.sex);
                GetXMLBuffer(str, "age", contact.age);
                GetXMLBuffer(str, "address", contact.address);
                GetXMLBuffer(str, "tel", contact.Tel);

                vcontact.push_back(contact);
                if (strcmp(contact.name, strname) == 0)
                {
                        printf("查询结果：\n");
                        printf("%s:姓名：%s     年龄：%s        性别：%s        住址：%s        联系电话：%s\n", \
                contact.id, contact.name, contact.age, contact.sex, contact.address, contact.Tel);
                }
        }

        // for (ii=0; ii<vcontact.size(); ii++)
        // {
        //         printf("%s\n", vcontact[ii].name);

        //         if (strcmp(vcontact[ii].name, strname) == 0)
        //         {
        //                 printf("查询结果：\n");
        //                 printf("%d:姓名：%s     年龄：%s        性别：%s        住址：%s        联系电话：%s\n", \
        //         vcontact[ii].id, vcontact[ii].name, vcontact[ii].age, vcontact[ii].sex, vcontact[ii].address, vcontact[ii].Tel);
        //         }

        // }
        fclose(fp);
        fp = NULL;
        return;
}

void UpdateContact(void)
{
        printf("请输入要更新的名字：");
        char strname[10];
        memset(strname, 0, sizeof(strname));
        scanf("%s", strname);
        
        int ii;
        struct st_Contact contact;
        vector<st_Contact> vcontact;
        vcontact.clear();


        FILE* fp;
        char str[301];
        char strcontact[301];
        char strstr[301];
        memset(strcontact, 0, sizeof(strcontact));

        if ( (fp=fopen("./contact.txt", "r")) == 0)
        {
                printf("文件打开失败！\n");
                exit(-1);
        }

        while (true)
        {
                memset(str, 0, sizeof(str));
                memset(&contact, 0, sizeof(struct st_Contact));
                if ( fgets(str, sizeof(str), fp) == 0 ) break;
                GetXMLBuffer(str, "id", contact.id);
                GetXMLBuffer(str, "name", contact.name);
                GetXMLBuffer(str, "sex", contact.sex);
                GetXMLBuffer(str, "age", contact.age);
                GetXMLBuffer(str, "address", contact.address);
                GetXMLBuffer(str, "tel", contact.Tel);

                if (strcmp(contact.name, strname) == 0)
                {
                        printf("请输入要更改的完整信息：");
                        printf("id:");
                        scanf("%s", contact.id);
                        printf("name:");
                        scanf("%s", contact.name);
                        printf("age:");
                        scanf("%s", contact.age);
                        printf("sex:");
                        scanf("%s", contact.sex);
                        printf("address:");
                        scanf("%s", contact.address);
                        printf("Tel:");
                        scanf("%s", contact.Tel);
                        
                }
                vcontact.push_back(contact);
        }
        fclose(fp);
        fp = NULL;
    
        FILE* fpp;
        if ( (fpp=fopen("./contact.txt", "w")) == 0)
        {
                printf("文件打开失败！\n");
                exit(-1);
        }

        //写入文件
        for (ii=0; ii<vcontact.size(); ii++)
        {
                memset(strstr, 0, sizeof(strstr));
                sprintf(strstr, "<id>%s</id><name>%s</name><sex>%s</sex><age>%s</age><address>%s</address><tel>%s</tel>\n", \
                vcontact[ii].id, vcontact[ii].name, vcontact[ii].sex, vcontact[ii].age, vcontact[ii].address, vcontact[ii].Tel);
                fprintf(fpp, strstr);
        }
        //关闭文件
        fclose(fpp);
        fpp = NULL;
        printf("更新完毕！\n");
        return;
}

void ClearContact(void)
{
        FILE* fp;

        printf("您选择了清空通讯录的危险操作，您确定执行吗？确定请输入yes.\n");
        char str[10];
        memset(str, 0, sizeof(str));
        scanf("%s", str);

        if ( strcmp(str, "yes") == 0)
        {
                if ((fp=fopen("./contact.txt", "w")) == 0 )
                {
                        printf("文件打开失败！");
                        exit(-1);
                }

                fclose(fp);
                fp = NULL;
                printf("清空通讯录成功！\n");
                //HomePage();
        }

        return;
}

void QuitContact(void)
{
        printf("程序即将退出...\n");
        Sleep(300);
        exit(1);
        return;
}


// 操作XMLBuffer的函数
// in_XMLBuffer，XML格式的字符串
// in_FieldName，字段的标签名
// out_Value，获取内容存放的变量的指针
bool GetXMLBuffer(const char *in_XMLBuffer,const char *in_FieldName,char *out_Value,const int in_Len)
{
  strcpy(out_Value,"");

  char *start=NULL,*end=NULL;
  char m_SFieldName[51],m_EFieldName[51];

  int m_NameLen = strlen(in_FieldName);
  memset(m_SFieldName,0,sizeof(m_SFieldName));
  memset(m_EFieldName,0,sizeof(m_EFieldName));

  snprintf(m_SFieldName,50,"<%s>",in_FieldName);
  snprintf(m_EFieldName,50,"</%s>",in_FieldName);

  start=0; end=0;

  start = (char *)strstr(in_XMLBuffer,m_SFieldName);

  if (start != 0)
  {
    end   = (char *)strstr(start,m_EFieldName);
  }

  if ((start==0) || (end == 0))
  {
    return false;
  }

  int   m_ValueLen = end - start - m_NameLen - 2 + 1 ;

  if ( ((m_ValueLen-1) <= in_Len) || (in_Len == 0) )
  {
    strncpy(out_Value,start+m_NameLen+2,m_ValueLen-1); out_Value[m_ValueLen-1]=0;
  }
  else
  {
    strncpy(out_Value,start+m_NameLen+2,in_Len); out_Value[in_Len]=0;
  }

  //DeleteLRChar(out_Value,' ');
  return true;
}

