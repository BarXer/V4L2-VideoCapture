
#include "CameralOpt.h"

static int fd ; 
static int length ; 
static char *YUV[3] ; 

//初始化摄像头
int Init_Cameral(void)
{
    fd = open("/dev/video15" , O_RDWR);
    if(fd < 0)
    {
        perror("OPen cameral device fail");
        return -1 ; 
    }

    //查看当前设置所支持的图片格式
    struct v4l2_format  format ;    

    //抓图
    //1.设置摄像头抓取图片的格式与大小
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ;
    format.fmt.pix.width = WIDTH; 
    format.fmt.pix.height = HEIGHT; 
    format.fmt.pix.pixelformat =V4L2_PIX_FMT_YUYV  ; 

    if(ioctl(fd ,VIDIOC_S_FMT  , &format) != 0)
    {
        perror("set cameral format fail");
        return -2 ; 
    }

    //2.分配空间　
    struct v4l2_requestbuffers requestBuffer ; 
    requestBuffer.count = 3 ; 
    requestBuffer.type =  V4L2_BUF_TYPE_VIDEO_CAPTURE ;
    requestBuffer.memory =  V4L2_MEMORY_MMAP;
    
    if(ioctl(fd , VIDIOC_REQBUFS , &requestBuffer) != 0)
    {
        perror("reqeust buffer fail");
        return -3 ; 
    }

    //3.该摄像头已经具有三个空间，querybuffer  排队的同是印射内存空间

    int i ; 
    struct v4l2_buffer querybuffer ; 
    querybuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
    querybuffer.memory = V4L2_MEMORY_MMAP ; 

    for(i = 0 ; i < 3 ; i++)
    {
        querybuffer.index = i ; 
        if(ioctl(fd , VIDIOC_QUERYBUF , &querybuffer) != 0)
        {
            perror("query buffer fail");
            return -4 ; 
        }
        
        //printf("sequence:%d offset:%d length:%d \n" , querybuffer.sequence , querybuffer.m.offset , querybuffer.length);
       // void *mmap(void *addr, size_t length, int prot, int flags,
       //                  int fd, off_t offset);

       YUV[i] = mmap(0 , querybuffer.length ,PROT_READ|PROT_WRITE , MAP_SHARED , fd , querybuffer.m.offset); 
       length = querybuffer.length ; 

        //入队
        if(ioctl(fd , VIDIOC_QBUF , &querybuffer) != 0)
        {
            perror("EnQueue Buffer");
            return -5 ; 
        }
    }
}

//开始摄像头　
int Start_Cameral(void)
{
    //开启摄像头
    int on = 1 ; 
    if(ioctl(fd , VIDIOC_STREAMON  , &on) != 0)
    {
        perror("Start Cameral fail");
        return -6 ; 
    }
}

//停止摄像头
int Stop_Cameral(void)
{
    //开启摄像头
    int on = 1 ; 
    if(ioctl(fd , VIDIOC_STREAMOFF, &on) != 0)
    {
        perror("Stop Cameral fail");
        return -6 ; 
    }
}

//退出摄像头
int Exit_Cameral(void)
{
    int i = 0 ; 
    for(i = 0 ; i < 3 ; i++)
    {
       munmap(YUV[i] , length);
    }
    close(fd);
}

//获取一张图片
int Get_Picture(char *yuv)
{
    //保存一张图片
    //先出队　　－－》　　取图片　　－－》　 入队
    struct v4l2_buffer  Dequeue ; 
    Dequeue.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
    Dequeue.memory = V4L2_MEMORY_MMAP ; 
    if(ioctl(fd , VIDIOC_DQBUF , &Dequeue) != 0)
    {
        perror("dequeue fail");
        return -7 ; 
    }
    
    //YUV[Dequeue.index]
    memcpy(yuv , YUV[Dequeue.index] , Dequeue.length);

    //入队
    if(ioctl(fd , VIDIOC_QBUF , &Dequeue) != 0)
    {
        perror("queue fail");
        return -8 ; 
    }
}



