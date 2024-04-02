#include "common_system.h"
#include "common_program.h"

using namespace std;
using namespace cv;


/*
    TrackKind_Judge_Vector说明
    边线坐标向量法
    赛道循环类型决策
    1.普通赛道循环类型
    2.圆环赛道循环类型
    3.十字赛道循环类型
    4.AI赛道类型
*/
LoopKind Judge::TrackKind_Judge_Vector(Img_Store* Img_Store_p,Data_Path *Data_Path_p,Function_EN* Function_EN_p)
{
    LoopKind Loop_Kind;
    static int State = 0;   // 状态记录
    static int State_Across = 0;
    static int State_Circle = 0;
    static int OutTime = 0;    // 出环时间

    if(Function_EN_p -> Loop_Kind_EN != MODEL_TRACK_LOOP)
    {
        int i;
        int j;
        // static int Record;
        Data_Path_p -> ElementPointNum[0] = 0;
        Data_Path_p -> ElementPointNum[1] = 0;
        int Vector[2][4] = {0}; // 左右中断点与上下两点构成的向量坐标
        int Vector_Add_Unit[1][4];   // 左右中断点上下两向量加和
        int Vector_ScalarProduct[2] = {0};  // 左右中断点向量点乘
        float Vector_Module[4] = {0};   // 左右中断点向量的模
        float AngleVector[2] = {0}; // 左右中断点向量夹角(角度制)
        // 寻断点范围
        // 左边线断点
        for(i = 15;i <= (Data_Path_p -> NumSearch[0])-15;)
        {
            // 左边线第一个向量
            Vector[0][0] = (Data_Path_p -> SideCoordinate_Eight[i-15][0])-(Data_Path_p -> SideCoordinate_Eight[i][0]);
            Vector[0][1] = (Data_Path_p -> SideCoordinate_Eight[i-15][1])-(Data_Path_p -> SideCoordinate_Eight[i][1]);
            // 左边线第二个向量
            Vector[1][0] = (Data_Path_p -> SideCoordinate_Eight[i+15][0])-(Data_Path_p -> SideCoordinate_Eight[i][0]);
            Vector[1][1] = (Data_Path_p -> SideCoordinate_Eight[i+15][1])-(Data_Path_p -> SideCoordinate_Eight[i][1]);

            // 计算中断点向量点乘
            Vector_ScalarProduct[0] = Vector[0][0]*Vector[1][0]+Vector[0][1]*Vector[1][1];

            // 计算中断点向量的模
            Vector_Module[0] = sqrt(pow(Vector[0][0],2)+pow(Vector[0][1],2));
            Vector_Module[1] = sqrt(pow(Vector[1][0],2)+pow(Vector[1][1],2));
        
            if( Vector_Module[0]*Vector_Module[1] != 0)
            {
                AngleVector[0] = acos(Vector_ScalarProduct[0]/(Vector_Module[0]*Vector_Module[1]))*(180/PI);    // 左边线断点向量夹角
                // cout << AngleVector[0] << "  " << AngleVector[1] << endl;
            }

            // 计算拐点并存储坐标，前提：拐点坐标不再边框上
            if(abs(AngleVector[0]) > (Data_Path_p -> PointIdentifyAngle[0]) && abs(AngleVector[0]) < (Data_Path_p -> PointIdentifyAngle[1]) && (Data_Path_p -> SideCoordinate_Eight[i][0]) > 30 && (Vector[0][1] < 8 || Vector[1][1] < 8))
            {
                //  cout << abs(AngleVector[0]) << endl;
                (Data_Path_p -> ElementPointCoordinate[(Data_Path_p -> ElementPointNum[0])][0]) = (Data_Path_p -> SideCoordinate_Eight[i][0]);
                (Data_Path_p -> ElementPointCoordinate[(Data_Path_p -> ElementPointNum[0])][1]) = (Data_Path_p -> SideCoordinate_Eight[i][1]);
                if(Data_Path_p -> ElementPointNum[0] == 0)
                {
                    // 向量加和
                    Vector_Add_Unit[0][0] = (Vector[0][0]+Vector[1][0])/abs(Vector[0][0]+Vector[1][0]);
                    Vector_Add_Unit[0][1] = (Vector[0][1]+Vector[1][1])/abs(Vector[0][1]+Vector[1][1]);
                }
                Data_Path_p -> ElementPointNum[0]++;
                i = i+10;
            }
            i++;
        }
        // 右边线断点
        for(j = 15;j <= (Data_Path_p -> NumSearch[1])-15;)
        {
            // 右边线第一个向量
            Vector[0][2] = (Data_Path_p -> SideCoordinate_Eight[j-15][2])-(Data_Path_p -> SideCoordinate_Eight[j][2]);
            Vector[0][3] = (Data_Path_p -> SideCoordinate_Eight[j-15][3])-(Data_Path_p -> SideCoordinate_Eight[j][3]);
            // 右边线第二个向量
            Vector[1][2] = (Data_Path_p -> SideCoordinate_Eight[j+15][2])-(Data_Path_p -> SideCoordinate_Eight[j][2]);
            Vector[1][3] = (Data_Path_p -> SideCoordinate_Eight[j+15][3])-(Data_Path_p -> SideCoordinate_Eight[j][3]);

            // 计算中断点向量点乘
            Vector_ScalarProduct[1] = Vector[0][2]*Vector[1][2]+Vector[0][3]*Vector[1][3];

            // 计算中断点向量的模
            Vector_Module[2] = sqrt(pow(Vector[0][2],2)+pow(Vector[0][3],2));
            Vector_Module[3] = sqrt(pow(Vector[1][2],2)+pow(Vector[1][3],2));
        
            if( Vector_Module[2]*Vector_Module[3] != 0)
            {
                AngleVector[1] = acos(Vector_ScalarProduct[1]/(Vector_Module[2]*Vector_Module[3]))*(180/PI);    // 右边线断点向量夹角
                // cout << AngleVector[0] << "  " << AngleVector[1] << endl;
            }

            // 计算拐点点并存储坐标，前提：拐点坐标不在边框上
            if(abs(AngleVector[1]) > (Data_Path_p -> PointIdentifyAngle[0]) && abs(AngleVector[1]) < (Data_Path_p -> PointIdentifyAngle[1]) && 319-(Data_Path_p -> SideCoordinate_Eight[j][2]) > 30 && (Vector[0][3] < 8 || Vector[1][3] < 8))
            {
                // cout << abs(AngleVector[1]) << endl;
                (Data_Path_p -> ElementPointCoordinate[(Data_Path_p -> ElementPointNum[1])][2]) = (Data_Path_p -> SideCoordinate_Eight[j][2]);
                (Data_Path_p -> ElementPointCoordinate[(Data_Path_p -> ElementPointNum[1])][3]) = (Data_Path_p -> SideCoordinate_Eight[j][3]);
                if(Data_Path_p -> ElementPointNum[1] == 0)
                {
                    // 向量加和
                    Vector_Add_Unit[0][2] = (Vector[0][2]+Vector[1][2])/abs(Vector[0][2]+Vector[1][2]);
                    Vector_Add_Unit[0][3] = (Vector[0][3]+Vector[1][3])/abs(Vector[0][3]+Vector[1][3]);
                }
                Data_Path_p -> ElementPointNum[1]++;
                j = j+10;
            }
            j++;
        }

        // 若左右边线都有中断点则为十字
        if((Data_Path_p -> ElementPointNum[0] >= 1) && (Data_Path_p -> ElementPointNum[1] >= 1) && Function_EN_p -> AcrossIdentify_EN == true)
        {
            // Record = Img_Store_p -> ImgNum;
            State++;
            State_Across = State;
            Loop_Kind = ACROSS_TRACK_LOOP;
            Data_Path_p -> Track_Kind = ACROSS_TRACK;
            Data_Path_p -> Circle_Track_Step = INIT;
        }
        // 若左右边线只有一边有中断点  //且当前图像序号和十字中存储的图像序号有间隔才为左右圆环：防止误判
        else if((Data_Path_p -> ElementPointNum[0] == 0) && (Data_Path_p -> ElementPointNum[1] >= 1) && State - State_Across >= 50 && Function_EN_p -> CircleIdentify_EN == true)
        {
            State++;
            Data_Path_p -> CircleTime = Img_Store_p -> ImgNum;
            Loop_Kind = R_CIRCLE_TRACK_LOOP;
            Data_Path_p -> Track_Kind = R_CIRCLE_TRACK;
            if(((Data_Path_p -> Circle_Track_Step) == INIT || (Data_Path_p -> Circle_Track_Step) == IN_PREPARE) && Vector_Add_Unit[0][3] == 1)
            {
                Data_Path_p -> Circle_Track_Step = IN_PREPARE;
            }
            if(Vector_Add_Unit[0][3] == -1 && (Img_Store_p -> ImgNum)-OutTime >= 100 && (Data_Path_p -> Circle_Track_Step == IN_PREPARE || Data_Path_p -> Circle_Track_Step == IN))   
            {
                Data_Path_p -> Circle_Track_Step = IN;
            }   
            if(Vector_Add_Unit[0][3] == 1 && ((Data_Path_p -> Circle_Track_Step) == IN || (Data_Path_p -> Circle_Track_Step) == OUT))
            {
                Data_Path_p -> Circle_Track_Step = OUT;
                OutTime = Img_Store_p -> ImgNum;
            }
        }
        else if((Data_Path_p -> ElementPointNum[0] >= 1) && (Data_Path_p -> ElementPointNum[1] == 0) && State - State_Across >= 50 && Function_EN_p -> CircleIdentify_EN == true)
        {
            State++;
            Loop_Kind = L_CIRCLE_TRACK_LOOP;
            Data_Path_p -> Track_Kind = L_CIRCLE_TRACK;
            if(((Data_Path_p -> Circle_Track_Step) == INIT || (Data_Path_p -> Circle_Track_Step) == IN_PREPARE) && Vector_Add_Unit[0][1] == 1)
            {
                Data_Path_p -> Circle_Track_Step = IN_PREPARE;
            }
            if(Vector_Add_Unit[0][1] == -1 && (Img_Store_p -> ImgNum)-OutTime >= 100 && (Data_Path_p -> Circle_Track_Step == IN_PREPARE || Data_Path_p -> Circle_Track_Step == IN))   
            {
                Data_Path_p -> Circle_Track_Step = IN;
            }   
            if(Vector_Add_Unit[0][1] == 1 && ((Data_Path_p -> Circle_Track_Step) == IN || (Data_Path_p -> Circle_Track_Step) == OUT))
            {
                Data_Path_p -> Circle_Track_Step = OUT;
                OutTime = Img_Store_p -> ImgNum;
            }
        }
        else
        {
            State++;
            Loop_Kind = COMMON_TRACK_LOOP;
            Data_Path_p -> Track_Kind = COMMON_TRACK;
            if((Data_Path_p -> Circle_Track_Step) == OUT)
            {
                Data_Path_p -> Circle_Track_Step = INIT;
            }
        }
    }
    else 
    {
        Loop_Kind = MODEL_TRACK_LOOP;
        Data_Path_p -> Track_Kind = MODEL_TRACK;
    }
    return Loop_Kind;
}


