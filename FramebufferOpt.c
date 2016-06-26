

#include "FramebufferOpt.h"

static int fd ; 
static int width ; 
static int height ; 
static int length ; 
static unsigned int *framebuffer;
int Init_Framebuffer(void)
{
    //设备    /dev/fb0
    //头文件  /usr/include/linux/fb.h
    fd = open("/dev/fb0" , O_RDWR);
    if(fd < 0)
    {
        perror("Open framebuffer fail");
        return -1 ; 
    }

    struct fb_var_screeninfo  info ; 

    //获取参数
    if(ioctl(fd , FBIOGET_VSCREENINFO , &info) != 0)
    {
        perror("get screen info fail");
        return -2 ; 
    }
    //                              1024         768           32
    printf("x: %d y:%d bpp:%d \n" , info.xres , info.yres , info.bits_per_pixel );

    width = info.xres ; 
    height = info.yres ; 

    framebuffer = mmap(0 , info.xres*info.yres*4 ,PROT_READ|PROT_WRITE , MAP_SHARED , fd , 0 );
    length = info.xres*info.yres*4 ; 
}

int Show_Picture(int _width , int _height , const char *imagebuffer)
{

    int row , col ; 
    for(row = 0 ; row < height ; row++)
    {
        for(col = 0 ; col < width ; col++)
        {
            if((row < _height) && (col < _width))
            {
            framebuffer[row*width+col]= RGB(imagebuffer[(row*_width+col)*3+2],imagebuffer[(row*_width+col)*3+1],imagebuffer[(row*_width+col)*3] ) ; 
            }else
            {   
            framebuffer[row*width+col]=RGB(255,0,0);
            }
        }
    }
}

int Exit_Framebuffer(void)
{
    munmap(framebuffer,length );
    close(fd);
}
