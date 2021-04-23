/*
文件操作，存入文本文件。
编辑人：chongqiu00
时间：2021、4、4
*/


#include "_public.h"

int main (int argc, char* argv[])
{
        //页面
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
                        default: printf("您输入的选项非法程序即将退出！\n");break;
                }
        }

        return 0;
}