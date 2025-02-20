#include "common_system.h"
#include "common_program.h"

#ifndef _LIBDATA_STORE_H_
#define _LIBDATA_STORE_H_

// 串口接收初始化的调试参数
#define Game_EN_Default 1   // 调试参数

#define BAUDRATE 115200 // 串口波特率
#define PI 3.1415926    // 圆周率

/*
    相机类型
*/
typedef enum CameraKind
{
    DEMO_VIDEO = 0, // 演示视频
    VIDEO_0 = 1,  // USB摄像头1
    VIDEO_1 = 2,  // USB摄像头2
}CameraKind;

/*
    图像通道
*/
typedef enum RGB_Channel
{
    R_Channel = 0,  // 红色通道
    G_Channel = 1,  // 绿色通道
    B_Channel = 2   // 蓝色通道
}RGB_Channel;

/*
    主函数循环类型(状态机)
*/
typedef enum LoopKind
{
    CAMERA_CATCH_LOOP = 0,   // 图像循环
    UART_RECEIVE_LOOP = 1,   // 串口接收循环
    UART_SEND_LOOP = 2,   // 串口发送循环
    IMG_SHOW_STORE_LOOP = 3,    // 图像显示存储循环
    JUDGE_LOOP = 4,    // 决策循环
    COMMON_TRACK_LOOP = 5,   // 普通赛道循环
    R_CIRCLE_TRACK_LOOP = 6,   // 右圆环赛道循环
    L_CIRCLE_TRACK_LOOP = 7,   // 左圆环赛道循环
    ACROSS_TRACK_LOOP = 8,   // 十字赛道循环
    MODEL_TRACK_LOOP = 9   // 模型赛道循环
}LoopKind;


/*
    发送赛道类型
*/
typedef enum TrackKind
{
    STRIGHT_TRACK = 0,   // 直赛道
    BEND_TRACK = 1,   // 弯赛道
    R_CIRCLE_TRACK_OUTSIDE = 2,   // 右圆环赛道外
    R_CIRCLE_TRACK_INSIDE = 3,   // 右圆环赛道内
    L_CIRCLE_TRACK_OUTSIDE = 4,   // 左圆环赛道外
    L_CIRCLE_TRACK_INSIDE = 5,   // 左圆环赛道内
    ACROSS_TRACK = 6,   // 十字赛道
    MODEL_TRACK = 7,    // 模型赛道
}TrackKind;


/*
    圆环入环步骤
*/
typedef enum CircleTrackStep
{
    IN_PREPARE = 0, // 准备入环
    IN = 1, // 入环
    OUT_PREPARE = 2,     // 准备出环
    OUT = 3,    // 出环
    OUT_2_STRIGHT = 4,  // 出环转直道
    INIT = 5,   // 占位
}CircleTrackStep;


/*
    模型救援赛道车库方向
*/
typedef enum RescueZoneGarageDir
{
    RIGHT_GARAGE = 0,  // 右车库
    LEFT_GARAGE = 1,   // 左车库
}RescueZoneGarageDir;


/*
    模型斑马线赛道步骤
*/
typedef enum CrosswalkZoneStep
{
    STOP_PREPARE = 0, // 准备停车
    STOP = 1,   // 停车
}CrosswalkZoneStep;


/*
    模型追逐区赛道车辆类型
*/
typedef enum ChaseZoneCarKind
{

}ChaseZoneCarKind;


/*
    模型赛道区域类型
*/
typedef enum ModelZoneKind
{
    BRIDGE_ZONE = 0,  // 桥梁区,对应bridge
    CROSSWALK_ZONE = 1,   // 斑马线区,对应crosswalk
    DANGER_ZONE = 2,  // 危险区,对应bomb
    RESCUE_ZONE = 3,  // 救援区,对应patient,tumble,evil,thief
    CHASE_ZONE = 4,   // 追逐区,对应spy
}ModelZoneKind;


/*
    模型推理元素
*/
typedef enum ModelDetectionElement
{
    BOMB = 0,   // 炸弹
    BRIDGE = 1, // 桥梁
    SAFTY = 2, // 安全
    CONE = 3,  // 锥桶
    CROSSWALK = 4,  // 斑马线
    DANGER = 5,     // 危险
    EVIL = 6,   // 恶人
    BLOCK = 7,  // 路障
    PATIENT = 8,    // 病人
    PROP = 9,   // 道具
    SPY = 10,   // 间谍
    THIEF = 11, // 小偷
    TUMBLE = 12    // 翻滚
}ModelDetectionElement;

