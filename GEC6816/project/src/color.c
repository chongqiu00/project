#include "color.h"

static int lcd_fd;
static unsigned int *lcd_p;

//1、打开lcd屏幕文件、申请映射内存
int open_lcd_color(void)
{
	lcd_fd = open("/dev/fb0", O_RDWR);
	if (lcd_fd == -1)
	{
		printf("open lcd error!\n");
		return -1;
	}	

	//write(lcd_fd, lcd_buf, 800*480*4);
	lcd_p =	mmap(	
					NULL, 					//默认为NULL,让系统自动分配,首地址
					800*480*4,				//映射空间的大小，也就是开发板现存
					PROT_READ|PROT_WRITE,	//允许进行读写操作
					MAP_SHARED,				//此内存空间被共享
					lcd_fd, 				//lcd的文件描述符
					0						//默认为0偏移
				);

	return 0;
}


//2、显示颜色(写颜色数据到屏幕文件中)
int show_color(int  color)
{

	int lcd_buf[800*480] = {0};  
	int i, j;
	for(i=0; i<800*480; i++)
		lcd_buf[i] = color;//绿色

	for (j=0; j < 800*480; j++)
	{
	 	*(lcd_p+j) = lcd_buf[j];	
	}
	return 0;

}

//3、关闭lcd屏幕文件、释放映射内存
int close_lcd_color(void)
{
	//3、关闭lcd屏幕文件
	munmap(lcd_p, 800*480*4);
	close(lcd_fd);
	return 0;
}


