#ifndef __CAMERA_H
#define __CAMERA_H

/*
	1、头文件
	2、宏定义
	3、函数声明
	....
*/
#include <stdio.h>
#include "jpeglib.h"
#include "yuyv.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <linux/input.h>
#include <stdlib.h>
#include "camera_lcd.h"
#include "jpeglib.h"

//1、打开摄像头
int open_camera(char *pathname_camera);
//2、运行摄像头
int camera_working(void);
//3、关闭摄像头
int close_camera(void);
void *camera_working_pthread(void *arg);
void *recording_pthread(void *arg);


#endif /* __CAMERA_H */