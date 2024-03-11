#include "common_system.h"
#include "common_program.h"

using namespace std;
using namespace cv;

int main()
{
    // 类定义
    Uart Uart;
    ImgProcess ImgProcess;
    Judge Judge;
    SYNC SYNC;
    // 数据结构体指针定义
    Img_Store Img_Store_c; 
    Img_Store *Img_Store_p = &Img_Store_c;
    Function_EN Function_EN_c;
    Function_EN *Function_EN_p = &Function_EN_c;
    Data_Path Data_Path_c;
    Data_Path *Data_Path_p = &Data_Path_c;
    UartSendProtocol UartSendProtocol_c;
    UartSendProtocol *UartSendProtocol_p = &UartSendProtocol_c;
    UartReceiveProtocol UartReceiveProtocol_c;
    UartReceiveProtocol *UartReceiveProtocol_p = &UartReceiveProtocol_c;
    Flag Flag_c;
    Flag *Flag_p = &Flag_c; 

    //摄像头初始化
     VideoCapture Camera; // 定义相机
    
    // 相机类型设置
    switch(CAMERA)
    {
        case DEMO_VIDEO:{ Camera.open("../img/sample.mp4"); break; }    // 演示视频
        case PC_CAMERA:{ Camera.open(0); break; }  // 开发端摄像头
        case USB_PC_CAMERA:{ Camera.open(2); break; }  // 开发端外接摄像头
        case USB_EB_CAMERA:{ Camera.open(0); break; }  // EB端外接摄像头
    }
    
    if (!Camera.isOpened())
    {
        cout << "<---------------------相机初始化失败--------------------->" << endl;
        abort();
    }
    else
    {
        cout << "<---------------------相机初始化成功--------------------->" << endl;
    }
    
    Function_EN_p -> Game_EN = true;
    Function_EN_p -> Loop_Kind_EN = UART_RECEIVE_LOOP;

    while(Function_EN_p -> Game_EN == true)
    {   
        // 图像主循环 / 串口接收循环 / 串口发送循环
        while( Function_EN_p -> Loop_Kind_EN == CAMERA_CATCH_LOOP || Function_EN_p -> Loop_Kind_EN == UART_RECEIVE_LOOP || Function_EN_p -> Loop_Kind_EN == UART_SEND_LOOP)
        {
            switch(Function_EN_p -> Loop_Kind_EN)
            {  
                // 图像获取
                case CAMERA_CATCH_LOOP:
                {
                    Camera >> (Img_Store_p -> Img_Color);   // 将视频流转为图像流
                    ImgProcess.ImgCompress((Img_Store_p -> Img_Color),(Function_EN_p -> ImgCompress_EN));   // 图像压缩：可在数据存储头文件中决定是否要进行图像压缩
                    ImgProcess.ImgPrepare(Img_Store_p,Data_Path_p); // 图像预处理

                    ImgSideSearch(Img_Store_p,Data_Path_p);   // 边线八邻域寻线

                    Img_Store_p -> ImgNum++;
                    Function_EN_p -> Loop_Kind_EN = JUDGE_LOOP;
                    waitKey(1);
                    break;
                }
                // 串口接收参数
                case UART_RECEIVE_LOOP:
                {
                    Uart.UartReceive(UartReceiveProtocol_p , (Function_EN_p -> Uart_EN));   // 串口接收设置
                    SYNC.UartReceive_Change_To_Program_SYNC(UartReceiveProtocol_p , Data_Path_p , Function_EN_p); // 同步串口接收协议至程序数据
                    Function_EN_p -> Loop_Kind_EN = CAMERA_CATCH_LOOP;
                    break;
                }
                // 串口发送
                case UART_SEND_LOOP:
                {
                    Judge.ServoDirAngle_Judge(Data_Path_p);
                    DataPrint(Data_Path_p,Function_EN_p);
                    SYNC.UartSend_Program_To_Change_SYNC(UartSendProtocol_p , Data_Path_p , Function_EN_p); // 同步程序数据至串口发送协议
                    Uart.UartSend(UartSendProtocol_p , (Function_EN_p -> Uart_EN));
                    Function_EN_p -> Loop_Kind_EN = UART_RECEIVE_LOOP;
                    break;
                }
            }
            
        }

        // 赛道状态机决策循环
        while( Function_EN_p -> Loop_Kind_EN == JUDGE_LOOP )
        {
            Function_EN_p -> Loop_Kind_EN = Judge.TrackKind_Judge_Vector(Img_Store_p,Data_Path_p);  // 切换至赛道循环
            ImgProcess.ImgInterrupt(Img_Store_p,Data_Path_p);    // 边线断点绘制
        }

        // 普通赛道主循环
        while( Function_EN_p -> Loop_Kind_EN == COMMON_TRACK_LOOP )
        {
            Data_Path_p -> MotorSpeed = 30;
            ImgPathSearch(Img_Store_p,Data_Path_p); // 赛道路径线寻线
            ImgProcess.ImgShow(Img_Store_p,Data_Path_p);    // 图像合成显示并保存
            Function_EN_p -> Loop_Kind_EN = UART_SEND_LOOP; // 前换至串口发送循环
        }

        // 左右圆环赛道主循环
        while( Function_EN_p -> Loop_Kind_EN == L_CIRCLE_TRACK_LOOP || Function_EN_p -> Loop_Kind_EN == R_CIRCLE_TRACK_LOOP)
        {
            Data_Path_p -> MotorSpeed = 20;
            Data_Path_p -> Circle_Track_Step = CircleTrack_Step_Judge(Img_Store_p,Data_Path_p);
            switch(Data_Path_p -> Circle_Track_Step)
            {
                case IN_PREPARE:
                {
                    CircleTrack_Step_IN_Prepare(Img_Store_p,Data_Path_p);   // 准备入环补线
                    ImgPathSearch(Img_Store_p,Data_Path_p); // 赛道路径线寻线
                    break;
                }
                case IN:
                {
                    CircleTrack_Step_IN(Img_Store_p,Data_Path_p);   // 入环补线
                    ImgPathSearch(Img_Store_p,Data_Path_p); // 赛道路径线寻线
                    break;
                }
                case OUT:
                {
                    // CircleTrack_Step_OUT(Img_Store_p,Data_Path_p);   // 出环补线
                    ImgPathSearch(Img_Store_p,Data_Path_p); // 赛道路径线寻线
                }
            }
            ImgProcess.ImgShow(Img_Store_p,Data_Path_p);    // 图像合成显示并保存
            Function_EN_p -> Loop_Kind_EN = UART_SEND_LOOP; // 前换至串口发送循环
        }

        // 十字赛道主循环
        while( Function_EN_p -> Loop_Kind_EN == ACROSS_TRACK_LOOP )
        {
            Data_Path_p -> MotorSpeed = 20;
            AcrossTrack(Img_Store_p,Data_Path_p);
            Function_EN_p -> Loop_Kind_EN = UART_SEND_LOOP; // 前换至串口发送循环
        }

        // AI赛道主循环
        while( Function_EN_p -> Loop_Kind_EN == AI_TRACK_LOOP )
        {
            // Data_Path_p -> Track_Kind = AI_TRACK;
        }
    }

    return 0;
}