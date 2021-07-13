#ifndef _BMP_H
#define _BMP_H

//printf、scanf函数
#include <stdio.h>
//open、lseek函数
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//read、close、write、lseek函数
#include <unistd.h>
//mmap函数
#include <sys/mman.h>

#define NORMAL 0
#define CIRCLE 1
#define BNULL   0
#define BUTTON 1


//1、打开lcd屏幕文件、申请映射内存
int open_lcd_bmp(void);

//2、显示任意位置大小bmp图片
int show_bmp(const char *bmp_path, int lcd_x, int lcd_y, int bmp_x, int bmp_y,int effect,int button); //长宽必须是4的倍数，且不能超出屏幕显示范围      

//3、关闭lcd屏幕文件、释放映射内存
int close_lcd_bmp(void);
//
void bmp_lcd(const char *bmp_path);
int draw_button(int w,int h,int x1,int y1,int a[],int n);
int open_bmp(const char *bmp_path,char a[],int bmp_x ,int bmp_y);
void Gray(char org[],int bmp_x ,int bmp_y);
void extend(int goal[],char org[],int bmp_x ,int bmp_y);
void circle_appear(int data[],int bmp_x, int bmp_y);
void normal_appear(int data[],int bmp_x, int bmp_y);
int show_first(void);
int show_pic(void);
int nopic(void) ;
int album_b1(void);
int album_ask(void);
int album_b2(void);
int album_b3(void);
int album_b4(void);
int album_refresh(void);
int refresh(void);
int show_recording(void);
int show_flash(void);
int showbmp(const char *bmp_path, int lcd_x, int lcd_y, int bmp_x, int bmp_y);


#endif