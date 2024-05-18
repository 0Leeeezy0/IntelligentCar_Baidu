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

    // 摄像头初始化
    VideoCapture Camera; // 定义相机
    CameraInit(Camera,JSON_FunctionConfigData.Camera_EN,120);
    // 开启摄像头图像获取线程
    thread CameraImgCapture (CameraImgGetThread,ref(Camera),ref(Img_Store_p -> Img_Capture));
    CameraImgCapture.detach();

    // 模型初始化
    PPNC_ModelInit(PPNCDetection,Function_EN_p,JSON_FunctionConfigData.ModelDetection_EN);
    // 开启ModelDetection模型推理线程
    thread ModelDetection (PPNC_ModelDetection_Thread,ref(PPNCDetection),Img_Store_p,Function_EN_p,ref(JSON_FunctionConfigData.ModelDetection_EN));
    ModelDetection.detach();
    
    Function_EN_p -> Game_EN = true;
    Function_EN_p -> Loop_Kind_EN = UART_RECEIVE_LOOP;

    while(Function_EN_p -> Game_EN == true)
    {   
        // 图像主循环 / 串口接收循环 / 串口发送循环
        while( Function_EN_p -> Loop_Kind_EN == CAMERA_CATCH_LOOP || Function_EN_p -> Loop_Kind_EN == UART_RECEIVE_LOOP || Function_EN_p -> Loop_Kind_EN == UART_SEND_LOOP || Function_EN_p -> Loop_Kind_EN == IMG_SHOW_STORE_LOOP)
        {
            switch(Function_EN_p -> Loop_Kind_EN)
            {  
                // 图像获取
                case CAMERA_CATCH_LOOP:
                {
                    Img_Store_p -> Img_Color = (Img_Store_p -> Img_Capture).clone();
                    ImgProcess.ImgRealFPS(Img_Store_p,true);
                    ImgProcess.ImgCompress(Img_Store_p -> Img_Color,JSON_FunctionConfigData.ImgCompress_EN);
                    ImgProcess.ImgPrepare(Img_Store_p,Data_Path_p,Function_EN_p); // 图像预处理

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
                    ImgProcess.ImgRealFPS(Img_Store_p,false);
                    ImgProcess.ImgShow(Img_Store_p,Data_Path_p,Function_EN_p);    // 图像合成显示并保存
                    DataPrint(Data_Path_p,Function_EN_p);
                    SYNC.UartSend_Program_To_Change_SYNC(UartSendProtocol_p , Data_Path_p , Function_EN_p); // 同步程序数据至串口发送协议
                    Uart.UartSend(UartSendProtocol_p , JSON_FunctionConfigData.Uart_EN);
                    Function_EN_p -> Loop_Kind_EN = UART_RECEIVE_LOOP;
                    break;
                }
                // 图像显示存储：专用于救援区
                case IMG_SHOW_STORE_LOOP:
                {
                    ImgProcess.ImgRealFPS(Img_Store_p,false);
                    ImgProcess.ImgShow(Img_Store_p,Data_Path_p,Function_EN_p);    // 图像合成显示并保存
                    DataPrint(Data_Path_p,Function_EN_p);
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

            // 继续进行多线程推理
            if(Function_EN_p -> ThreadModelDetection_EN == true);   
            {
                Function_EN_p -> ThreadModelDetection_EN = false;
            }
        }

        // 普通赛道主循环
        while( Function_EN_p -> Loop_Kind_EN == COMMON_TRACK_LOOP )
        {
            if(Data_Path_p -> Circle_Track_Step == IN_PREPARE)
            {
                CircleTrack_Step_IN_Prepare_Stright(Img_Store_p,Data_Path_p);   // 准备入环补线
            }
            if(Data_Path_p -> Circle_Track_Step == OUT_2_STRIGHT)
            {
                Circle2CommonTrack(Img_Store_p,Data_Path_p);    // 出环转直线补线
            }
            ImgPathSearch(Img_Store_p,Data_Path_p); // 赛道路径线寻线
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
                case OUT:
                {
                    CircleTrack_Step_OUT(Img_Store_p,Data_Path_p);   // 出环补线
                    break;
                }
            }
            ImgPathSearch(Img_Store_p,Data_Path_p); // 赛道路径线寻线
            Judge.ServoDirAngle_Judge(Data_Path_p); // 舵机角度计算
            Judge.MotorSpeed_Judge(Data_Path_p);    // 电机速度决策
            Function_EN_p -> Loop_Kind_EN = UART_SEND_LOOP; // 切换至串口发送循环
        }

        // 十字赛道主循环
        while( Function_EN_p -> Loop_Kind_EN == ACROSS_TRACK_LOOP )
        {
            AcrossTrack(Img_Store_p,Data_Path_p);   // 十字赛道补线
            ImgPathSearch(Img_Store_p,Data_Path_p); // 赛道路径线寻线
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
                case BRIDGE_ZONE:{ Bridge_Zone(Img_Store_p,Data_Path_p); break; }
                case CROSSWALK_ZONE:{ Crosswalk_Zone(Img_Store_p,Data_Path_p); break; }
                case DANGER_ZONE:{ Danger_Zone(PPNCDetection,Img_Store_p,Data_Path_p,Function_EN_p); break; }
                case RESCUE_ZONE:{ Rescue_Zone(PPNCDetection,Img_Store_p,Data_Path_p,Function_EN_p); break; }
                case CHASE_ZONE:{ break; }
            }
            Function_EN_p -> Loop_Kind_EN = UART_SEND_LOOP; // 切换至串口发送循环
        }
    }

    return 0;
}
