#include "common_system.h"
#include "common_program.h"

using namespace std;
using namespace cv;

/*
    Brigdge_Zone说明
    桥梁区域
*/
void Bridge_Zone(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
    Judge Judge;
    JSON_TrackConfigData JSON_TrackConfigData = Data_Path_p -> JSON_TrackConfigData_v[0];
    Data_Path_p -> JSON_TrackConfigData_v[0].Forward = JSON_TrackConfigData.BridgeZoneForward;
    Data_Path_p -> MotorSpeed = JSON_TrackConfigData.BridgeZoneMotorSpeed;
    ImgPathSearch(Img_Store_p,Data_Path_p);
    Judge.ServoDirAngle_Judge(Data_Path_p);
}


/*
    Danger_Zone说明
    危险区域
*/
void Danger_Zone(PPNCDetection& PPNCDetection,Img_Store *Img_Store_p,Data_Path *Data_Path_p,Function_EN *Function_EN_p)
{
    Judge Judge;
    JSON_TrackConfigData JSON_TrackConfigData = Data_Path_p -> JSON_TrackConfigData_v[0];
    JSON_FunctionConfigData JSON_FunctionConfigData = Function_EN_p -> JSON_FunctionConfigData_v[0];
    Data_Path_p -> JSON_TrackConfigData_v[0].Forward = JSON_TrackConfigData.DangerZoneForward;

    PredictResult CloseBarrier;
    static PredictResult Block;
    CloseBarrier.y = 0;
    CloseBarrier.label = "cone";

    while(Function_EN_p -> ThreadModelDetection_EN == false);  // 等待模型推理完成

    // 获取推理结果并对结果进行处理
    // 需注意模型推理结果中的坐标是左上角坐标
    for(int i=0;i<PPNCDetection.results.size();i++)
    {
        PredictResult result = PPNCDetection.results[i];
        if(result.y >= CloseBarrier.y && (result.label == "cone" || result.label == "block"))
        {
            CloseBarrier.x = result.x;
            CloseBarrier.y = result.y;
            CloseBarrier.width = result.width;
            CloseBarrier.height = result.height;
            CloseBarrier.label = result.label;
        }
    }
    // // 防止模型不给力识别不到方块
    // if(Data_Path_p -> InflectionPointNum[0] != 0)
    // {
    //     Block.x = (Data_Path_p -> InflectionPointCoordinate[0][0]);
    //     Block.y = (Data_Path_p -> InflectionPointCoordinate[0][1]);
    // }
    // if(Data_Path_p -> InflectionPointNum[1] != 0)
    // {
    //     Block.x = (Data_Path_p -> InflectionPointCoordinate[0][2]);
    //     Block.y = (Data_Path_p -> InflectionPointCoordinate[0][3]);
    // }
    if(CloseBarrier.label == "cone" && JSON_FunctionConfigData.DangerZone_Cone_Detection_EN == true)
    {
        // 锥桶避障补线
        circle((Img_Store_p -> Img_OTSU),Point(CloseBarrier.x+int(CloseBarrier.width/2),CloseBarrier.y+int(CloseBarrier.height/2)),JSON_TrackConfigData.DangerZone_Cone_Radius,Scalar(255),3);	
    }
    if(CloseBarrier.label == "block")
    {
        // 方块避障补线
        line((Img_Store_p -> Img_OTSU),Point(CloseBarrier.x+int(CloseBarrier.width/2)+JSON_TrackConfigData.DangerZone_Block_Radius,CloseBarrier.y+CloseBarrier.height/2),Point(CloseBarrier.x+int(CloseBarrier.width/2)+JSON_TrackConfigData.DangerZone_Block_Radius,CloseBarrier.y-CloseBarrier.height/2),Scalar(255),3);
        line((Img_Store_p -> Img_OTSU),Point(CloseBarrier.x+int(CloseBarrier.width/2)-JSON_TrackConfigData.DangerZone_Block_Radius,CloseBarrier.y+CloseBarrier.height/2),Point(CloseBarrier.x+int(CloseBarrier.width/2)-JSON_TrackConfigData.DangerZone_Block_Radius,CloseBarrier.y-CloseBarrier.height/2),Scalar(255),3);
        line((Img_Store_p -> Img_OTSU),Point(CloseBarrier.x+int(CloseBarrier.width/2),CloseBarrier.y+CloseBarrier.height*1.5),Point(CloseBarrier.x+int(CloseBarrier.width/2)+JSON_TrackConfigData.DangerZone_Block_Radius,CloseBarrier.y+CloseBarrier.height/2),Scalar(255),3);
        line((Img_Store_p -> Img_OTSU),Point(CloseBarrier.x+int(CloseBarrier.width/2),CloseBarrier.y+CloseBarrier.height*1.5),Point(CloseBarrier.x+int(CloseBarrier.width/2)-JSON_TrackConfigData.DangerZone_Block_Radius,CloseBarrier.y+CloseBarrier.height/2),Scalar(255),3);
    }
    // // 防止模型不给力识别不到方块
    // if((Data_Path_p -> InflectionPointNum[0] != 0 || Data_Path_p -> InflectionPointNum[1] != 0))
    // {
    //     // 方块避障补线
    //     line((Img_Store_p -> Img_OTSU),Point(Block.x+JSON_TrackConfigData.DangerZone_Block_Radius,Block.y+10),Point(Block.x+JSON_TrackConfigData.DangerZone_Block_Radius,Block.y-10),Scalar(255),3);
    //     line((Img_Store_p -> Img_OTSU),Point(Block.x-JSON_TrackConfigData.DangerZone_Block_Radius,Block.y+10),Point(Block.x-JSON_TrackConfigData.DangerZone_Block_Radius,Block.y-10),Scalar(255),3);
    //     line((Img_Store_p -> Img_OTSU),Point(Block.x,Block.y+20),Point(Block.x+JSON_TrackConfigData.DangerZone_Block_Radius,Block.y+40),Scalar(255),3);
    //     line((Img_Store_p -> Img_OTSU),Point(Block.x,Block.y+20),Point(Block.x-JSON_TrackConfigData.DangerZone_Block_Radius,Block.y+40),Scalar(255),3);
    // }
    
    ImgPathSearch(Img_Store_p,Data_Path_p);
    Judge.ServoDirAngle_Judge(Data_Path_p);

    // 准备避障舵机角度扩大
    // 锥桶
    if(CloseBarrier.y <= JSON_TrackConfigData.Danger_Zone_Barrier_Y && PPNCDetection.results.size() >=1 && CloseBarrier.label == "cone")
    {
        (Data_Path_p -> ServoAngle) = int((Data_Path_p -> ServoAngle)*JSON_TrackConfigData.Danger_Zone_Barrier_Servor_Angle_Factor[0]);
    }
    // 方块
    if(CloseBarrier.y <= JSON_TrackConfigData.Danger_Zone_Barrier_Y && PPNCDetection.results.size() >=1 && CloseBarrier.label == "block")
    {
        (Data_Path_p -> ServoAngle) = int((Data_Path_p -> ServoAngle)*JSON_TrackConfigData.Danger_Zone_Barrier_Servor_Angle_Factor[1]);
    }
    // 防止模型不给力识别不到方块
    if((Data_Path_p -> InflectionPointNum[0] != 0 || Data_Path_p -> InflectionPointNum[1] != 0) && CloseBarrier.label != "cone")
    {
        if((Data_Path_p -> InflectionPointCoordinate[0][1]) <= JSON_TrackConfigData.Danger_Zone_Barrier_Y || (Data_Path_p -> InflectionPointCoordinate[0][3]) <= JSON_TrackConfigData.Danger_Zone_Barrier_Y)
        {
            (Data_Path_p -> ServoAngle) = int((Data_Path_p -> ServoAngle)*JSON_TrackConfigData.Danger_Zone_Barrier_Servor_Angle_Factor[1]);
        }
    }

    Data_Path_p -> MotorSpeed = JSON_TrackConfigData.DangerZoneMotorSpeed;
}


