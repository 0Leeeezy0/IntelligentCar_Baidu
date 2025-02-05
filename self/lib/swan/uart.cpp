#include <iostream>
#include <cmath>
#include <thread>
#include <stdio.h>
#include <stdlib.h>    
#include <string.h>
#include <unistd.h>    
#include <sys/types.h>  
#include <sys/time.h>
#include <sys/stat.h>   
#include <fcntl.h>      
#include <termios.h>  
#include <errno.h>
#include <vector>
#include <opencv2/opencv.hpp>

#include "swan/uart.h"

using namespace std;
using namespace cv;
using namespace swan;

// 串口初始化
int SwanUart::Init()
{
    int UartInitResult;  // 串口初始化结果

    if(Base.open(InitParameter.DevicePath,(int)InitParameter.DeviceBaudrate,0,8,1,1,2,1))
    {
        cout << "<-------------------------------uart module init info------------------------------->" << endl;
        cout << "uart device: ";
        cout << InitParameter.DevicePath;
        cout << " open successful" << endl; 
        cout << "uart baudrate: ";
        cout << InitParameter.DeviceBaudrate << endl;
        cout << "<----------------------------------------------------------------------------------->" << endl;
        cout << endl;
        UartInitResult = 1;
    }
    else
    {
        cout << "<-------------------------------uart module init info------------------------------->" << endl;
        cout << "uart device: ";
        cout << InitParameter.DevicePath;
        cout << " open failed" << endl; 
        cout << "<----------------------------------------------------------------------------------->" << endl;
        cout << endl;
        UartInitResult = 0;
    }
    return UartInitResult;
}

// 串口发送(1.使用SendInfoAdd添加串口数据 2.传入SnedInfo)
void SwanUart::Send()
{
    for(int i = 0;i < SendInfos.size();i++)
    {
        Base.send(&SendInfos[i],1);
    }
}
void SwanUart::Send(vector<unsigned char> SendInfos)
{
    for(int i = 0;i < SendInfos.size();i++)
    {
        Base.send(&SendInfos[i],1);
    }
}


// 串口数据接收(unsigned char、short int、int、long int、long long int)型数据
void SwanUart::Receive(unsigned char& ReceiveInfo)
{
    unsigned char ReceiveInfoGet;
    while(Base.receive(&ReceiveInfoGet,1) == 0);
    ReceiveInfos.push_back(ReceiveInfoGet);
    ReceiveInfo = ReceiveInfoGet;
}
void SwanUart::Receive(short int& ReceiveInfo)
{
    unsigned char ReceiveInfoGet[2];
    for(int i = 0;i < 2;i++)
    {
        while(Base.receive(ReceiveInfoGet+i,1) == 0);
        ReceiveInfos.push_back(ReceiveInfoGet[i]);
    }
    unsigned char* ReceiveInfoGet_p = ReceiveInfoGet;
    ReceiveInfo = *(short int*)ReceiveInfoGet_p;
}
void SwanUart::Receive(int& ReceiveInfo)
{
    unsigned char ReceiveInfoGet[4];
    
    for(int i = 0;i < 4;i++)
    {
        while(Base.receive(ReceiveInfoGet+i,1) == 0);
        ReceiveInfos.push_back(ReceiveInfoGet[i]);
    }
    unsigned char* ReceiveInfoGet_p = ReceiveInfoGet;
    ReceiveInfo = *(int*)ReceiveInfoGet_p;
}
void SwanUart::Receive(long int& ReceiveInfo)
{
    if(InitParameter.ComputerArch == X86_64 || InitParameter.ComputerArch == ARM_64)
    {
        unsigned char ReceiveInfoGet[8];
        for(int i = 0;i < 8;i++)
        {
            while(Base.receive(ReceiveInfoGet+i,1) == 0);
            ReceiveInfos.push_back(ReceiveInfoGet[i]);
        }
        unsigned char* ReceiveInfoGet_p = ReceiveInfoGet;
        ReceiveInfo = *(long int*)ReceiveInfoGet_p;
    }
    else
    {
        unsigned char ReceiveInfoGet[4];
        for(int i = 0;i < 4;i++)
        {
            while(Base.receive(ReceiveInfoGet+i,1) == 0);
            ReceiveInfos.push_back(ReceiveInfoGet[i]);
        }
        unsigned char* ReceiveInfoGet_p = ReceiveInfoGet;
        ReceiveInfo = *(long int*)ReceiveInfoGet_p;
    }
}
void SwanUart::Receive(long long int& ReceiveInfo)
{
    unsigned char ReceiveInfoGet[8];
    
    for(int i = 0;i < 8;i++)
    {
        while(Base.receive(ReceiveInfoGet+i,1) == 0);
        ReceiveInfos.push_back(ReceiveInfoGet[i]);
    }
    unsigned char* ReceiveInfoGet_p = ReceiveInfoGet;
    ReceiveInfo = *(long long int*)ReceiveInfoGet_p;
}



