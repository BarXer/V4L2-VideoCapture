
#ifndef  _FRAMEBUFFER_OPT_H
#define  _FRAMEBUFFER_OPT_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include "j-yuv.h"
#include <sys/mman.h>

#define  RGB(r,g,b)   (((r)<<16)|((g)<<8)|(b))

int Init_Framebuffer(void);

int Show_Picture(int _width , int _height , const char *imagebuffer);

int Exit_Framebuffer(void);

#endif //_FRAMEBUFFER_OPT_H