/*
    ModelTrack_Judge说明
    模型赛道决策
    本函数不集成至TrackKind_Judge_Vector中是为了在开发端能进行调试
*/
LoopKind Judge::ModelTrack_Judge(vector<PredictResult> results,Data_Path *Data_Path_p)
{
    LoopKind Loop_Kind;
    for(int i=0;i<results.size();i++)
    {
        PredictResult result = results[i];
    
        if(result.label == "bomb"){ Loop_Kind = MODEL_TRACK_LOOP; Data_Path_p -> Model_Zone_Kind = Model_Danger_Zone; }
        else if(result.label == "bridge"){ Loop_Kind = MODEL_TRACK_LOOP; Data_Path_p -> Model_Zone_Kind = Model_Bridge_Zone; }
        else if(result.label == "crosswalk"){ Loop_Kind = MODEL_TRACK_LOOP; Data_Path_p -> Model_Zone_Kind = Model_Crosswalk_Zone; }
        // else if(result.label == "")
        // 
    }
    return Loop_Kind;
}


/*
    ServoDirAngle_Judge说明
    计算舵机方向和舵机角度
*/
void Judge::ServoDirAngle_Judge(Data_Path *Data_Path_p)
{
    static int Count = 0; 
    static int Dir = 0;
    (Data_Path_p -> ServoAngle) = (Data_Path_p -> TrackCoordinate[(Data_Path_p -> Forward)-(Data_Path_p -> Path_Search_Start)][0]) - 160;
    if(Data_Path_p -> Circle_Track_Step == OUT)
    {
        Count = 0;
        switch(Data_Path_p -> Track_Kind)
        {
            case L_CIRCLE_TRACK:{ Dir = 1; break; }
            case R_CIRCLE_TRACK:{ Dir = -1; break; }
        }
    }
    Count++;
    if(Count <= 10)
    {
        (Data_Path_p -> ServoAngle) = Dir*100;
    }
    // 计算舵机方向和角度
    if((Data_Path_p -> ServoAngle) < 0)
    {
        (Data_Path_p -> ServoDir) = 0;  // 左转
        (Data_Path_p -> ServoAngle) = abs(Data_Path_p -> ServoAngle);
    }
    else
    {
        (Data_Path_p -> ServoDir) = 1;  // 右转
    }
}


