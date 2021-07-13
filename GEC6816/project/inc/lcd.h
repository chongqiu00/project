/*
 * 一个简单的示例：lcd全部刷为紫色
 *
 * #include "lcd.h"
 * int main()
 * {
 *  int fd;
 *  init_lcd(&fd);
 *  lcd_all_color(&fd, 0xab8899);
 *  uinit_lcd(&fd);
 *  return 0;
 *  }
 */

#ifndef LCD_H
#define LCD_H

/*
 * **注意：**
 * 1. lcd屏幕左上方为(0,0)
 * 2. 刷rgb颜色,注意颜色参数为十六进制表示，例如：0xffffff
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "jpeglib.h"
#include <setjmp.h>
#include <linux/input.h>
#include <math.h>

#define DEV_LCD "/dev/fb0" /* lcd设备文件 */
char lcd_buf[800*480*4];


/*
 * 功能：打开lcd设备?
 * 参数：lcd_fd为lcd设备标识符
 * 返回值：成功返回0，失败返回非0
 */
int init_lcd(int* lcd_fd);

/*
 * 功能：关闭lcd设备
 * 参数：lcd_fd为lcd设备标识符
 * 返回值：成功返回0，失败返回非0
 */
int lcd_uinit(int* lcd_fd);

/*
 * 刷线：
 * y圆线
 */
/*
 * 功能：在lcd上的(x,y)为圆心，r为半径画圆线，圆线的颜色为color    
 * 参数：
 *  lcd_fd为lcd设备标识符；
 *  color为圆内的颜色
 *  x,y分别为圆心的x,y轴坐标；取值范围x:(0~800) y:(0,480)
 *  r为圆的半径, 取值范围r:(0,240)
 *  value为圆线的粗细
 * 返回值：成功返回0，失败返回-1
 */   
int lcd_circle_wire(int* lcd_fd, int color, int x, int y, int r, int value);


/*
 * 刷简单图形：
 * 圆，矩形
 */

/*
 * 功能：将lcd屏幕全部显示为color颜色
 * 参数：lcd_fd为lcd设备标识符；color为指定颜色的gbr值
 * 返回值：成功返回0，失败返-1 
 */                                                                     
int lcd_all_color(int*lcd_fd, int color);

/*
 * 功能：在lcd上的(x,y)为圆心，r为半径画圆，圆的颜色为color   
 * 参数：
 *  lcd_fd为lcd设备标识符；
 *  color为圆内的颜色
 *  x,y分别为圆心的x,y轴坐标；取值范围x:(0~800) y:(0,480)
 *  r为圆的半径, 取值范围r:(0,240)
 * 返回值：成功返回0，失败返回-1   
 */   
int lcd_circle(int* lcd_fd, int color, int x, int y, int r);

/*
 * 功能：在lcd屏幕中的(x,y)为图形左上角起始显示长high宽wide的矩形,矩形的颜色为color
 * 参数：
 *  lcd_fd为lcd设备标识符；
 *  color为矩形内的颜色
 *  x,y分别为矩形的左上角顶点坐标；取值范围x:(0~800) y:(0,480)
 * 返回值：成功返回0，失败返回-1   
 */   
int lcd_cuboid(int* lcd_fd, int x, int y, int wide, int high, int color);

/*
 * 刷图片
 */

/*
 * 功能：在lcd上面刷指定大小的图片(bmp)
 * 参数：
 *  lcd_fd为lcd设备标识符；
 *  const char *pathname:图片
 *  int x, int y:图片显示的起点。取值范围x:(0~800) y:(0,480)
 * 返回值：成功返回0，失败返回-1   
 */  

int lcd_draw_bmp(int* lcd_fd, const char *pathname, int x, int y, int w, int h);



/*
 * 功能：在lcd上面刷指定大小的图片（jpg）支持缩小放大图片。
 * 参数：
 *  lcd_fd为lcd设备标识符；
 *  int x, int y:图片显示的起点。取值范围x:(0~800) y:(0,480)
 *  char* img_buf, int img_size:动态数据，若为空，则为静态数据打开
 *  const char* pathname:静态数据，若为空，则为动态数据打开
 *  int shrink_picture, int enlarge_picture:缩小，放大。
 * 返回值：成功返回0，失败返回-1   
 */  


struct my_error_mgr {
    struct jpeg_error_mgr pub;	/* "public" fields */

    jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo);

int lcd_draw_jpeg(int* lcd_fd, int x, int y, char* img_buf, int img_size, const char* pathname,int shrink_picture, int enlarge_picture);

#endif




