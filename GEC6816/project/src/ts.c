#include "ts.h"

static int ts_fd;		//static的作用是，不让这个变量在其他文件使用

//1、打开触摸屏文件
int open_ts(void)
{
	ts_fd = open("/dev/input/event0", O_RDONLY);
	if (ts_fd == -1)
	{
		printf("open event0 fail!\n");
		return -1;
	}
	return 0;
}


//2、读取触摸屏的xy的数据
int get_ts_xy(int *ts_x, int *ts_y)
{
	struct input_event ts_buf;
	//2、读取触摸屏的数据
		while(1)
		{
			read(ts_fd, &ts_buf, sizeof(ts_buf));	//scanf函数
			//获取x轴坐标值
			if (ts_buf.type == EV_ABS && ts_buf.code == ABS_X)
			{
				
				*ts_x = ts_buf.value/1024.0*800;	//黑色边
				//*ts_x = ts_buf.value;				//蓝色边
			}

			//获取y轴坐标值
			if (ts_buf.type == EV_ABS && ts_buf.code == ABS_Y)
			{
				
				*ts_y = ts_buf.value/600.0*480;	  //黑色边
				//*ts_y = ts_buf.value;				//蓝色边
			
			}
			if(ts_buf.type == EV_KEY && ts_buf.code == BTN_TOUCH)
			{
				if(ts_buf.value == 0)
				{
					printf("(%d, %d)\n", *ts_x, *ts_y);
					break;
				}
				
			}
		}	
}


//3、关闭触摸屏文件
int close_ts()
{
	close(ts_fd);
	return 0;
}

int ts_return(int x , int y, int i )
{

	if(x <= 133)
	   {
		  if(i==0)
		  {
             return 3;
		  }
          else
          {
             i--;
             return i;
		  }
	   }
	   else if(x >= 667)
	   {
		  if(i==3)
		  {
             return 0;
		  }
          else
          {
             i++;
             return i;
		  }
	   }
	   else 
	   {
		  return i; 
	   }
}