/*
    MotorSpeed_Judge说明
    电机速度决策
*/
void Judge::MotorSpeed_Judge(Data_Path *Data_Path_p)
{
    switch(Data_Path_p -> Track_Kind)
    {
        case COMMON_TRACK:
        {
            if(Data_Path_p -> ServoAngle > 30 || Data_Path_p -> Circle_Track_Step == IN_PREPARE)
            {
                Data_Path_p -> MotorSpeed = Data_Path_p -> MotorSpeedInterval[0];
            }
            else
            {
                Data_Path_p -> MotorSpeed = Data_Path_p -> MotorSpeedInterval[1];
            }
            break;
        }
        case L_CIRCLE_TRACK:
        {
            Data_Path_p -> MotorSpeed = Data_Path_p -> MotorSpeedInterval[0];
            break;
        }
        case R_CIRCLE_TRACK:
        {
            Data_Path_p -> MotorSpeed = Data_Path_p -> MotorSpeedInterval[0];
            break;
        }
        case ACROSS_TRACK:
        {
            Data_Path_p -> MotorSpeed = Data_Path_p -> MotorSpeedInterval[0];
            break;
        }
    }
}


/*
    ConfigData_SYNC说明
    车辆上位机设置文件数据同步
*/
void SYNC::ConfigData_SYNC(Data_Path *Data_Path_p,Function_EN *Function_EN_p)
{
    ifstream ConfigFile("../config.json");
    nlohmann::json ConfigData = nlohmann::json::parse(ConfigFile);

    Function_EN_p -> Uart_EN = ConfigData.at("UART_EN");    // 获取串口使能参数
    Function_EN_p -> ImgUnpivot_EN = ConfigData.at("IMG_UNPIVOT_EN");   // 获取图像逆透视使能参数
    Function_EN_p -> ImgCompress_EN = ConfigData.at("IMG_COMPRESS_EN");  // 获取图像压缩使能参数
    Function_EN_p -> Camera_EN = CameraKind(ConfigData.at("CAMERA_EN"));   // 获取摄像头使能参数
    Function_EN_p -> VideoShow_EN = ConfigData.at("VIDEO_SHOW_EN"); // 获取图像显示使能参数
    Function_EN_p -> AcrossIdentify_EN = ConfigData.at("ACROSS_IDENTIFY_EN");   // 获取十字特征点识别使能参数
    Function_EN_p -> CircleIdentify_EN = ConfigData.at("CIRCLE_IDENTIFY_EN");   // 获取圆环特征点识别使能参数
    Data_Path_p -> PointIdentifyAngle[0] = ConfigData.at("MIN_POINT_ANGLE");  // 获取元素特征点角度区间
    Data_Path_p -> PointIdentifyAngle[1] = ConfigData.at("MAX_POINT_ANGLE"); 
    Data_Path_p -> MotorSpeedInterval[0] = ConfigData.at("MIN_MOTOR_SPEED");  // 获取电机速度区间
    Data_Path_p -> MotorSpeedInterval[1] = ConfigData.at("MAX_MOTOR_SPEED"); 
    Data_Path_p -> DilateErode_Factor[0] = ConfigData.at("DILATE_FACTOR");  // 获取图形学膨胀系数
    Data_Path_p -> DilateErode_Factor[1] = ConfigData.at("ERODE_FACTOR");  // 获取图形学腐蚀系数
}


