#include "./inc/color.h"
#include "./inc/ts.h"
#include "./inc/bmp.h"
#include "./inc/lcd.h"
#include "./inc/camera.h"

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> //system

 int photo_flag = 0;
 int record_state = 0;
 int play_done = 0;
 int path = 0;
 int x,y,commd;
 int delete_num = 0; //鐓х墖琚垹鏁�
 int video_num = 0; 
 int ernum[4] = {5,5,5,5};
 extern int camera_pic_num;
 extern int record_pic_num;
pthread_t  pt_camera;
pthread_t  pt_record;
char *BMP_PATH[] = {"./pic/202_1.bmp","./pic/202_2.bmp","./pic/202_3.bmp","./pic/202_4.bmp"}; //瀛樻斁鍦板潃

/////////////////////
int start_show_ui(int* lcd_fd)
{
  lcd_draw_jpeg(lcd_fd, 0, 0, NULL, 500*480, "start_up.jpg", 1, 1); /* 寮€鏈烘彁绀虹晫闈� */
  sleep(1);
  srand(time(NULL));
  int x, y, r;

  for ( int i=0; i<50; i++) /* 婊戜笅褰╄櫣鐞� */
  {
    x = rand() % 700;
    y = rand() % 380;
    r = rand() % 20;
    lcd_circle(lcd_fd, 0xff0000, x, y, r);
    lcd_circle(lcd_fd, 0xff7f00, x+10, y+10, r+3);
    lcd_circle(lcd_fd, 0xfffff00, x+20, y+20, r+4);
    lcd_circle(lcd_fd, 0xff00, x+40, y+40, r+5);
    lcd_circle(lcd_fd, 0xffff, x+60, y+60, r+6);
    lcd_circle(lcd_fd, 0xff, x+80, y+80, r+7);
    lcd_circle(lcd_fd, 0x8b00ff, x+100, y+100, r+8);
  }

  printf("寮€鏈猴紒\n");

  return 0;
}
/////////////////////////////////////////////

int address_add(int *address,int MAX,int MIN)
{
	
	while(1)
	{
		if(*address == MAX)
		{
			*address = MIN;
		}
		else (*address)++;
		if((*address != ernum[0])&&(*address !=ernum[1])&&(*address != ernum[2])&&(*address != ernum[3])) 
		{
			break;
		}
	}
	
}
int address_sub(int *address,int MAX,int MIN)
{
	while(1)
	{
		if(*address == MIN)
		{
			*address = MAX;
		}
		else (*address)--;
		if((*address != ernum[0])&&(*address !=ernum[1])&&(*address != ernum[2])&&(*address != ernum[3]))  
		{
			break;
		}
	}
	
}
void album(void)
{
	show_pic();
	while(1)
	{
		if(delete_num == 4)//鏄剧ず绌虹晫闈�
		{
			nopic();
		}
		else{
			show_bmp(BMP_PATH[path],30,72, 560, 332,NORMAL,BNULL);
		}
			   //妫€娴嬫寜閿�
		get_ts_xy(&x,&y);
		//鍒ゆ柇4寮犵収鐗囨槸鍚﹀叏鍒犲畬浜�
		if(delete_num < 4)  {  //杩樺墿浣欑収鐗�
			//涓婁笅锛岄€€鍑�
			if(x>=623 && x<=623+180 && y>=0&&y<120)  //涓婁竴寮�
			{
				address_add(&path,3,0);
				album_b1();
				usleep(300000);
				album_refresh();
				show_bmp(BMP_PATH[path], 30,72,560,336,NORMAL,BNULL);
			}
			if(x>=623 && x<=623+180 && y>=120&&y<120*2)//涓嬩竴寮�
			{
				address_sub(&path,3,0);
				album_b2();
				usleep(300000);
				album_refresh();
				show_bmp(BMP_PATH[path], 30,72, 560, 336,NORMAL,BNULL);
			}
			if(x>=623 && x<=623+180 && y>=120*2&&y<120*3)//鍒犻櫎
			{
				album_b3(); 
				//鍐嶆璇㈤棶*****************************************************************
				album_ask();
				album_refresh();
				printf("Are you sure?\n");				
				while(1)
				{
					//鎸夐敭妫€娴�
					get_ts_xy(&x,&y);
					//鍒犻櫎
					if(x>310 && x<=620 && y>=0&&y<480)
					{
						printf("ok\n");
						// 绉昏繘鍨冨溇妗�
						char buf1[]={"mv "};
						char buf[64]={0};
						sprintf(buf, "%s%s /IOT/sys1/trush/",buf1,BMP_PATH[path]);
						system(buf);
						//鏍囪鏀瑰彉
						delete_num++;
						ernum[delete_num - 1] = path;
						if(delete_num == 4){
							//鏄剧ず鏃犵収鐗�
							nopic() ;
							break;
						}
						else{
							address_add(&path,3,0);
							//鏄剧ず涓嬩竴寮�
							show_pic();
							show_bmp(BMP_PATH[path], 30,72, 560, 336,NORMAL,BNULL);
							break;
						}}
					//鍙栨秷鍒犻櫎
					else if(x>=0 && x<=310 && y>=0&&y<480)
					{
						//鏄剧ず鐓х墖
						printf("Cannel\n");
						show_bmp(BMP_PATH[path], 30,72, 560, 336,NORMAL,BNULL);
						break;
					}}}}//閫€鍑�
		if(x>=623 && x<=623+180 && y>=120*3&&y<120*4){
				album_b4();
				usleep(300000);
				show_first();
				break;
			}}}

