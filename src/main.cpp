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

    // 模型部署库类定义
    PPNCDetection PPNCDetection;
    NDTensor Tensor;

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

    // 参数获取
    SYNC.ConfigData_SYNC(Data_Path_p,Function_EN_p);
    JSON_FunctionConfigData JSON_FunctionConfigData = Function_EN_p -> JSON_FunctionConfigData_v[0];
    JSON_TrackConfigData JSON_TrackConfigData = Data_Path_p -> JSON_TrackConfigData_v[0];

    //摄像头初始化
     VideoCapture Camera; // 定义相机
    
    // 相机类型设置
    switch(JSON_FunctionConfigData.Camera_EN)
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

    // 模型初始化
    PPNC_ModelInit(PPNCDetection,Function_EN_p,JSON_FunctionConfigData.ModelDetection_EN);
    // 开启ModelDetection模型推理线程
    thread ModelDetection (ref(PPNC_ModelDetection_Thread),ref(PPNCDetection),Img_Store_p,Function_EN_p,ref(JSON_FunctionConfigData.ModelDetection_EN));
    ModelDetection.detach();
    
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
                    ImgProcess.ImgCompress((Img_Store_p -> Img_Color),(JSON_FunctionConfigData.ImgCompress_EN));   // 图像压缩：可在数据存储头文件中决定是否要进行图像压缩
                    ImgProcess.ImgPrepare(Img_Store_p,Data_Path_p,Function_EN_p,JSON_TrackConfigData.DilateErode_Factor[0],JSON_TrackConfigData.DilateErode_Factor[1]); // 图像预处理

                    if(Function_EN_p -> ThreadModelDetection_EN == true);   // 多线程推理结束
                    {
                        Function_EN_p -> ThreadModelDetection_EN = false;
                    }
                    
                    ImgSideSearch(Img_Store_p,Data_Path_p);   // 边线八邻域寻线

                    Img_Store_p -> ImgNum++;
                    Function_EN_p -> Loop_Kind_EN = JUDGE_LOOP;
                    waitKey(1);
                    break;
                }
                // 串口接收参数
                case UART_RECEIVE_LOOP:
                {
                    Uart.UartReceive(UartReceiveProtocol_p , JSON_FunctionConfigData.Uart_EN);   // 串口接收设置
                    SYNC.UartReceive_Change_To_Program_SYNC(UartReceiveProtocol_p , Data_Path_p , Function_EN_p); // 同步串口接收协议至程序数据
                    Function_EN_p -> Loop_Kind_EN = CAMERA_CATCH_LOOP;
                    break;
                }
                // 串口发送
                case UART_SEND_LOOP:
                {
                    DataPrint(Data_Path_p,Function_EN_p);
                    SYNC.UartSend_Program_To_Change_SYNC(UartSendProtocol_p , Data_Path_p , Function_EN_p); // 同步程序数据至串口发送协议
                    Uart.UartSend(UartSendProtocol_p , JSON_FunctionConfigData.Uart_EN);
                    Function_EN_p -> Loop_Kind_EN = UART_RECEIVE_LOOP;
                    break;
                }
            }
            
        }

        // 赛道状态机决策循环
        while( Function_EN_p -> Loop_Kind_EN == JUDGE_LOOP )
        {
            Function_EN_p -> Loop_Kind_EN = Judge.ModelTrack_Judge(PPNCDetection.results,Data_Path_p,Img_Store_p,Function_EN_p);  // 模型赛道决策
            Function_EN_p -> Loop_Kind_EN = Judge.TrackKind_Judge(Img_Store_p,Data_Path_p,Function_EN_p);  // 切换至赛道循环
        }

        // 普通赛道主循环
        while( Function_EN_p -> Loop_Kind_EN == COMMON_TRACK_LOOP )
        {
            if(Data_Path_p -> Circle_Track_Step == IN_PREPARE)
            {
                CircleTrack_Step_IN_Prepare_Stright(Img_Store_p,Data_Path_p);   // 准备入环补线
            }
            ImgPathSearch(Img_Store_p,Data_Path_p); // 赛道路径线寻线
            ImgProcess.ImgShow(Img_Store_p,Data_Path_p,Function_EN_p);    // 图像合成显示并保存
            Judge.ServoDirAngle_Judge(Data_Path_p); // 舵机角度计算
            Judge.MotorSpeed_Judge(Data_Path_p);    // 电机速度决策
            Function_EN_p -> Loop_Kind_EN = UART_SEND_LOOP; // 前换至串口发送循环
        }

        // 左右圆环赛道主循环
        while( Function_EN_p -> Loop_Kind_EN == L_CIRCLE_TRACK_LOOP || Function_EN_p -> Loop_Kind_EN == R_CIRCLE_TRACK_LOOP )
        {
            switch(Data_Path_p -> Circle_Track_Step)
            {
                case IN_PREPARE:
                {
                    CircleTrack_Step_IN_Prepare(Img_Store_p,Data_Path_p);   // 准备入环补线
                    break;
                }
                case IN:
                {
                    CircleTrack_Step_IN(Img_Store_p,Data_Path_p);   // 入环补线
                    break;
                }
            }
            ImgPathSearch(Img_Store_p,Data_Path_p); // 赛道路径线寻线
            ImgProcess.ImgShow(Img_Store_p,Data_Path_p,Function_EN_p);    // 图像合成显示并保存
            Judge.ServoDirAngle_Judge(Data_Path_p); // 舵机角度计算
            Judge.MotorSpeed_Judge(Data_Path_p);    // 电机速度决策
            if(Data_Path_p -> Circle_Track_Step == OUT)
            {
                CircleTrack_Step_OUT(Img_Store_p,Data_Path_p);   // 出环打角
            }
            Function_EN_p -> Loop_Kind_EN = UART_SEND_LOOP; // 切换至串口发送循环
        }

        // 十字赛道主循环
        while( Function_EN_p -> Loop_Kind_EN == ACROSS_TRACK_LOOP )
        {
            AcrossTrack(Img_Store_p,Data_Path_p);   // 十字赛道补线
            ImgPathSearch(Img_Store_p,Data_Path_p); // 赛道路径线寻线
            ImgProcess.ImgShow(Img_Store_p,Data_Path_p,Function_EN_p);    // 图像合成显示并保存
            Judge.ServoDirAngle_Judge(Data_Path_p); // 舵机角度计算
            Judge.MotorSpeed_Judge(Data_Path_p);    // 电机速度决策
            Function_EN_p -> Loop_Kind_EN = UART_SEND_LOOP; // 切换至串口发送循环
        }

        // AI赛道主循环
        while( Function_EN_p -> Loop_Kind_EN == MODEL_TRACK_LOOP)
        {   
            /* 
                AI 赛道元素执行时不使用传统元素寻线等，只进行摄像头采集
            */
            switch(Data_Path_p -> Model_Zone_Kind)
            {
                case BRIDGE_ZONE:{ Bridge_Zone(Img_Store_p,Data_Path_p); ImgPathSearch(Img_Store_p,Data_Path_p); Judge.ServoDirAngle_Judge(Data_Path_p); break; }
                case CROSSWALK_ZONE:{ Crosswalk_Zone(Img_Store_p,Data_Path_p); ImgPathSearch(Img_Store_p,Data_Path_p); Judge.ServoDirAngle_Judge(Data_Path_p); break; }
                case DANGER_ZONE:{ break; }
                case RESCURE_ZONE:{ break; }
                case CHASE_ZONE:{ break; }
            }
            ImgProcess.ImgShow(Img_Store_p,Data_Path_p,Function_EN_p);    // 图像合成显示并保存
            Function_EN_p -> Loop_Kind_EN = UART_SEND_LOOP; // 切换至串口发送循环
        }
    }

    return 0;
}