/*
    Crosswalk_Zone说明
    斑马线区域
*/
void Crosswalk_Zone(PPNCDetection& PPNCDetection,Img_Store *Img_Store_p,Data_Path *Data_Path_p,Function_EN *Function_EN_p)
{
    Judge Judge;
    JSON_TrackConfigData JSON_TrackConfigData = Data_Path_p -> JSON_TrackConfigData_v[0];

    while(Function_EN_p -> ThreadModelDetection_EN == false);  // 等待模型推理完成

    // 斑马线区域补白
    for(int i=0;i<PPNCDetection.results.size();i++)
    {
        PredictResult result = PPNCDetection.results[i];
        if(result.label == "crosswalk")
        {
            line((Img_Store_p -> Img_OTSU),Point(result.x,result.y),Point(result.x,result.y+result.height),Scalar(255),3);
            line((Img_Store_p -> Img_OTSU),Point(result.x+result.width,result.y),Point(result.x+result.width,result.y+result.height),Scalar(255),3);
            Rect Rect;
            Rect.x = result.x+3;
            Rect.y = result.y;
            Rect.width = result.width-6;
            Rect.height = result.height;
            rectangle((Img_Store_p -> Img_OTSU),Rect,Scalar(0),-1);
        }
    }

    switch(Data_Path_p -> Crosswalk_Zone_Step)
    {
        case STOP_PREPARE:{ Data_Path_p -> MotorSpeed = JSON_TrackConfigData.CrosswalkZoneMotorSpeed; break; }
        case STOP:{ Data_Path_p -> MotorSpeed = 0; break; }
    }
    ImgPathSearch(Img_Store_p,Data_Path_p);
    Judge.ServoDirAngle_Judge(Data_Path_p);
}


