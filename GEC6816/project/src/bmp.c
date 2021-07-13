#include "bmp.h"

static int lcd_fd;
static unsigned int *lcd_p;
static unsigned int *lcd_p_flag;

int draw_button(int w,int h,int x1,int y1,int a[],int n)
{
	int x,y;
	for(y=0;y<480;y++)
	{
		for(x=0;x<800;x++)
		{
			 if ((x <= x1 + w)&&(x >= x1)&&(y >= y1)&&(y <= y1 + h)&&(((x >= x1 + w-n)||(x <= x1+n))||((y >= y1 + h-n)||(y <= y1+n))))//
			{
				a[(480-1-y)*800+x] = 0x00b40326;
			} 
		}
	}
}

//1、打开lcd屏幕文件、申请映射内存
int open_lcd_bmp(void)
{
	lcd_fd = open("/dev/fb0", O_RDWR);
	if (lcd_fd == -1)
	{
		printf("open lcd error!\n");
		return -2;
	}
	//b、写1.bmp图片数据到lcd屏幕上
	//write(lcd_fd, lcd_buf , 800*480*4);
	lcd_p  =	mmap(	
	 					NULL, 					//默认为NULL,让系统自动分配,首地址
						800*480*4,				//映射空间的大小，也就是开发板现存
				 		PROT_READ|PROT_WRITE,	//允许进行读写操作
				 		MAP_SHARED,				//此内存空间被共享
				 		lcd_fd, 				//lcd的文件描述符
				 		0						//默认为0偏移
					);

	lcd_p_flag = lcd_p;  //让lcd_p_flag记住lcd_p的初始位置
	return 0;

}

int open_bmp(const char *bmp_path,char a[],int bmp_x ,int bmp_y)
{
	int bmp_fd = open(bmp_path, O_RDONLY);
	if (bmp_fd == -1)
	{
		printf("open bmp error!\n");
		return -1;
	}
	lseek(bmp_fd, 54, SEEK_SET);
	read(bmp_fd, a, bmp_x*bmp_y*3);
	close(bmp_fd);
}
void Gray(char org[],int bmp_x ,int bmp_y)
{
	int i;
	for(i=0; i<bmp_x*bmp_y; i++)
	{
		int gray;
		gray = org[3*i]*0.11+org[3*i+1]*0.59+org[3*i+2]*0.3;
		org[3*i] = gray;
		org[3*i+1] = gray;
		org[3*i+2] = gray;
		
	}
}

void extend(int goal[],char org[],int bmp_x ,int bmp_y)
{
	int i;
	for(i=0; i<bmp_x*bmp_y; i++)
	{
		goal[i] = (org[3*i]<<0) + (org[3*i+1]<<8) + (org[3*i+2]<<16) + (0x00<<24);
		             //     B          +        G            +          R           +   人为的添加一位
	}
}

void circle_appear(int data[],int bmp_x, int bmp_y)
{
	int x,y,r,n;
	for(n=20;n>=0;n--)
	  {
		r = n * 27;
		for(y=0;y<480;y++) 
		{
			for(x=0;x<800;x++)
			{
				if(((x-400)*(x-400))+((y-240)*(y-240))>=r*r)
				{
					*(lcd_p+y*800+x) = data[(bmp_y-1-y)*bmp_x+x];
				}
			}
		}
         //usleep(1);		
	  }
}