/*
    UartReceive_Bit_To_Change_SYNC说明
    串口接收协议数据交换区和数据位同步
    unsigned char -> int
*/
void SYNC::UartReceive_Bit_To_Change_SYNC(UartReceiveProtocol *UartReceiveProtocol_p)
{
    UartReceiveProtocol_p -> Forward = UartReceiveProtocol_p -> Data_1;
    UartReceiveProtocol_p -> Path_Search_Start = UartReceiveProtocol_p -> Data_2;
    UartReceiveProtocol_p -> Path_Search_End = UartReceiveProtocol_p -> Data_3;
    UartReceiveProtocol_p -> Side_Search_Start = UartReceiveProtocol_p -> Data_4;
    UartReceiveProtocol_p -> Side_Search_End = UartReceiveProtocol_p -> Data_5;
    UartReceiveProtocol_p -> Game_EN = UartReceiveProtocol_p -> Data_6;
}


/*
    UartSend_Change_To_Bit_SYNC说明
    @使用前必须先将程序内部数据和串口发送交换区同步 UartSend_Program_To_Change_SYNC()
    串口发送协议数据交换区和数据位同步
    int -> unsigned char
*/
void SYNC::UartSend_Change_To_Bit_SYNC(UartSendProtocol *UartSendProtocol_p)
{
    UartSendProtocol_p -> Data_1 = UartSendProtocol_p -> ServoDir;
    UartSendProtocol_p -> Data_2 = UartSendProtocol_p -> ServoAngle;
    UartSendProtocol_p -> Data_3 = UartSendProtocol_p -> MotorSpeed;
    UartSendProtocol_p -> Data_4 = UartSendProtocol_p -> Track_Kind;
}


