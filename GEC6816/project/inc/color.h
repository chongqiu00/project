#ifndef _COLOR_H
#define _COLOR_H 

#include <stdio.h>
//open函数
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//read、close、write函数
#include <unistd.h>
//mmap函数
#include <sys/mman.h>


//1、打开lcd屏幕文件、申请映射内存
int open_lcd_color(void);	

//2、显示颜色(写颜色数据到屏幕文件中)
int show_color(int color);

//3、关闭lcd屏幕文件、释放映射内存
int close_lcd_color(void);

#endif /* color.h */