/*
�ļ������������ı��ļ���
�༭�ˣ�chongqiu00
ʱ�䣺2021��4��4
*/


#include "_public.h"

int main (int argc, char* argv[])
{
        //ҳ��
        HomePage();

        int select;

        while ( scanf("%d", &select) == 1 )
        {
                switch(select)
                {
                        case 1: AddContact(); break;
                        case 2: ShowContact(); break;
                        case 3: DelectContact(); break;
                        case 4: SeekContact(); break;
                        case 5: UpdateContact(); break;
                        case 6: ClearContact(); break;
                        case 0: QuitContact(); break;
                        default: printf("�������ѡ��Ƿ����򼴽��˳���\n");break;
                }
        }

        return 0;
}