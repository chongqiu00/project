#include "WorkManage.h"

int main(int argc, char* argv[])
{
    WorkManger wm;
    int choice;

    while (1)
    {
        wm.Show_Menu();
        cout << "ÇëÊäÈëÄúµÄÑ¡Ôñ£º" << endl;
        cin >> choice;
        switch (choice)
        {
            case 0: 
            {
                wm.Quit_System(); 
                break;
            } 
            case 1:
            {
                wm.add_manger();
                break;
            }
            case 2:
            {
                wm.show_manger();
                break;
            }
            case 3:
            {
                wm.delete_manger();
                break;
            }
            case 4:
            {
                wm.chage_manger();
                break;
            }
            case 5:
            {
                wm.search_manger();
                break;
            }
            case 6:
            {
                wm.sort();
                break;
            }
            case 7:
            {
                wm.clear();
                break;
            }
            default:
            {
                system("clr");
                break;
            }
        }
        
        system("pause");
        system("cls");
    }
    return 0;
}