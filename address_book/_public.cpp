#include "_public.h"

void HomePage(void)
{
        printf("***************************************\n");
        printf("************ͨѶ¼����ϵͳ*************\n");
        printf("************ 1, �����ϵ�� ************\n");
        printf("************ 2, ��ʾ��ϵ�� ************\n");
        printf("************ 3, ɾ����ϵ�� ************\n");
        printf("************ 4, ������ϵ�� ************\n");
        printf("************ 5, �޸���ϵ�� ************\n");
        printf("************ 6, �����ϵ�� ************\n");
        printf("************ 0, �˳�ͨѶ¼ ************\n");
        printf("***************************************\n");
        printf("�������Ӧ�����֣�\n");
        return;
}

void AddContact(void)
{

        printf("***************************************\n");
        printf("�����ϵ�ˣ�\n");

        FILE* fp;
        char str[301];
        char ch;

        printf("������name, sex, age, address, address, Tel:\n");
        printf("���磺<id>1</id><name>zww</name><sex>Ů</sex><age>22</age><address>����</address><tel>18379199173</tel>\n");

        memset(str, 0, sizeof(str));
        scanf("%s", str);
        strcat(str, "\n");
        if ( (fp=fopen("./contact.txt", "a+")) == NULL)
        {
                printf("�ļ���ʧ��!���򼴽��˳�!\n");
                exit(-1);
        }
        fprintf(fp, str);
        fclose(fp);
        printf("�����ϵ����ϣ�\n");
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
                printf("�ļ���ʧ�ܣ�\n");
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

                // printf("%d:������%s     ���䣺%s        �Ա�%s        סַ��%s        ��ϵ�绰��%s\n", \
                // contact.id, contact.name, contact.age, contact.sex, contact.address, contact.Tel);
        }

        fclose(fp);
        fp = NULL;

        for (int ii=0; ii<vcontact.size(); ii++)
        {

                printf("%s:������%s\t���䣺%s\t�Ա�%s\tסַ��%s\t��ϵ�绰��%s\n", \
                vcontact[ii].id, vcontact[ii].name, vcontact[ii].age, vcontact[ii].sex, vcontact[ii].address, vcontact[ii].Tel);
        }
        
        printf("��ʾ��ϣ�\n");
       // HomePage();

        return;
}

void DelectContact(void)
{
        printf("������Ҫɾ�������֣�");
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
                printf("�ļ���ʧ�ܣ�\n");
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
                printf("�ļ���ʧ�ܣ�\n");
                exit(-1);
        }

        //д���ļ�
        for (ii=0; ii<vcontact.size(); ii++)
        {
                memset(strstr, 0, sizeof(strstr));
                sprintf(strstr, "<id>%s</id><name>%s</name><sex>%s</sex><age>%s</age><address>%s</address><tel>%s</tel>\n", \
                vcontact[ii].id, vcontact[ii].name, vcontact[ii].sex, vcontact[ii].age, vcontact[ii].address, vcontact[ii].Tel);
                fprintf(fpp, strstr);
        }

        //�ر��ļ�
        fclose(fpp);
        fpp = NULL;
        printf("ɾ����ϣ�\n");

        return;
}

void SeekContact(void)
{
        printf("�������ѯ�����֣�");
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
                printf("�ļ���ʧ�ܣ�\n");
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
                        printf("��ѯ�����\n");
                        printf("%s:������%s     ���䣺%s        �Ա�%s        סַ��%s        ��ϵ�绰��%s\n", \
                contact.id, contact.name, contact.age, contact.sex, contact.address, contact.Tel);
                }
        }

        // for (ii=0; ii<vcontact.size(); ii++)
        // {
        //         printf("%s\n", vcontact[ii].name);

        //         if (strcmp(vcontact[ii].name, strname) == 0)
        //         {
        //                 printf("��ѯ�����\n");
        //                 printf("%d:������%s     ���䣺%s        �Ա�%s        סַ��%s        ��ϵ�绰��%s\n", \
        //         vcontact[ii].id, vcontact[ii].name, vcontact[ii].age, vcontact[ii].sex, vcontact[ii].address, vcontact[ii].Tel);
        //         }

        // }
        fclose(fp);
        fp = NULL;
        return;
}

void UpdateContact(void)
{
        printf("������Ҫ���µ����֣�");
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
                printf("�ļ���ʧ�ܣ�\n");
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
                        printf("������Ҫ���ĵ�������Ϣ��");
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
                printf("�ļ���ʧ�ܣ�\n");
                exit(-1);
        }

        //д���ļ�
        for (ii=0; ii<vcontact.size(); ii++)
        {
                memset(strstr, 0, sizeof(strstr));
                sprintf(strstr, "<id>%s</id><name>%s</name><sex>%s</sex><age>%s</age><address>%s</address><tel>%s</tel>\n", \
                vcontact[ii].id, vcontact[ii].name, vcontact[ii].sex, vcontact[ii].age, vcontact[ii].address, vcontact[ii].Tel);
                fprintf(fpp, strstr);
        }
        //�ر��ļ�
        fclose(fpp);
        fpp = NULL;
        printf("������ϣ�\n");
        return;
}

void ClearContact(void)
{
        FILE* fp;

        printf("��ѡ�������ͨѶ¼��Σ�ղ�������ȷ��ִ����ȷ��������yes.\n");
        char str[10];
        memset(str, 0, sizeof(str));
        scanf("%s", str);

        if ( strcmp(str, "yes") == 0)
        {
                if ((fp=fopen("./contact.txt", "w")) == 0 )
                {
                        printf("�ļ���ʧ�ܣ�");
                        exit(-1);
                }

                fclose(fp);
                fp = NULL;
                printf("���ͨѶ¼�ɹ���\n");
                //HomePage();
        }

        return;
}

void QuitContact(void)
{
        printf("���򼴽��˳�...\n");
        Sleep(300);
        exit(1);
        return;
}


// ����XMLBuffer�ĺ���
// in_XMLBuffer��XML��ʽ���ַ���
// in_FieldName���ֶεı�ǩ��
// out_Value����ȡ���ݴ�ŵı�����ָ��
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

