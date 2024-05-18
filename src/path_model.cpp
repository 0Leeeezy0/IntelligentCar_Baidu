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
    Data_Path_p -> Forward = JSON_TrackConfigData.BridgeZoneForward;
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

    Data_Path_p -> Forward = JSON_TrackConfigData.DangerZoneForward;

    while(Function_EN_p -> ThreadModelDetection_EN == false);  // 等待模型推理完成

    // 获取推理结果并对结果进行处理
    // 需注意模型推理结果中的坐标是左上角坐标
    for(int i=0;i<PPNCDetection.results.size();i++)
    {
        PredictResult result = PPNCDetection.results[i];
        if(result.label == "cone")
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
void Crosswalk_Zone(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
    Judge Judge;
    JSON_TrackConfigData JSON_TrackConfigData = Data_Path_p -> JSON_TrackConfigData_v[0];
    switch(Data_Path_p -> Crosswalk_Zone_Step)
    {
        case START:{ Data_Path_p -> MotorSpeed = JSON_TrackConfigData.CrosswalkZoneMotorSpeed[0]; Data_Path_p -> ServoDir = 0; Data_Path_p -> ServoAngle = 0; break; }
        case STOP_PREPARE:{ Data_Path_p -> MotorSpeed = JSON_TrackConfigData.CrosswalkZoneMotorSpeed[1]; Data_Path_p -> ServoDir = 0; Data_Path_p -> ServoAngle = 0; break; }
        case STOP:{ Data_Path_p -> MotorSpeed = 0; Data_Path_p -> ServoDir = 0; Data_Path_p -> ServoAngle = 0; break; }
    }
}


/*
    Rescue_Path说明
    救援区域
*/
void Rescue_Zone(PPNCDetection& PPNCDetection,Img_Store *Img_Store_p,Data_Path *Data_Path_p,Function_EN *Function_EN_p)
{
    Judge Judge;
    JSON_TrackConfigData JSON_TrackConfigData = Data_Path_p -> JSON_TrackConfigData_v[0];

    int coneNum = 0;
    static int Num = 0;

    Data_Path_p -> Forward = JSON_TrackConfigData.DangerZoneForward;

    while(Function_EN_p -> ThreadModelDetection_EN == false);  // 等待模型推理完成

    // 获取推理结果并对结果进行处理
    // 需注意模型推理结果中的坐标是左上角坐标
    for(int i=0;i<PPNCDetection.results.size();i++)
    {
        PredictResult result = PPNCDetection.results[i];
        if(result.label == "cone")
        {
            // 锥桶数量获取
            coneNum++;
        }
    }

    // 在识别到标志20帧后才开始判断入库时间
    if(Num >= 20)
    {
        // 若检测到的锥桶数量超过阈值，则将控制权转移
        if(coneNum <= JSON_TrackConfigData.RescueZoneConeNum)
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