void normal_appear(int data[],int bmp_x, int bmp_y)
{
	int x,y;
		for(x=0; x<bmp_x; x++)
	   {
		  for(y=0; y<bmp_y; y++)
		  {
			 *(lcd_p+y*800+x) = data[(bmp_y-1-y)*bmp_x+x];
		  }
	   }
}
//2、显示任意位置大小bmp图片
int show_bmp(const char *bmp_path, int lcd_x, int lcd_y, int bmp_x, int bmp_y,int effect,int button)
{
	char bmp_buf[bmp_x*bmp_y*3];
	int lcd_buf[bmp_x*bmp_y];
	open_bmp(bmp_path,bmp_buf,bmp_x,bmp_y);	
   // Gray(bmp_buf,bmp_x,bmp_y);	
	extend(lcd_buf,bmp_buf,bmp_x,bmp_y);
	if(button == BUTTON)
	{
		draw_button(150,90,645,320,lcd_buf,4);
	}
	lcd_p = lcd_p + lcd_x;		//x轴
	lcd_p = lcd_p + lcd_y*800;	//y轴
	if(effect == CIRCLE)
	{
		circle_appear(lcd_buf,bmp_x,bmp_y);
	}
	else if(effect == NORMAL)
	{
		normal_appear(lcd_buf,bmp_x,bmp_y);
	}
    
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}
int show_first() //lcd_draw_jpeg
{
	char bmp_buf[800*480*3];
	int lcd_buf[800*480];
	open_bmp("./ui/ui.bmp",bmp_buf,800,480);	
	extend(lcd_buf,bmp_buf,800,480);	
	normal_appear(lcd_buf,800,480);
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}
int show_flash()  //拍照特效
{
	char bmp_buf[640*480*3];
	int lcd_buf[640*480];
	open_bmp("./ui/flash.bmp",bmp_buf,640,480);	
	extend(lcd_buf,bmp_buf,640,480);	
	normal_appear(lcd_buf,640,480);
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}
int show_recording()  //拍照特效
{
	char bmp_buf[160*480*3];
	int lcd_buf[160*480];
	open_bmp("./ui/recording.bmp",bmp_buf,160,480);	
	extend(lcd_buf,bmp_buf,160,480);
	lcd_p = lcd_p + 640;		//x轴
	lcd_p = lcd_p + 0;	//y轴	
	normal_appear(lcd_buf,160,480);
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}
int album_b1()  //相册按键一
{
	char bmp_buf[160*480*3];
	int lcd_buf[160*480];
	open_bmp("./ui/album_b1.bmp",bmp_buf,160,480);	
	extend(lcd_buf,bmp_buf,160,480);	
	lcd_p = lcd_p + 640;		//x轴
	lcd_p = lcd_p + 0;	//y轴	
	normal_appear(lcd_buf,160,480);
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}
int album_b2()  //相册按键一
{
	char bmp_buf[160*480*3];
	int lcd_buf[160*480];
	open_bmp("./ui/album_b2.bmp",bmp_buf,160,480);	
	extend(lcd_buf,bmp_buf,160,480);	
	lcd_p = lcd_p + 640;		//x轴
	lcd_p = lcd_p + 0;	//y轴	
	normal_appear(lcd_buf,160,480);
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}
int album_b3()  //相册按键一
{
	char bmp_buf[160*480*3];
	int lcd_buf[160*480];
	open_bmp("./ui/album_b3.bmp",bmp_buf,160,480);	
	extend(lcd_buf,bmp_buf,160,480);	
	lcd_p = lcd_p + 640;		//x轴
	lcd_p = lcd_p + 0;	//y轴	
	normal_appear(lcd_buf,160,480);
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}
int album_b4()  //相册按键一
{
	char bmp_buf[160*480*3];
	int lcd_buf[160*480];
	open_bmp("./ui/album_b4.bmp",bmp_buf,160,480);	
	extend(lcd_buf,bmp_buf,160,480);	
	lcd_p = lcd_p + 640;		//x轴
	lcd_p = lcd_p + 0;	//y轴	
	normal_appear(lcd_buf,160,480);
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}
int album_refresh()  //相册复原
{
	char bmp_buf[160*480*3];
	int lcd_buf[160*480];
	open_bmp("./ui/album_refresh.bmp",bmp_buf,160,480);	
	extend(lcd_buf,bmp_buf,160,480);	
	lcd_p = lcd_p + 640;		//x轴
	lcd_p = lcd_p + 0;	//y轴	
	normal_appear(lcd_buf,160,480);
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}
int nopic()  //无照片
{
	char bmp_buf[800*480*3];
	int lcd_buf[800*480];
	open_bmp("./ui/nopic.bmp",bmp_buf,800,480);	
	extend(lcd_buf,bmp_buf,800,480);	
	normal_appear(lcd_buf,800,480);
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}
int album_ask()  //相册询问
{
	char bmp_buf[280*168*3];
	int lcd_buf[280*168];
	open_bmp("./ui/ask.bmp",bmp_buf,280,168);	
	extend(lcd_buf,bmp_buf,280,168);
	lcd_p = lcd_p + 170;		//x轴
	lcd_p = lcd_p + 156*800;	//y轴	
	normal_appear(lcd_buf,280,168);
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}
int refresh()  //刷新首页边框
{
	char bmp_buf[160*480*3];
	int lcd_buf[160*480];
	open_bmp("./ui/refresh.bmp",bmp_buf,160,480);	
	extend(lcd_buf,bmp_buf,160,480);
	lcd_p = lcd_p + 640;		//x轴
	lcd_p = lcd_p + 0;	//y轴	
	normal_appear(lcd_buf,160,480);
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}
int show_pic() //相册
{
	char bmp_buf[800*480*3];
	int lcd_buf[800*480];
	open_bmp("./ui/album_4.bmp",bmp_buf,800,480);	
	extend(lcd_buf,bmp_buf,800,480);	
	normal_appear(lcd_buf,800,480);
	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p			

}
//3、关闭lcd屏幕文件、释放映射内存
int close_lcd_bmp(void)
{
	
	munmap(lcd_p, 800*480*4);
	close(lcd_fd);

}



