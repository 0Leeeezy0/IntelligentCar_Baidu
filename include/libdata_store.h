#include "common_system.h"
#include "common_program.h"

#ifndef _LIBDATA_STORE_H_
#define _LIBDATA_STORE_H_

// 串口接收初始化的调试参数
#define Forward_Default 55  // 调试参数
#define Path_Search_Start_Default 30   // 调试参数
#define Path_Search_End_Default 140 // 调试参数
#define Side_Search_Start_Default 10   // 调试参数
#define Side_Search_End_Default 200   // 调试参数
#define Game_EN_Default 1   // 调试参数

#define BAUDRATE 115200 // 串口波特率
#define PI 3.1415926    // 圆周率
#define FPS_TIME 1 // 帧间时间
#define DILATE_FACTOR 0 // 边线图形学膨胀系数
#define ERODE_FACTOR 2 // 边线图形学腐蚀系数

// 非串口接收初始化的调试参数
#define UART_EN false    // 串口
#define IMGCOMPRESS_EN true // 图像压缩
#define UNPIVOT_EN true // 暂时没有用
#define CAMERA USB_PC_CAMERA // 相机采集类型 摄像头/视频

/*
    相机类型
*/
typedef enum CameraKind
{
    DEMO_VIDEO = 0, // 演示视频
    PC_CAMERA = 1,  // 开发端摄像头
    USB_PC_CAMERA = 2,  // 开发端外接摄像头
    USB_EB_CAMERA = 3   // EB端摄像头
}CameraKind;

/*
    主函数循环类型(状态机)
*/
typedef enum LoopKind
{
    CAMERA_CATCH_LOOP = 0,   // 图像循环
    UART_RECEIVE_LOOP = 1,   // 串口接收循环
    UART_SEND_LOOP = 2,   // 串口发送循环
    JUDGE_LOOP = 3,    // 决策循环
    COMMON_TRACK_LOOP = 4,   // 普通赛道循环
    R_CIRCLE_TRACK_LOOP = 5,   // 右圆环赛道循环
    L_CIRCLE_TRACK_LOOP = 6,   // 左圆环赛道循环
    ACROSS_TRACK_LOOP = 7,   // 十字赛道循环
    MODEL_TRACK_LOOP = 8   // 模型赛道循环
}LoopKind;


/*
    发送赛道类型
*/
typedef enum TrackKind
{
    COMMON_TRACK = 0,   // 普通赛道
    R_CIRCLE_TRACK = 1,   // 右圆环赛道
    L_CIRCLE_TRACK = 2,   // 左圆环赛道
    ACROSS_TRACK = 3,   // 十字赛道
    MODEL_TRACK = 4,   // 模型赛道
}TrackKind;

/*
    圆环入环步骤
*/
typedef enum CircleTrackStep
{
    IN_PREPARE = 0, // 准备入环
    IN = 1, // 入环
    OUT = 2, // 出环
    INIT = 3,   // 占位
}CircleTrackStep;

/*
    模型赛道区域类型
*/
typedef enum ModelZoneKind
{
    Model_Bridge_Zone = 0,  // 桥梁区
    Model_Crosswalk_Zone = 1,   // 斑马线区
    Model_Danger_Zone = 2,  // 危险区
    Model_Rescue_Zone = 3,  // 救援区
    Model_Chase_Zone = 4,   // 追逐区
}ModelZoneKind;

/*
    模型推理元素
*/
typedef enum ModelDetectionElement
{
    Cone_L = 0, // 左锥桶
    Cone_R = 1, // 右锥桶
    Block_L = 2, // 左路障
    Block_R = 3, // 右路障
    Garage_L = 4,   // 左车库
    Garage_R = 5    // 右车库
}ModelDetectionElement;

/*
    图像存储
*/
typedef struct Img_Store
{
    cv::Mat Img_Color;  // 使用
    cv::Mat Img_Color_Unpivot;  // 使用
    cv::Mat Img_Gray;
    cv::Mat Img_Gray_Unpivot;   // 使用
    cv::Mat Img_OTSU;
    cv::Mat Img_OTSU_Unpivot;   // 使用
    cv::Mat Img_Track;
    cv::Mat Img_Track_Unpivot;  //使用
    cv::Mat Img_All;    // 使用
    cv::Mat Dilate_Kernel = getStructuringElement(cv::MORPH_CROSS,cv::Size(2,2));  // 边线形态学膨胀核大小
    cv::Mat Erode_Kernel = getStructuringElement(cv::MORPH_CROSS,cv::Size(2,2));  // 边线形态学腐蚀核大小
    int ImgNum;
}Img_Store;