/*
    JSON文件存储的工程功能设置参数
*/
typedef struct JSON_FunctionConfigData
{
    bool Uart_EN; // 串口使能
    bool ImgCompress_EN;   // 图像压缩使能
    CameraKind Camera_EN;   // 相机使能
    bool VideoShow_EN;  // 图像显示使能
    bool ImageSave_EN;  // 图像存储使能
    bool DataPrint_EN;  // 数据显示使能
    bool AcrossIdentify_EN;    // 十字特征点识别使能
    bool CircleIdentify_EN;    // 圆环特征点识别使能
    bool ModelDetection_EN;     // 模型推理使能
    bool RescueZone_Lable_Detection_EN;  // 救援区域标志推理使能
    bool DangerZone_Cone_Detection_EN;  // 危险区域锥桶推理使能
}JSON_FunctionConfigData;

/*
    JSON文件存储的赛道识别设置参数
*/
typedef struct JSON_TrackConfigData
{
    int Forward;    // 前瞻点
    int Default_Forward;    // 默认前瞻点，用于前瞻点初始化
    int Path_Search_Start;  // 寻路径起始点
    int Path_Search_End;    // 寻路径结束点
    int Side_Search_Start; // 寻边线起始点
    int Side_Search_End; // 寻边线结束点
    int TrackWidth = 0; // 赛道宽度
    int CircleOutWidth = 0; // 圆环出环补线终点与中线距离
    int BendPointNum[2] = {0};   // 弯点数量
    int InflectionPointIdentifyAngle[2] = {0};    // 元素拐点识别角度
    int InflectionPointVectorDistance = 0;   // 边线元素拐点向量距离
    int BendPointIdentifyAngle[2] = {0};    // 边线弯点识别角度
    int BendPointVectorDistance = 0;   // 边线弯点向量距离
    int CommonMotorSpeed[6] = {0};    // 电机速度：0.直道 1.小角度弯道 2.大角度弯道 3.十字赛道 4.圆环赛道(外) 5.圆环赛道(内)
    int DilateErode_Factor[2] = {0};    // 图形学膨胀腐蚀系数
    float Filter_Factor = 0;  // 路径线滤波强度系数
    int BridgeZoneMotorSpeed = 0;   // 桥梁区域电机速度
    int DangerZoneMotorSpeed = 0;   // 危险区域电机速度
    int RescueZoneMotorSpeed = 0;   // 救援区域电机速度
    int CrosswalkZoneMotorSpeed = 0;    // 斑马线区域电机准备停车速度
    int Circle_In_Prepare_Time = 0;    // 准备入环限定时间
    int DangerTime = 0; // 进入危险区域的时间
    int BridgeTime = 0; // 进入桥梁区域的时间
    int RescueTime = 0; // 救援区进入车库前准备时间上限
    int CrosswalkStopTime = 0;  // 进入斑马线区域停车的时间
    int RescueGarageTime = 0;   // 获取救援区域过标志后与开始判断进车库时机的时间间隔
    int RescueZoneConeAvgY = 0;  // 救援区域锥桶平均高度阈值
    int Crosswalk_Y = 0;    // 斑马线识别纵坐标阈值
    int Bomb_Y = 0; // 爆炸物识别纵坐标阈值
    int Bridge_Y = 0;   // 桥识别纵坐标阈值
    int Rescue_Y = 0;   // 救援区域识别标识牌纵坐标阈值
    int Danger_Zone_Barrier_Y = 0;  // 危险区域障碍物准备避障纵坐标阈
    float Danger_Zone_Barrier_Servor_Angle_Factor[2];  // 危险区域障碍物避障舵机角度倍率 
    int DangerZone_Cone_Radius = 0; // 危险区域锥桶避障补线半径
    int DangerZone_Block_Radius = 0; // 危险区域路障避障补线半径
    int DangerZoneForward = 0;  // 危险区域前瞻值
    int BridgeZoneForward = 0; // 桥梁区域前瞻值
}JSON_TrackConfigData;