void bmp_lcd(const char *bmp_path)
{
	open_lcd_bmp();
	show_bmp(bmp_path, 0,0, 623, 480,NORMAL,BUTTON);
	//draw_button(150,80,645,320,lcd_buf);
	close_lcd_bmp(); 
	//sleep(3);
	
}
int showbmp(const char *bmp_path, int lcd_x, int lcd_y, int bmp_x, int bmp_y)
{
	//1、第一阶段：读取到1.bmp图片的数据
	//a、打开1.bmp图片的文件
	int bmp_fd = open(bmp_path, O_RDONLY);
	if (bmp_fd == -1)
	{
		printf("open bmp error!\n");
		return -1;
	}
	//b、lseek函数跳过54个头信息
	lseek(bmp_fd, 54, SEEK_SET);
	//c、读取1.bmp图片的数据
	char bmp_buf[bmp_x*bmp_y*3];
	read(bmp_fd, bmp_buf, bmp_x*bmp_y*3);
	//d、关闭1.bmp图片的文件
	close(bmp_fd);
		

	//2、第二阶段：
	//800*480*3的bmp图片数据转换成
	//800*480*4的lcd显示数据
	int lcd_buf[bmp_x*bmp_y];
	int i;
	for(i=0; i<bmp_x*bmp_y; i++)
	{
		lcd_buf[i] = (bmp_buf[3*i]<<0) + (bmp_buf[3*i+1]<<8) + (bmp_buf[3*i+2]<<16) + (0x00<<24);
		             //     B          +        G            +          R           +   人为的添加一位
	}

	//3、第三阶段：写入1.bmp图片的数据到lcd屏幕
	//c、调节图片开始刷的位置
	lcd_p = lcd_p + lcd_x;		//x轴
	lcd_p = lcd_p + lcd_y*800;	//y轴

	int x,y;
	for(x=0; x<bmp_x; x++)
	{
		for(y=0; y<bmp_y; y++)
		{
			*(lcd_p+y*800+x) = lcd_buf[(bmp_y-1-y)*bmp_x+x];
		}
	}

	lcd_p = lcd_p_flag;		//将其初始位置重新赋予lcd_p		

}	

