/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "serial_port.h"
#include <string.h>
#include <stdio.h>  /*标准输入输出定义*/
#include <stdlib.h> /*标准函数库定义*/
#include <unistd.h> /*Unix 标准函数定义*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>  /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>  /*错误号定义*/
#include <stdint.h>
#include <sys/ioctl.h>

static int speed_arr[14] = { B38400, B19200, B9600, B4800, B2400, B1200, B300,
                             B38400, B19200, B9600, B4800, B2400, B1200, B300, };
static int name_arr[14] = {38400,  19200,  9600,  4800,  2400,  1200,  300,
                           38400,  19200,  9600, 4800, 2400, 1200,  300, };

Serial_Port:: Serial_Port()
{
    fd=-1;
}

bool Serial_Port::OpenPort(const char * dev)
{
    char* _dev=new char[32];
    strcpy(_dev,dev);
    fd = open(_dev, O_RDWR | O_NOCTTY | O_NDELAY);         //| O_NOCTTY | O_NDELAY
    if (-1 == fd) {
        perror("Can't Open Serial Port");
        return false;
    } else m_portName = dev;

    int DTR_flag;
    DTR_flag = TIOCM_DTR;
    ioctl(fd,TIOCMBIS,&DTR_flag);//Set RTS pin
    return true;
}


int Serial_Port::setup(int speed,int databits,int stopbits,int parity,int flow_ctrl)
{
    int   i;
    //int   status;
    struct termios options;
    /*tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数还可以测试配置是否正确，该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1.
    */
    if( tcgetattr( fd,&options)  !=  0)
    {
        perror("SetupSerial 1");
        return(false);
    }

    //设置串口输入波特率和输出波特率
    for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
    {
        if  (speed == name_arr[i])
        {
            cfsetispeed(&options, speed_arr[i]);
            cfsetospeed(&options, speed_arr[i]);
        }
    }

    cfmakeraw(&options);
    //修改控制模式，保证程序不会占用串口
    options.c_cflag |= CLOCAL;
    //修改控制模式，使得能够从串口中读取输入数据
    options.c_cflag |= CREAD;

    //设置数据流控制
    switch(flow_ctrl)
    {

    case 0 ://不使用流控制
        options.c_cflag &= ~CRTSCTS;
        break;

    case 1 ://使用硬件流控制
        options.c_cflag |= CRTSCTS;
        break;
    case 2 ://使用软件流控制
        options.c_cflag |= IXON | IXOFF | IXANY;
        break;
    }

    //设置数据位
    //屏蔽其他标志位
    options.c_cflag &= ~CSIZE;
    switch (databits)
    {
    case 5    :
        options.c_cflag |= CS5;
        break;
    case 6    :
        options.c_cflag |= CS6;
        break;
    case 7    :
        options.c_cflag |= CS7;
        break;
    case 8:
        options.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr,"Unsupported data size\n");
        return (false);
    }
    //设置校验位
    switch (parity)
    {
    case 'n':
    case 'N': //无奇偶校验位。
        options.c_cflag &= ~PARENB;
        options.c_iflag &= ~INPCK;
        break;
    case 'o':
    case 'O'://设置为奇校验
        options.c_cflag |= (PARODD | PARENB);
        options.c_iflag |= INPCK;
        break;
    case 'e':
    case 'E'://设置为偶校验
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        options.c_iflag |= INPCK;
        break;
    case 's':
    case 'S': //设置为空格
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupported parity\n");
        return (false);
    }
    // 设置停止位
    switch (stopbits)
    {
    case 1:
        options.c_cflag &= ~CSTOPB; break;
    case 2:
        options.c_cflag |= CSTOPB; break;
    default:
        fprintf(stderr,"Unsupported stop bits\n");
        return (false);
    }

    //修改输出模式，原始数据输出
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
//    options.c_lflag &= ~(ISIG | ICANON);

    //设置等待时间和最小接收字符
    options.c_cc[VTIME] = 2; /* 读取一个字符等待1*(1/10)s */
    options.c_cc[VMIN] = 0; /* 读取字符的最少个数为1 */

    //如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读
    tcflush(fd,TCIFLUSH);

    //激活配置 (将修改后的termios数据设置到串口中）
    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
        perror("com set error!\n");
        return (false);
    }
    return (true);
}

int  Serial_Port::readBuffer(uint8_t * buffer,int size)
{
    int ret = 0;
    if(fd >= 0) ret = read(fd, buffer, size);
    return ret;
}

int  Serial_Port::writeBuffer(uint8_t * buffer,int size)
{
    int ret = 0;
    if(fd >= 0) ret = write(fd, buffer, size);
    return ret;
}

uint8_t  Serial_Port::getchar()
{
    uint8_t t;
    read(fd, &t, 1);
    return t;
}

void  Serial_Port::closePort()
{
    if(fd >= 0) close(fd);
    fd=-1;
}

bool Serial_Port::isOpen()
{
    return fd>=0 ? true:false;
}

void Serial_Port::flush()
{
    if(fd >= 0) {
        fsync(fd);
        //tcflush(fd, TCIFLUSH);    //清空输入缓存
        //tcflush(fd, TCOFLUSH);    //清空输出缓存
        //tcflush(fd, TCIOFLUSH);   //清空输入输出缓存
    }
}

int Serial_Port::writePort(const QByteArray &array)
{
    return writeBuffer((uint8_t *)array.data(), array.size());
}

QByteArray Serial_Port::readAll()
{
    QByteArray res;
    int ret = readBuffer(mBuf, sizeof(mBuf));
    if(ret > 0) res.append((char *)mBuf, ret);
    return res;
}