/*
    图像存储
*/
typedef struct Img_Store
{
    std::queue<cv::Mat> Img_Capture;    // 摄像头图像
    cv::Mat Img_Color;  // 使用
    cv::Mat Img_Color_Unpivot;
    cv::Mat Img_Gray;     // 使用
    cv::Mat Img_Gray_Unpivot; 
    cv::Mat Img_OTSU;     // 使用
    cv::Mat Img_OTSU_Unpivot;   // 使用 
    cv::Mat Img_Track;    // 使用
    cv::Mat Img_Track_Unpivot; 
    cv::Mat Img_Text;   // 使用
    cv::Mat Img_All;    // 使用
    cv::Mat Dilate_Kernel = getStructuringElement(cv::MORPH_CROSS,cv::Size(2,2));  // 边线形态学膨胀核大小
    cv::Mat Erode_Kernel = getStructuringElement(cv::MORPH_CROSS,cv::Size(2,2));  // 边线形态学腐蚀核大小
    int ImgNum;
}Img_Store;

/*
    函数使能
*/
typedef struct Function_EN
{
    std::vector<JSON_FunctionConfigData> JSON_FunctionConfigData_v;   // JSON文件存储的工程功能设置参数
    bool Game_EN;   // 比赛开始
    bool Gyroscope_EN;    // 陀螺仪状态使能：当陀螺仪积分到一定角度时出环
    LoopKind Loop_Kind_EN;  // 循环类型使能：0.图像循环 1.普通赛道循环 2.圆环赛道循环 3.十字赛道循环 4.AI赛道循环 5.串口发送循环
    bool ThreadModelDetection_EN;  // 多线程模型推理使能：当模型推理线程结束后才会使能，若模型推理线程还在进行则不使能
    bool Control_EN = false; // 控制权转移使能
    bool Garage_EN = true;  // 车库使能
}Function_EN;

/*
    路径相关数据
*/
typedef struct Data_Path
{
    std::vector<JSON_TrackConfigData> JSON_TrackConfigData_v; // JSON文件存储的赛道识别设置参数
    
    // 赛道识别结果
    // 边线结果
    int SideCoordinate[10000][4] = {0};   // 左右边线坐标(中线寻线法)
    int SideCoordinate_Eight[10000][4] = {0};   // 左右边线坐标(八邻域)
    int NumSearch[2] = {0}; // 左右八邻域寻线坐标数量
    int TrackCoordinate[10000][2] = {0};   // 路径线坐标
    int Vector_Add_Unit_Dir[2];   // 左右拐点上下两向量纵坐标加和方向
    int InflectionPointCoordinate[10000][4] = {0};  // 左右边线元素拐点坐标
    int InflectionPointNum[2] = {0};    // 元素拐点数量
    int BendPointNum[2] = {0};    // 边线弯点数量
    int BendPointCoordinate[10000][4] = {0};  // 左右边线弯点坐标
    TrackKind Track_Kind; // 赛道类型：1.直赛道 2.弯赛道 3.右圆环赛道外 4.右圆环赛道内 5.左圆环赛道外 6.左圆环赛道内 7.十字赛道 8.模型赛道
    CircleTrackStep Circle_Track_Step = INIT;  // 圆环入环步骤：1.准备入环 2.入环 3.出环
    TrackKind Previous_Circle_Kind; // 目前圆环类型
    // 控制参数
    int ServoDir = 0;  // 舵机方向
    int ServoAngle = 0;    // 舵机角度
    int MotorSpeed = 0;    // 电机速度

    // 模型赛道参数
    ModelZoneKind Model_Zone_Kind = CROSSWALK_ZONE;    // 模型赛道区域类型
    CrosswalkZoneStep Crosswalk_Zone_Step;    // 模型斑马线赛道步骤
    RescueZoneGarageDir Rescue_Zone_Garage_Dir = LEFT_GARAGE;    // 模型救援赛道车库方向
    ChaseZoneCarKind Chase_Zone_CarKind;    // 模型追逐区赛道步骤
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
    int Control_EN;    // 控制权转移使能
    RescueZoneGarageDir Rescue_Zone_Garage_Dir;    // 救援区域车库方向
    ChaseZoneCarKind Chase_Zone_Car_Kind;  // 追逐区域车辆类型

    // 串口数据位
    unsigned char Data_1;
    unsigned char Data_2;
    unsigned char Data_3;
    unsigned char Data_4;
    unsigned char Data_5;
    unsigned char Data_6;
    unsigned char Data_7;

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
    int Control_EN;    // 控制权转移使能
    int Gyroscope_EN;   // 陀螺仪状态
    int Game_EN;    // 比赛开始

    // 串口数据位
    unsigned char Data_1;
    unsigned char Data_2;
    unsigned char Data_3;

    //串口帧尾
    unsigned char CRC16;    // 0XA2
}UartReceiveProtocol;

#endif