/*
    Rescue_Path说明
    救援区域
*/
void Rescue_Zone(PPNCDetection& PPNCDetection,Img_Store *Img_Store_p,Data_Path *Data_Path_p,Function_EN *Function_EN_p)
{
    Judge Judge;
    JSON_TrackConfigData JSON_TrackConfigData = Data_Path_p -> JSON_TrackConfigData_v[0];
    JSON_FunctionConfigData JSON_FunctionConfigData = Function_EN_p -> JSON_FunctionConfigData_v[0];

    int cone_Y = 0;
    int cone_Num = 0;
    int cone_Avg_Y = 0;
    static int Num = 0;

    while(Function_EN_p -> ThreadModelDetection_EN == false);  // 等待模型推理完成

    // 获取推理结果并对结果进行处理
    // 需注意模型推理结果中的坐标是左上角坐标
    for(int i=0;i<PPNCDetection.results.size();i++)
    {
        PredictResult result = PPNCDetection.results[i];
        if(result.label == "cone")
        {
            // 锥桶数量获取
            cone_Num++;
            // 锥桶总高度获取
            cone_Y += result.y;
        }
    }

    cone_Avg_Y = int(cone_Y/cone_Num);

    // 在识别到标志固定帧后才开始判断入库时间
    if(Num >= JSON_TrackConfigData.RescueGarageTime && JSON_FunctionConfigData.RescueZone_Lable_Detection_EN == true)
    {
        // 若检测到的锥桶平均高度在阈值区间内，则将控制权转移
        if(cone_Avg_Y >= JSON_TrackConfigData.RescueZoneConeAvgY)
        {
            switch(Data_Path_p -> Rescue_Zone_Garage_Dir)
            {
                case LEFT_GARAGE:{ Function_EN_p -> Control_EN = true; break; }
                case RIGHT_GARAGE:{ Function_EN_p -> Control_EN = true; break; }
            }
        }
        Num = 0;
    }
    else if(Num >= JSON_TrackConfigData.RescueGarageTime && Function_EN_p -> Garage_EN == true && JSON_FunctionConfigData.RescueZone_Lable_Detection_EN == false)
    {
        // 若检测到的锥桶平均高度在阈值区间内，则将控制权转移
        if(cone_Avg_Y >= JSON_TrackConfigData.RescueZoneConeAvgY)
        {
            switch(Data_Path_p -> Rescue_Zone_Garage_Dir)
            {
                case LEFT_GARAGE:{ Function_EN_p -> Control_EN = true; Function_EN_p -> Garage_EN = false; break; }
                case RIGHT_GARAGE:{ Function_EN_p -> Control_EN = true; Function_EN_p -> Garage_EN = false; break; }
            }
        }
        Num = 0;
    }

    ImgPathSearch(Img_Store_p,Data_Path_p);
    Judge.ServoDirAngle_Judge(Data_Path_p);
    Data_Path_p -> MotorSpeed = JSON_TrackConfigData.RescueZoneMotorSpeed;

    Num++;
}