// 串口初始化信息获取
void SwanUart::initInfosGet(const char* DevicePath,_UartDeviceBaudrate_ UartDeviceBaudrate,_ComputerArchtecture_ ComputerArch)
{
    InitParameter.DevicePath = DevicePath;
    InitParameter.DeviceBaudrate = UartDeviceBaudrate;
    InitParameter.ComputerArch = ComputerArch;
}


// 串口发送数据清空
void SwanUart::sendInfosClear()
{
    SendInfos.clear();
}


// 串口接收数据清空
void SwanUart::receiveInfosClear()
{
    ReceiveInfos.clear();
}


// 串口发送数据添加(unsigned char、long long int、long int、int、short int输入)
void SwanUart::sendInfoAdd(unsigned char data)
{
    SendInfos.push_back(data);
}
void SwanUart::sendInfoAdd(long long int data)
{
    long long int* data_p = &data;
    unsigned char* data_add_p = (unsigned char*)data_p;
    for(int i = 0;i < 8;i++)
    {
        SendInfos.push_back(*(data_add_p+i));
    }
}
void SwanUart::sendInfoAdd(long int data)
{
    long int* data_p = &data;
    unsigned char* data_add_p = (unsigned char*)data_p;
    if(InitParameter.ComputerArch == X86_64 || InitParameter.ComputerArch == ARM_64)
    {
        for(int i = 0;i < 8;i++)
        {
            SendInfos.push_back(*(data_add_p+i));
        }
    }
    else
    {
        for(int i = 0;i < 4;i++)
        {
            SendInfos.push_back(*(data_add_p+i));
        }
    }
}
void SwanUart::sendInfoAdd(int data)
{
    int* data_p = &data;
    unsigned char* data_add_p = (unsigned char*)data_p;
    for(int i = 0;i < 4;i++)
    {
        SendInfos.push_back(*(data_add_p+i));
    }
}
void SwanUart::sendInfoAdd(short int data)
{
    short int* data_p = &data;
    unsigned char* data_add_p = (unsigned char*)data_p;
    for(int i = 0;i < 2;i++)
    {
        SendInfos.push_back(*(data_add_p+i));
    }
}



// 串口发送帧尾设置

// 串口发送信息添加


