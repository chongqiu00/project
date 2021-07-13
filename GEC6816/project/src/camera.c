#include "camera.h"
#include "bmp.h"

//定义结构体变量
static struct jpg_data video_buf;

extern int photo_flag;	//变量声明
extern int record_state;
char buf_camera_pic[64] = {0};
char buf_record_pic[64] = {0};
int camera_pic_num = 0;
int record_pic_num = 0;

//1、打开摄像头
int open_camera(char *pathname_camera)
{
	lcd_open_camera();
	mmap_lcd_camera();
	//初始化摄像头
	linux_v4l2_yuyv_init(pathname_camera);
	//开启摄像头捕捉
	linux_v4l2_start_yuyv_capturing();
	return 0;
}

//2、运行摄像头
int camera_working(void)
{
	while(1)
	{
		//获取摄像头捕捉的画面
		linux_v4l2_get_yuyv_data (&video_buf);
		
		//显示摄像头捕捉的画面
		show_video_data_camera(0, 0, video_buf.jpg_data , video_buf.jpg_size);
		//按键触发拍照
		if (photo_flag == 1)
		{
			printf("现在拍的是第%d张图片。。。。。\n", camera_pic_num);
			sprintf(buf_camera_pic, "./%d.jpg", camera_pic_num);
			int fd = open(buf_camera_pic, O_WRONLY|O_CREAT, 0777);
			if (fd == -1)
			{
				printf("creat jpg fail! \n");
				return -1;
			}
			write(fd, video_buf.jpg_data, video_buf.jpg_size);
			close(fd);
			camera_pic_num++;
			show_flash();
			photo_flag = 0;
		}
		if(record_state == 1)
		{
			printf("现在拍的是第%d张图片。。。。。\n", record_pic_num);
			sprintf(buf_record_pic, "./record/%d.jpg", record_pic_num);
			int fd = open(buf_record_pic, O_WRONLY|O_CREAT, 0777);
			if (fd == -1)
			{
				printf("creat jpg fail! \n");
			}
			write(fd, video_buf.jpg_data, video_buf.jpg_size);
			close(fd);
			record_pic_num++;
			//usleep(500000);
		}
	}
	return 0;
}


//3、关闭摄像头
int close_camera(void)
{
	//摄像头解除初始化
	linux_v4l2_yuyv_quit();
	lcd_close_camera();
	return 0;
}
void *camera_working_pthread(void *arg)
{
	camera_working();
}
void *recording_pthread(void *arg)
{
	while(1)
	{
		if (record_state == 1)
		{
			printf("现在拍的是第%d张图片。。。。。\n", record_pic_num);
			sprintf(buf_record_pic, "./record/%d.jpg", record_pic_num);
			int fd = open(buf_record_pic, O_WRONLY|O_CREAT, 0777);
			if (fd == -1)
			{
				printf("creat jpg fail! \n");
			}
			write(fd, video_buf.jpg_data, video_buf.jpg_size);
			close(fd);
			record_pic_num++;
			usleep(500000);
		}
		if(record_state == 0)
		{
			system("ffmpeg -f image2 -i ./record/%d.jpg -r 20 ./1.avi ");
			usleep(1000000);
			break;
		}
		
		
		
		
		
	}
	
}

