#ifndef _LIBUART_EB_H_
#define _LIBUART_EB_H_

class Uart
{
    public:
        /*
            串口发送
            @参数说明
            UartSendProtocol_p 串口发送协议指针
            UartEN 串口使能
                使能 true
                失能 false
        */
        void UartSend(UartSendProtocol *UartSendProtocol_p,bool UartEN);


        /*
            串口接收
            @参数说明
            UartReceiveProtocol_p 串口接收协议指针
            UartEN 串口使能
                使能 true
                失能 false
        */
        void UartReceive(UartReceiveProtocol *UartReceiveProtocol_p,bool UartEN);
};

#endif