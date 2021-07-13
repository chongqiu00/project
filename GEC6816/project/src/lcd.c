#include "lcd.h"

int init_lcd(int* lcd_fd)
{
	*lcd_fd = open(DEV_LCD, O_RDWR);
	if (*lcd_fd == -1){
		printf("lcd(%s) open faild!\n", DEV_LCD);
		return -1;
	}
    printf("lcd(%s) open succeed!\n", DEV_LCD);
}

int lcd_uinit(int* lcd_fd)
{
    int i;
    i = close(*lcd_fd);
    if ( -1 == i ){
        printf("lcd(%s) close faild!\n");
        return -1;
    }else{
        printf("lcd(%s) close succeed!\n");
        return 0;
    }
}

int lcd_circle_wire(int* lcd_fd, int color, int x, int y, int r, int value)
{
  int j,i;
  int* lcd_buff = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,*lcd_fd,0);
  if (MAP_FAILED == lcd_buff)
  {
     perror("error");
     return -1;
  }
  for ( j=0; j<480; j++ )
  {
     for (i=0; i<800; i++ )
     {
       if( (((i-x)*(i-x) + (j-y)*(j-y)) >= (r*r)) && 
           (((i-x)*(i-x) + (j-y)*(j-y)) <= ((r+value)*(r+value))) )
        lcd_buff[j*800+i] = color;
     }
  }
  munmap(lcd_buff, 800*480*4);
  return 0;    
}

int lcd_all_color(int*lcd_fd, int color)
{
  int j,i;
  //��?
  int* lcd_buff = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,*lcd_fd,0);
  if (MAP_FAILED == lcd_buff)
  {
     perror("error");
     return -1;
  }
  
  for ( j=0; j<480; j++ )
  {
     for (i=0; i<800; i++ )
     {
      lcd_buff[j*800+i] = color;
     }
  }
  munmap(lcd_buff, 800*480*4);
  return 0;
}

int lcd_circle(int* lcd_fd, int color, int x, int y, int r)
{
  int j,i;
  int* lcd_buff = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,*lcd_fd,0);
  if (MAP_FAILED == lcd_buff)
  {
     perror("error");
     return -1;
  }
  for ( j=0; j<480; j++ )
  {
     for (i=0; i<800; i++ )
     {
       if( ((i-x)*(i-x) + (j-y)*(j-y)) < (r*r) )
        lcd_buff[j*800+i] = color;
     }
  }
  munmap(lcd_buff, 800*480*4);
  return 0;
}

int lcd_cuboid(int* lcd_fd, int x, int y, int wide, int high, int color)
{
  int j,i;
  int* lcd_buff = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,*lcd_fd,0);
  if (MAP_FAILED == lcd_buff)
  {
    perror("error");
    return -1;
  }

  for (j=x; j<x+wide; j++) //?
  {
     for (i=y; i<y+high; i++) //?
     {
       lcd_buff[j*800+i] = color;
     }
  }
  munmap(lcd_buff, 800*480*4);
  return 0;
}

int lcd_draw_bmp(int*lcd_fd, const char *pathname, int x, int y, int w, int h)
{
	//1,��
	int* lcd_ptr = mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, *lcd_fd, 0);
	
	if (lcd_ptr == MAP_FAILED) {
		printf("mmap failed!\n");
		return -1;
	}
	
	int i, j;
	
	//a ����
	
	int bmp_fd = open(pathname, O_RDWR);
	
	//����
	if (bmp_fd == -1) {
		printf("open bmp file failed!\n");
		return -1;
	}
	
	//2? ��������lcd��
	char header[54];
	char rgb_buf[w*h*3];
	//a ����������?
	
	read(bmp_fd, header, 54);
	read(bmp_fd, rgb_buf, w*h*3);
	//b �����lcd��
	
	
	// int r = 0xef, g = 0xab, b = 0xcd;
	// int color = 0xefabcd;
	
	//int color = b;
	
	// ?1����1
	//     b : 00000000 00000000 00000000 11001101
	//     g : 00000000 00000000 10101011 00000000
	// color : 00000000 00000000 10101011 11001101
	
	// 1000  = 800*1+200
	// 1800  = 800*2+200
	
	//24 --- 32
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			int b = rgb_buf[(j*w+i)*3+0];
			int g = rgb_buf[(j*w+i)*3+1];
			int r = rgb_buf[(j*w+i)*3+2];
			
			int color = b;
			color |= (g << 8);
			color |= (r << 16);
			*(lcd_ptr+800*(h-1-j+y)+(i+x)) = color;
		}
	}
	
	//3,����
	//a ������
	close(bmp_fd);
    munmap(lcd_ptr, 800*480*4);
	
	// //b
	// close(lcd_fd);
}


METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


//char* img_buf, int img_size:动态数据，若为空，则为静态数据打开
//const char* pathname:静态数据，若为空，则为动态数据打开
//int shrink_picture, int enlarge_picture:缩小，放大�?
int lcd_draw_jpeg(int* lcd_fd, int x, int y, char* img_buf, int img_size, const char* pathname,int shrink_picture, int enlarge_picture)
{
	int* lcd_ptr = mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, *lcd_fd, 0);

	if (lcd_ptr == MAP_FAILED) {
		printf("mmap lcd device failed!\n");
		return -2;
	}
	//jpeg解压缩对象和错误处理对象的定�?
	struct stat statbuf;
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	char* argb_buf = lcd_buf;
	int x_s = x; 
	int jpeg_size;
	char * jpeg_buf;
	int jpeg_fd;

	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if (pathname != NULL )//静�?
    {
		//1，打开源文�?
		jpeg_fd = open(pathname, O_RDWR);

		if (jpeg_fd == -1) {
			printf("open jpeg file : %s failed !\n", pathname);
			return -1;
		}

		if (setjmp(jerr.setjmp_buffer)) {
			jpeg_destroy_decompress(&cinfo);
			close(jpeg_fd);
			return -1;
		}

		//1，分配并初始化解压缩对象
		jpeg_create_decompress(&cinfo);

		//2，指定解压缩数据�?
		fstat(jpeg_fd, &statbuf);  
		
		jpeg_size = statbuf.st_size;
		//printf("jpeg_size=%d\n", jpeg_size);
		jpeg_buf = calloc(1, jpeg_size);
	//	printf("jpeg_buf=%d\n", jpeg_buf);
	}
    else//动�?,有关摄像头�?
    {
        if (setjmp(jerr.setjmp_buffer)) {
    	jpeg_destroy_decompress(&cinfo);
    	close(jpeg_fd);
    	return -1;}

        //1，分配并初始化解压缩对象
  	    jpeg_create_decompress(&cinfo);

  	    //2，指定解压缩数据�?
  	    fstat(jpeg_fd, &statbuf);  

		//动�?
		jpeg_size = img_size;
		jpeg_buf = img_buf;
	}

  	read(jpeg_fd, jpeg_buf, jpeg_size);

  	jpeg_mem_src(&cinfo, jpeg_buf, jpeg_size);

  	//3，读取文件头
  	(void) jpeg_read_header(&cinfo, TRUE);
	  //图片压缩比num/denom=1/2
	  cinfo.scale_num = shrink_picture; //缩小
	  cinfo.scale_denom = enlarge_picture;//放大

  	//4, 发起开始解压缩信号
  	(void) jpeg_start_decompress(&cinfo);

  	int i, color;

  	//5，取出数�?
  	while (cinfo.output_scanline < cinfo.output_height) {
  		argb_buf = lcd_buf;
  		//每次读取一行的数据
  		(void) jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&argb_buf, 1);

  		//显示图片的数据到lcd屏幕�?
  		for(i = 0; i<(cinfo.output_width); i++) {
				/* 获取rgb�? */
				color = 		*(argb_buf+2);
				color = color | *(argb_buf+1)<<8;
				color = color | *(argb_buf)<<16;
				
				/* 显示像素�? */
				// lcd_draw_point(x,y,color);
				// int lcd_draw_point(int i, int j, int color)
				// {
				// 	*(lcd_ptr+800*j+i) = color;
				// }
				*(lcd_ptr+800*y+x) = color;

				argb_buf +=3;
				
				x++;
			}
			
			/* 换行 */
			y++;
			x = x_s;	
  	}

  	//6，解压完�?
  	(void) jpeg_finish_decompress(&cinfo);
  	//7，释放资�?
  	jpeg_destroy_decompress(&cinfo);

	munmap(lcd_ptr, 800*480*4);
  	close(jpeg_fd);

	return 0;
}





