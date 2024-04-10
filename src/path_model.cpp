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
    Data_Path_p -> MotorSpeed = Data_Path_p -> MotorSpeedInterval[1]+10;
}


/*
    Danger_Zone说明

*/
void Danger_Zone()
{
    
}


/*
    Crosswalk_Zone说明

*/
void Crosswalk_Zone(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
    switch(Data_Path_p -> Model_Crosswalk_Zone_Step)
    {
        case START:{ Data_Path_p -> ServoDir = 0; Data_Path_p -> ServoAngle = 0; Data_Path_p -> MotorSpeed = 40; break; }
        case STOP_PREPARE:{ Data_Path_p -> ServoDir = 0; Data_Path_p -> ServoAngle = 0; Data_Path_p -> MotorSpeed = 20; break; }
        case STOP:{ Data_Path_p -> ServoDir = 0; Data_Path_p -> ServoAngle = 0; Data_Path_p -> MotorSpeed = 0; abort(); }
    }
}


/*
    *_Garage_Path说明

*/
void R_Garage_Zone()
{
    
}

void L_Garage_Zone()
{
    
}