/*
    UartSend_Program_To_Change_SYNC说明
    @使用前必须先将串口发送数据位和数据交换区同步 UartSend_Change_To_Bit_SYNC()
    同步串口发送数据交换区和程序内部数据
    ServoDir
    ServoAngle
    MotorSpeed
    Track_Kind
*/
void SYNC::UartSend_Program_To_Change_SYNC(UartSendProtocol *UartSendProtocol_p , Data_Path *Data_Path_p , Function_EN *Function_EN_p)
{
    //  同步程序内部至串口发送数据交换区
    UartSendProtocol_p -> ServoDir = Data_Path_p -> ServoDir;
    UartSendProtocol_p -> ServoAngle = Data_Path_p -> ServoAngle;
    UartSendProtocol_p -> MotorSpeed = Data_Path_p -> MotorSpeed;
    UartSendProtocol_p -> Track_Kind = (int)Data_Path_p -> Track_Kind;
}


/*
    UartReceive_Change_To_Program_SYNC说明
    @使用后必须先将串口接收数据位和数据交换区同步 Uart_Receive_Bit_To_Change_SYNC()
    同步串口接收数据交换区和程序内部数据
    Forward
    Path_Search_Start
    Path_Search_End
    Side_Search_Start
    Side_Search_End
    Game_EN
*/
void SYNC::UartReceive_Change_To_Program_SYNC(UartReceiveProtocol *UartReceiveProtocol_p , Data_Path *Data_Path_p , Function_EN *Function_EN_p)
{
    //  同步串口接收数据交换区至程序内部
    Data_Path_p -> Forward = UartReceiveProtocol_p -> Forward;
    Data_Path_p -> Path_Search_Start = UartReceiveProtocol_p -> Path_Search_Start;
    Data_Path_p -> Path_Search_End = UartReceiveProtocol_p -> Path_Search_End;
    Data_Path_p -> Side_Search_Start = UartReceiveProtocol_p -> Side_Search_Start;
    Data_Path_p -> Side_Search_End = UartReceiveProtocol_p -> Side_Search_End;
    Function_EN_p -> Game_EN = (bool)UartReceiveProtocol_p -> Game_EN;
}


