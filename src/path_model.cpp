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
    Data_Path_p -> MotorSpeed = JSON_TrackConfigData.MotorSpeedInterval[1];
    ImgPathSearch(Img_Store_p,Data_Path_p);
    Judge.ServoDirAngle_Judge(Data_Path_p);
}


/*
    Danger_Zone说明
    危险区域
*/
void Danger_Zone(Img_Store *Img_Store_p,Data_Path *Data_Path_p,int DangerZoneMotorSpeed,int ConeRadius)
{
    Judge Judge;
    
    Data_Path_p -> MotorSpeed = DangerZoneMotorSpeed;

    circle((Img_Store_p -> Img_Color),Point((Data_Path_p -> InflectionPointCoordinate[0][0]),(Data_Path_p -> InflectionPointCoordinate[0][1])),ConeRadius,Scalar(0),2);	// 左锥桶避障区域
    circle((Img_Store_p -> Img_Color),Point((Data_Path_p -> InflectionPointCoordinate[0][2]),(Data_Path_p -> InflectionPointCoordinate[0][3])),ConeRadius,Scalar(0),2);	// 右锥桶避障区域

    ImgPathSearch(Img_Store_p,Data_Path_p);
    Judge.ServoDirAngle_Judge(Data_Path_p);
}


/*
    Crosswalk_Zone说明
    斑马线区域
*/
void Crosswalk_Zone(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
    Judge Judge;
    switch(Data_Path_p -> Model_Crosswalk_Zone_Step)
    {
        case START:{ Data_Path_p -> MotorSpeed = 30; Data_Path_p -> ServoDir = 0; Data_Path_p -> ServoAngle = 0; break; }
        case STOP_PREPARE:{ Data_Path_p -> MotorSpeed = 30; Data_Path_p -> ServoDir = 0; Data_Path_p -> ServoAngle = 0; break; }
        case STOP:{ Data_Path_p -> MotorSpeed = 0; Data_Path_p -> ServoDir = 0; Data_Path_p -> ServoAngle = 0; break; }
    }
}


/*
    *_Garage_Path说明
    车库区域
*/
void R_Garage_Zone()
{
    
}

void L_Garage_Zone()
{
    
}