// 串口基础库
bool SwanUart::Base::open(const char* portname, int baudrate, char parity, char databit, char stopbit, char localmode, char outputmode, char inputmode, char synchronizeflag)
{
    // 打开串口
    SwanUart::Base::pHandle[0] = -1;
    // 以 读写、不阻塞 方式打开
    SwanUart::Base::pHandle[0] = ::open(portname,O_RDWR|O_NOCTTY|O_NONBLOCK);
    
    // 打开失败，则打印失败信息，返回false
    if(pHandle[0] == -1)
    {
        std::cout << portname << " open failed , may be you need 'sudo' permission." << std::endl;
        return false;
    }

    // 设置串口参数
    // 创建串口参数对象
    struct termios options;
    // 先获得串口的当前参数
    if(tcgetattr(pHandle[0],&options) < 0)
    {
        std::cout << portname << " open failed , get serial port attributes failed." << std::endl;
        return false;
    }

    // 设置波特率
    switch(baudrate)
    {
        case 4800:
            cfsetispeed(&options,B4800);
            cfsetospeed(&options,B4800);
            break;
        case 9600:
            cfsetispeed(&options,B9600);
            cfsetospeed(&options,B9600);
            break;   
        case 19200:
            cfsetispeed(&options,B19200);
            cfsetospeed(&options,B19200);
            break;
        case 38400:
            cfsetispeed(&options,B38400);
            cfsetospeed(&options,B38400);
            break;
        case 57600:
            cfsetispeed(&options,B57600);
            cfsetospeed(&options,B57600);
            break;
        case 115200:
            cfsetispeed(&options,B115200);
            cfsetospeed(&options,B115200);
            break;
        default:
            std::cout << portname << " open failed , unkown baudrate , only support 4800,9600,19200,38400,57600,115200." << std::endl;
            return false;
    }

    // 设置校验位
    switch(parity)
    {
        // 无校验
        case 0:
            options.c_cflag &= ~PARENB;//PARENB：产生奇偶位，执行奇偶校验
            options.c_cflag &= ~INPCK;//INPCK：使奇偶校验起作用
            break;
        // 设置奇校验
        case 1:
            options.c_cflag |= PARENB;//PARENB：产生奇偶位，执行奇偶校验
            options.c_cflag |= PARODD;//PARODD：若设置则为奇校验,否则为偶校验
            options.c_cflag |= INPCK;//INPCK：使奇偶校验起作用
            options.c_cflag |= ISTRIP;//ISTRIP：若设置则有效输入数字被剥离7个字节，否则保留全部8位
            break;
        // 设置偶校验
        case 2:
            options.c_cflag |= PARENB;//PARENB：产生奇偶位，执行奇偶校验
            options.c_cflag &= ~PARODD;//PARODD：若设置则为奇校验,否则为偶校验
            options.c_cflag |= INPCK;//INPCK：使奇偶校验起作用
            options.c_cflag |= ISTRIP;//ISTRIP：若设置则有效输入数字被剥离7个字节，否则保留全部8位
            break;
        default:
            std::cout << portname << " open failed , unkown parity ." << std::endl;
            return false;
    }

    // 设置数据位
    switch(databit)
    {
        case 5:
            options.c_cflag &= ~CSIZE;//屏蔽其它标志位
            options.c_cflag |= CS5;
            break;
        case 6:
            options.c_cflag &= ~CSIZE;//屏蔽其它标志位
            options.c_cflag |= CS6;
            break;
        case 7:
            options.c_cflag &= ~CSIZE;//屏蔽其它标志位
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag &= ~CSIZE;//屏蔽其它标志位
            options.c_cflag |= CS8;
            break;
        default:
            std::cout << portname << " open failed , unkown databit ." << std::endl;
            return false;
    }

    // 设置停止位
    switch(stopbit)
    {
        case 1:
            options.c_cflag &= ~CSTOPB;//CSTOPB：使用1位停止位
            break;
        case 2:
            options.c_cflag |= CSTOPB;//CSTOPB：使用2位停止位
            break;
        default:
            std::cout << portname << " open failed , unkown stopbit ." << std::endl;
            return false;
    }

    // 设置本地模式
    switch(localmode)
    {
        case 1:
            options.c_lflag |= (ICANON | ECHO | ECHOE | ISIG); // 规范输入模式：必须在每次发送数据之后再发送换行控制符才能从串口缓存中读取
        case 2:
            options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);    // 原始输入模式：将串口缓存直接读取
    }

    // 设置输出模式
    switch(outputmode)
    {
        case 1:
            options.c_oflag |= OPOST; // 规范输出模式
        case 2:
            options.c_oflag &= ~OPOST;    // 原始输出模式
    }

    // 设置控制字符屏蔽
    switch(inputmode)
    {
        case 1:
            options.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); // 屏蔽各种控制字符
        case 2:
            options.c_iflag |= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); // 不屏蔽各种控制字符
    }

    options.c_cflag |= CLOCAL | CREAD;

    // 激活新配置
    if((tcsetattr(pHandle[0],TCSANOW,&options))!=0) 
    { 
        std::cout << portname << " open failed , can not complete set attributes ." << std::endl;
        return false; 
    } 
    //std::cout << "---------------------UART OPEN SUCCESSED---------------------" << std::endl;
    return true;
}

void SwanUart::Base::close()
{
    if(pHandle[0] != -1)
    {
        ::close(pHandle[0]);
    }
}

int SwanUart::Base::send(const void *buf,int len)
{
    int sendCount = 0;
    if(pHandle[0] != -1)
    {   
        // 将 buf 和 len 转换成api要求的格式
        const char *buffer = (char*)buf;
        size_t length = len;
        // 已写入的数据个数
        ssize_t tmp;

        while(length > 0)
        {
            if((tmp = write(pHandle[0], buffer, length)) <= 0)
            {
                if(tmp < 0&&errno == EINTR)
                {
                    tmp = 0;
                }
                else
                {
                    break;
                }
            }
            length -= tmp;
            buffer += tmp;
        }

        sendCount = len - length;
    }
   
    return sendCount;
}

int SwanUart::Base::receive(void *buf,int maxlen)
{
    int receiveCount = ::read(pHandle[0],buf,maxlen);
    if(receiveCount < 0)
    {
        receiveCount = 0;
    }
    return receiveCount;
}