/*
    DataPrint说明
    打印数据
    程序参数：1.前瞻点 2.寻边线起始点 3.寻边线结束点 4.边线断点起始点 5.边线断点结束点 6.比赛状态
    运动参数：1.舵机方向 2.舵机角度 3.点击速度
*/
void DataPrint(Data_Path *Data_Path_p,Function_EN *Function_EN_p)
{
    cout << "\033c";    // 每次打印前清屏

    if(Function_EN_p -> Uart_EN == true)
    {
        cout << "<---------------------比赛模式--------------------->" << endl;
        cout << endl;
    }
    else
    {
        cout << "<---------------------调试模式--------------------->" << endl;
        cout << endl;
    }

    // 打印程序参数
    cout << "<---------------------程序参数--------------------->" << endl;
    cout << " 前瞻点：" << Data_Path_p -> Forward << endl;
    cout << " 路径线起始点：" << Data_Path_p -> Path_Search_Start << endl; 
    cout << " 路径线结束点：" << Data_Path_p -> Path_Search_End << endl; 
    cout << " 边线起始点：" << Data_Path_p -> Side_Search_Start << endl; 
    cout << " 边线结束点：" << Data_Path_p -> Side_Search_End << endl; 
    cout << " 比赛状态：";
    switch(Function_EN_p -> Game_EN)
    {
        case true:{ cout << "开始" << endl; break;}
        case false:{ cout << "结束" << endl; break;}
    }
    cout << "<-------------------------------------------------->" << endl;
    cout << endl;

    // 打印运动参数
    cout << "<---------------------运动参数--------------------->" << endl;
    cout << "舵机方向：";
    cout << Data_Path_p -> ServoDir << endl;
    cout << "舵机角度：";
    cout << Data_Path_p -> ServoAngle << endl;
    cout <<  "电机速度：";
    cout << Data_Path_p -> MotorSpeed << endl;
    cout <<  "赛道类型：";
    switch(Data_Path_p -> Track_Kind)
    {
        case COMMON_TRACK:{ cout << "普通赛道" << endl; break; }
        case R_CIRCLE_TRACK:
        { 
            cout << "右圆环赛道  "; 
            switch(Data_Path_p -> Circle_Track_Step)
            {
                case IN_PREPARE:{cout << "准备入环" << endl; break;}
                case IN:{cout << "入环" << endl; break;}
                case OUT:{cout << "出环" << endl; break;}
                case INIT:{cout << "初始化" << endl; break;}
            }
            break;
        }
        case L_CIRCLE_TRACK:
        { 
            cout << "左圆环赛道  "; 
            switch(Data_Path_p -> Circle_Track_Step)
            {
                case IN_PREPARE:{cout << "准备入环" << endl; break;}
                case IN:{cout << "入环" << endl; break;}
                case OUT:{cout << "出环" << endl; break;}
                case INIT:{cout << "初始化" << endl; break;}
            }
            break;
        }
        case ACROSS_TRACK:{ cout << "十字赛道" << endl; break; }
        case MODEL_TRACK:{ cout << "AI" << endl; break; }
    }
    cout << "<-------------------------------------------------->" << endl;
}