int main(int argc, char const *argv[])
{

  //==========鍑芥暟鍙橀噺鍒濆鍖�==========//
  //
  int lcd_fd;
  init_lcd(&lcd_fd);
  show_ui_start(&lcd_fd);
  uinit_lcd(&lcd_fd);
  //
   int camera_state = 0;
   int video_state = 0;
   
   int i;
   
  //==========鍔熻兘鍒濆鍖�==========// 
   open_ts();
   open_lcd_bmp();
   show_first();
   system("rm ./*.jpg");
   system("rm ./record/*.jpg");
   system("rm ./1.avi");
   system("mv ./trush/*.bmp /IOT/sys1/pic/");
   printf("鍒濆鍖栧畬姣昞n");
   //lcd_draw_jpg_camera(0,0,"./1.jpg") ;

   while(1)
   {
	   if(play_done == 1)
	   {
		   show_first();
		   play_done = 0;
	   }
	   get_ts_xy(&x,&y);
	   if( x>=623 && x<=623+180 && y>=0&&y<0+80)//鎵撳紑鎽勫儚澶�
	   {
		   if(camera_state == 0)
		   {
				camera_state = 1;
				open_camera("/dev/video7");
				pthread_create(&pt_camera, NULL, camera_working_pthread, NULL);	
				////////////////////////////////////////////////
		   }
		   
		   
	   }
	    if( x>=623 && x<=623+180 && y>=80&&y<80+80)  //鍏抽棴鎽勫儚澶�
	   {
		   if(camera_state == 1)
		   {
			    pthread_cancel(pt_camera);	//鍙栨秷璇ョ嚎绋�
				usleep(200000);
				close_camera();				//鍏抽棴鎽勫儚澶存枃浠�
				usleep(200000);
				show_first();
				camera_state = 0;
		   }
		   else
		   {
			  printf("======鏈墦寮€鎽勫儚澶�======"); 
		   }
			
	   }
	   if( x>=623 && x<=623+180 && y>=80*3&&y<80*3+80)  //鎷嶇収
	   {
		   if(camera_state == 0)    //鎻愮ず鎵撳紑鎽勫儚澶�
		   {
			 printf("======鏈墦寮€鎽勫儚澶�======");   
		   }
		   else
		   {
				photo_flag = 1;
				printf("======鎷嶇収======");
		   }
		   
	   }
	   if( x>=623 && x<=623+180 && y>=80*4&&y<80*4+80)  //褰曞睆
	   {
		   if(camera_state == 0)    //鎻愮ず鎵撳紑鎽勫儚澶�
		   {
			  printf("======璇锋墦寮€鎽勫儚澶�======\n"); 
		   }
		   else
		   {
			   
			   if(record_state == 0)
			   {
					record_state = 1;
					printf("======寮€濮嬪綍鍒�======");
					//pthread_create(&pt_record, NULL,recording_pthread, NULL);
							//鍥剧墖缃崲
							show_recording();
							//妫€娴嬫寜閿�   2
								get_ts_xy(&x,&y);
								if(x>=623 && x<=623+180 && y>=80*4&&y<80*5)//鍋滄褰曞儚
								{
									char buff1[64] = {"/bin/ffmpeg -f image2 -i ./record/""};
									record_state = 0;
									refresh();
									video_num++; 
									system("ffmpeg -f image2 -i ./record/%d.jpg -r 1 ./1.avi &");
									sleep(6);
									 //ffmpeg -f image2 -i ./%d.jpg -r 4 ./1.avi 
									system("killall -9 ffmpeg");
									printf("缁撴潫褰曞埗\n");
								}
							
				   
			   }
			  
		   }
	   }
	   if( x>=623 && x<=623+180 && y>=80*5&&y<80*5+80)  //鎵撳紑瑙嗛
	   {
		   if(camera_state == 1)
		   {
			   printf("璇峰叧闂憚鍍忓ご\n");
		   }
		   else 
		   {
			   //榛戝睆
			    system("/bin/mplayer -slave -quiet -geometry 30:72 -zoom -x 560 -y 336 1.avi ");
				sleep(1);
			    play_done = 1;
				
		   }
	   }
	    if( x>=623 && x<=623+180 && y>=80*2&&y<80*2+80)   //鐩稿唽
	   {
		   if(camera_state == 1)
		   {
			   printf("璇峰叧闂憚鍍忓ご\n");
		   }
		   else
		   {
				//path = 0;
				//鏄剧ず鐩稿唽鐣岄潰
				album();
		   }
		   
		   
	   }
     
   }
   
}



