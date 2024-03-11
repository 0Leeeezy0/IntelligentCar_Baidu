#include "common_system.h"
#include "common_program.h"

WzSerialPort::WzSerialPort()
{
}

WzSerialPort::~WzSerialPort()
{

}

bool WzSerialPort::open(const char* portname, int baudrate, char parity, char databit, char stopbit, char loaclmode, char synchronizeflag)
{
    // 打开串口
    pHandle[0] = -1;
    // 以 读写、不阻塞 方式打开
    pHandle[0] = ::open(portname,O_RDWR|O_NOCTTY|O_NONBLOCK);
    
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
    switch(loaclmode)
    {
        case 1:
            options.c_lflag |= (ICANON | ECHO | ECHOE); // 规范输入模式：必须在每次发送数据之后再发送换行控制符才能从串口缓存中读取
        case 2:
            options.c_lflag &= ~(ICANON | ECHO | ECHOE);    // 原始输入模式：将串口缓存直接读取
    }

    // 激活新配置
    if((tcsetattr(pHandle[0],TCSANOW,&options))!=0) 
    { 
        std::cout << portname << " open failed , can not complete set attributes ." << std::endl;
        return false; 
    } 
    //std::cout << "---------------------UART OPEN SUCCESSED---------------------" << std::endl;
    return true;
}

void WzSerialPort::close()
{
    if(pHandle[0] != -1)
    {
        ::close(pHandle[0]);
    }
}

int WzSerialPort::send(const void *buf,int len)
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

int WzSerialPort::receive(void *buf,int maxlen)
{
    int receiveCount = ::read(pHandle[0],buf,maxlen);
    if(receiveCount < 0)
    {
        receiveCount = 0;
    }
    return receiveCount;
}