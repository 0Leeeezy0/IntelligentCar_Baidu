#ifndef _UART_H_
#define _UART_H_


namespace swan   // Swan
{
    // 串口波特率
    typedef enum _UartDeviceBaudrate_ 
    {
        B_4800  = 4800,
        B_9600 = 9600,
        B_14400 = 14400,
        B_19200 = 19200,
        B_38400 = 38400,
        B_56000 = 56000,
        B_57600 = 57600,
        B_115200 = 115200
    }_UartDeviceBaudrate_;

    // 计算机平台硬件架构类型
    typedef enum _ComputerArchtecture_
    {
        X86 = 0,
        X86_64 = 1,
        ARM_32 = 2,
        ARM_64 = 3
    }_ComputerArchtecture_;

    class SwanUart
    {
        public:
            /*
                串口设备初始化
                @返回值说明
                返回串口初始化结果：0.初始化失败 1.初始化成功
            */ 
            int Init();


            /*
                串口发送
                由于发送的数据需要在工程各处添加，因此设立该函数可以统一发送
                1.使用SendInfoAdd添加串口数据 2.传入SnedInfo
                @参数说明
                SendInfos 串口发送数据容器
            */
            void Send();
            void Send(std::vector<unsigned char> SendInfos);


            /*
                串口接收
                由于接收的数据不需要在工程各处多次解析，因此设立该函数在接收阶段就将数据解析完成
                @参数说明
                ReceiveInfo 接收的数据(unsigned char、short int、int、long int、long long int)型数据
            */ 
            void Receive(unsigned char& ReceiveInfo);
            void Receive(short int& ReceiveInfo);
            void Receive(int& ReceiveInfo);
            void Receive(long int& ReceiveInfo);
            void Receive(long long int& ReceiveInfo);


            /*
                串口初始化信息获取
                @参数说明
                UartDevicePath 串口设备路径
                UartDeviceBaudrate 串口设备波特率
                ComputerArch 计算机平台硬件架构
            */
            void initInfosGet(const char* UartDevicePath,_UartDeviceBaudrate_ UartDeviceBaudrate,_ComputerArchtecture_ ComputerArch);


            /*
                串口数据清空
            */ 
            void sendInfosClear();


            /*
                串口接收数据清空
            */
            void receiveInfosClear();


            /*
                串口发送数据添加
                @参数说明
                data (unsigned char、long long int、long int、int、short int)型数据
            */
            void sendInfoAdd(unsigned char data);
            void sendInfoAdd(long long int data);
            void sendInfoAdd(long int data);
            void sendInfoAdd(int data);
            void sendInfoAdd(short int data);

        private: 
            
            // 串口初始化信息
            typedef struct _InitParameter_
            {
                const char* DevicePath;  // 串口设备路径
                _UartDeviceBaudrate_ DeviceBaudrate;    // 串口设备波特率
                _ComputerArchtecture_ ComputerArch; // 计算机平台硬件架构
            }_InitParameter_;

            _InitParameter_ InitParameter;

            std::vector<unsigned char> SendInfos;   // 串口发送数据

            std::vector<unsigned char> ReceiveInfos;    // 串口接收数据

            class Base
            {
                public:
                    // 打开串口,成功返回true，失败返回false
                    // portname(串口名): 在Windows下是"COM1""COM2"等，在Linux下是"/dev/ttyS1"等
                    // baudrate(波特率): 9600、19200、38400、43000、56000、57600、115200 
                    // parity(校验位): 0为无校验，1为奇校验，2为偶校验，3为标记校验（仅适用于windows)
                    // databit(数据位): 4-8(windows),5-8(linux)，通常为8位
                    // stopbit(停止位): 1为1位停止位，2为2位停止位,3为1.5位停止位
                    // inputmode(本地模式): 1为规范输入模式，2为原始输入模式
                    // outputmode(输出模式): 1为规范输出模式，2为原始输出模式
                    // inputmode(输入模式): 1位屏蔽控制字符，2位不屏蔽控制字符
                    // synchronizeflag(同步、异步,仅适用与windows): 0为异步，1为同步
                    bool open(const char* portname, int baudrate, char parity, char databit, char stopbit, char localmode, char outputmode, char inputmode, char synchronizeflag=1);

                    //关闭串口，参数待定
                    void close();
                
                    //发送数据或写数据，成功返回发送数据长度，失败返回0
                    int send(const void *buf,int len);

                    //接受数据或读数据，成功返回读取实际数据的长度，失败返回0
                    int receive(void *buf,int maxlen);

                private:

                    int pHandle[16];

                    char synchronizeflag;
            };

            Base Base;
    };
}

#endif