
#include <stdio.h>
#include "CameralOpt.h"
#include "FramebufferOpt.h"
#include "j-yuv.h"

#define  msleep(x)   usleep(1000*x)

int main(void)
{
    //初始化摄像头
    
    Init_Cameral();
    Init_Framebuffer();
    Start_Cameral();

    unsigned char yuv[WIDTH*HEIGHT*2] ; 
    unsigned char bmp[WIDTH *HEIGHT *3];
    
    static int i = 0 ; 
    while(1)
    {

    //获取图片
    Get_Picture(yuv);

    //yuyv 转 bmp
    yuyv2rgb24(yuv, bmp , WIDTH ,HEIGHT );

    Show_Picture(WIDTH , HEIGHT , bmp);
    printf("i : %d \n" , i++);
    }

    Exit_Framebuffer();
    Exit_Cameral();
    return 0 ; 
}