/*
    标志位设置
*/
typedef struct Flag
{
    bool CameraInit;
}Flag;

/*
    函数使能
*/
typedef struct Function_EN
{
    bool Uart_EN = UART_EN; // 串口使能
    bool Unpivot_EN = UNPIVOT_EN;   // 逆透视使能
    bool ImgCompress_EN = IMGCOMPRESS_EN;   // 图像压缩使能
    bool Game_EN;   // 比赛开始
    LoopKind Loop_Kind_EN;  // 循环类型使能：0.图像循环 1.普通赛道循环 2.圆环赛道循环 3.十字赛道循环 4.AI赛道循环 5.串口发送循环
}Function_EN;

/*
    路径相关数据
*/
typedef struct Data_Path
{
    int Forward;    // 前瞻点
    int Path_Search_Start; // 寻路径起始点
    int Path_Search_End;   // 寻路径结束点
    int Side_Search_Start; // 寻边线起始点
    int Side_Search_End; // 寻边线结束点
    int Side_Width; // 左右边线起始点距离
    int SideCoordinate[10000][4] = {0};   // 左右边线坐标(中线寻线法)
    int SideCoordinate_Eight[10000][4] = {0};   // 左右边线坐标(八邻域)
    int NumSearch[2] = {0}; // 左右八邻域寻线坐标数量
    int CircleRecord[100000][2] = {0}; // 左右圆环赛道记录
    int TrackCoordinate[10000][2] = {0};   // 路径线坐标
    int InterruptCoordinate[10000][4] = {0};  // 左右边线断点坐标
    int InterruptNum[2] = {0};    // 边线断点数量
    int CircleTime = 0; // 进入圆环的图像序号
    int ServoDir;  // 舵机方向
    int ServoAngle;    // 舵机角度
    int MotorSpeed;    // 电机速度
    TrackKind Track_Kind; // 赛道类型：1.普通赛道 2.左圆环赛道 3.右圆环赛道 4.十字赛道
    CircleTrackStep Circle_Track_Step = INIT;  // 圆环入环步骤：1.准备入环 2.入环 3.出环
    ModelZoneKind Model_Zone_Kind;    // 模型赛道区域类型
    // vector<Model_Detection_Element> Model_Detection_Element;    // 模型预测元素
}Data_Path;

/*
    串口通讯发送协议
*/
typedef struct UartSendProtocol
{
    // 波特率
    int Baudrate = BAUDRATE;
    // 串口帧头
    unsigned char Head_1 = 0xA0; // 0xA0
    unsigned char Head_2 = 0xA1; // 0xA1

    // 数据交换区
    int ServoDir;  // 舵机方向
    int ServoAngle;    // 舵机角度
    int MotorSpeed;    // 电机速度
    int Track_Kind;  // 赛道类型

    // 串口数据位
    unsigned char Data_1;
    unsigned char Data_2;
    unsigned char Data_3;
    unsigned char Data_4;

    //串口帧尾
    unsigned char CRC16 = 0XA2;    // 0xA2
}UartSendProtocol;


/*
    串口通讯接收协议
*/
typedef struct UartReceiveProtocol
{   
    // 波特率
    int Baudrate = BAUDRATE;
    // 串口帧头
    unsigned char Head_1;   // 0XA0
    unsigned char Head_2;   // 0XA1

    // 数据交换区
    int Forward;    // 前瞻点
    int Path_Search_Start;  // 寻路径起始点
    int Path_Search_End;    // 寻路径结束点
    int Side_Search_Start; // 寻边线起始点
    int Side_Search_End; // 寻边线结束点
    int Game_EN;    // 比赛开始

    // 串口数据位
    unsigned char Data_1;
    unsigned char Data_2;
    unsigned char Data_3;
    unsigned char Data_4;
    unsigned char Data_5;
    unsigned char Data_6;

    //串口帧尾
    unsigned char CRC16;    // 0XA2
}UartReceiveProtocol;

#endif
