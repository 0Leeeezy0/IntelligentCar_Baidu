#include "libdata_store.h"

#ifndef _LIBDATA_PROCESS_H_
#define _LIBDATA_PROCESS_H_


class Judge
{
    public:

        /*
            赛道循环类型决策
            边线坐标向量法
            1.普通赛道类型
            2.圆环赛道类型
            3.十字赛道类型
            4.AI赛道类型
            @参数说明
            Img_Store_p 图像存储指针
            Data_Path_p 路径相关数据指针
            Function_EN_p 函数使能相关指针
            @返回值说明
            返回赛道循环类型
        */
        LoopKind TrackKind_Judge_Vector(Img_Store* Img_Store_p,Data_Path *Data_Path_p,Function_EN* Function_EN_p);


        /*
            模型赛道决策
            @参数说明
            results 模型预测结果容器
            Data_Path_p 路径相关数据指针
            @返回值说明
            返回赛道类型
        */
        LoopKind ModelTrack_Judge(std::vector<PredictResult> results,Data_Path *Data_Path_p);


        /*
            计算舵机方向和角度
            @参数说明
            Data_Path_p 路径相关数据指针
        */
        void ServoDirAngle_Judge(Data_Path *Data_Path_p);
};


class SYNC
{
    public:
        /*
            串口接收协议数据交换区和数据位同步
            unsigned char -> int
            @参数说明
            UartReceiveProtocol_p 串口接收协议指针
        */
        void UartReceive_Bit_To_Change_SYNC(UartReceiveProtocol *UartReceiveProtocol_p);


        /*
            串口发送协议数据交换区和数据位同步
            int -> unsigned char
            @参数说明
            UartSendProtocol_p 串口发送协议指针
        */
        void UartSend_Change_To_Bit_SYNC(UartSendProtocol *UartSendProtocol_p);


        /*
            同步程序内部数据至串口发送数据交换区
            @参数说明
            UartSendProtocol_p 串口发送协议指针
            Data_Path_p 路径相关数据指针
            Function_EN_p 函数使能指针
        */
        void UartSend_Program_To_Change_SYNC(UartSendProtocol *UartSendProtocol_p , Data_Path *Data_Path_p , Function_EN *Function_EN_p);


        /*
            同步串口接收数据交换区至程序内部数据
            @参数说明
            UartReceiveProtocol_p 串口发送协议指针
            Data_Path_p 路径相关数据指针
            Function_EN_p 函数使能指针
        */
        void UartReceive_Change_To_Program_SYNC(UartReceiveProtocol *UartReceiveProtocol_p , Data_Path *Data_Path_p , Function_EN *Function_EN_p);
};


/*
    打印数据
    程序参数：1.前瞻点 2.寻边线起始点 3.寻边线结束点 4.边线断点起始点 5.边线断点结束点 6.比赛状态
    运动参数：1.舵机方向 2.舵机角度 3.点击速度
    @参数说明
    Data_Path_p 路径相关数据指针
    Function_EN_p 函数使能指针
    @注意
    使用前必须使用 UartReceive_Change_To_Program_SYNC() 同步数据 和 ServoDirAngle_Judge() 计算运动参数
*/
void DataPrint(Data_Path *Data_Path_p,Function_EN *Function_EN_p);

#endif

