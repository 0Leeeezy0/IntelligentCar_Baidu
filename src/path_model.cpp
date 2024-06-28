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

    while(Function_EN_p -> ThreadModelDetection_EN == false);  // 等待模型推理完成

    // 获取推理结果并对结果进行处理
    // 需注意模型推理结果中的坐标是左上角坐标
    for(int i=0;i<PPNCDetection.results.size();i++)
    {
        PredictResult result = PPNCDetection.results[i];
        if(result.label == "cone" && JSON_FunctionConfigData.DangerZone_Cone_Detection_EN == true)
        {
            // 锥桶避障补线
            circle((Img_Store_p -> Img_OTSU),Point(result.x+int(result.width/2),result.y+int(result.height/2)),JSON_TrackConfigData.DangerZone_Cone_Radius,Scalar(255),3);	
        }
        if(result.label == "block")
        {
            // 路障避障补线
            line((Img_Store_p -> Img_OTSU),Point(result.x+int(result.width/2)+JSON_TrackConfigData.DangerZone_Block_Radius,result.y+result.height),Point(result.x+int(result.width/2)+JSON_TrackConfigData.DangerZone_Block_Radius,result.y),Scalar(255),3);
            line((Img_Store_p -> Img_OTSU),Point(result.x+int(result.width/2)-JSON_TrackConfigData.DangerZone_Block_Radius,result.y+result.height),Point(result.x+int(result.width/2)-JSON_TrackConfigData.DangerZone_Block_Radius,result.y),Scalar(255),3);
        }
    }
    
    ImgPathSearch(Img_Store_p,Data_Path_p);
    Judge.ServoDirAngle_Judge(Data_Path_p);
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

    cone_Avg_Y = 239-int(cone_Y/cone_Num);

    // 在识别到标志20帧后才开始判断入库时间
    if(Num >= JSON_TrackConfigData.RescueGarageTime)
    {
        // 若检测到的锥桶平均高度在阈值区间内，则将控制权转移
        if(cone_Avg_Y >= JSON_TrackConfigData.RescueZoneConeAvgY[0] && cone_Avg_Y <= JSON_TrackConfigData.RescueZoneConeAvgY[1] && cone_Num > 2)
        {
            switch(Data_Path_p -> Rescue_Zone_Garage_Dir)
            {
                case LEFT_GARAGE:{ Function_EN_p -> Control_EN = true; break; }
                case RIGHT_GARAGE:{ Function_EN_p -> Control_EN = true; break; }
            }
        }
        Num = 0;
    }

    ImgPathSearch(Img_Store_p,Data_Path_p);
    Judge.ServoDirAngle_Judge(Data_Path_p);
    Data_Path_p -> MotorSpeed = JSON_TrackConfigData.RescueZoneMotorSpeed;

    Num++;
}

