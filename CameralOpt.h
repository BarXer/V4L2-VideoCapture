
#ifndef  _CAMERAL_OPT_H
#define  _CAMERAL_OPT_H 

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>

#define WIDTH   640
#define HEIGHT  480

int Init_Cameral(void);

int Start_Cameral(void);

int Stop_Cameral(void);

int Exit_Cameral(void);

int Get_Picture(char *yuv);

#endif  //_CAMERAL_OPT_H
