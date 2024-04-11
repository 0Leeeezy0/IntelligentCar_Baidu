#include "common_program.h"
#include "common_system.h"

using namespace std;
using namespace cv;


// 圆环准备入环步骤：补线
void CircleTrack_Step_IN_Prepare(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
    JSON_TrackConfigData JSON_TrackConfigData = Data_Path_p -> JSON_TrackConfigData_v[0];

    switch(Data_Path_p -> Track_Kind)
    {
        case L_CIRCLE_TRACK:
        {
            // 准备左入环补线
            // 赛道彩色图像
            line((Img_Store_p -> Img_Track),Point((Data_Path_p -> SideCoordinate_Eight[0][0]),(Data_Path_p -> SideCoordinate_Eight[0][1])),Point((Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[1])-1][2])-(JSON_TrackConfigData.TrackWidth),(Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[0])-1][1])),Scalar(128,0,128),4);
            // 赛道二值化图像
            line((Img_Store_p -> Img_OTSU),Point((Data_Path_p -> SideCoordinate_Eight[0][0]),(Data_Path_p -> SideCoordinate_Eight[0][1])),Point((Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[1])-1][2])-(JSON_TrackConfigData.TrackWidth),(Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[0])-1][1])),Scalar(255),4);
            
            break;
        }
        case R_CIRCLE_TRACK:
        {
            // 准备右入环补线
            // 赛道彩色图像
            line((Img_Store_p -> Img_Track),Point((Data_Path_p -> SideCoordinate_Eight[0][2]),(Data_Path_p -> SideCoordinate_Eight[0][3])),Point((Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[0])-1][0])+(JSON_TrackConfigData.TrackWidth),(Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[1])-1][3])),Scalar(128,0,128),4);
            // 赛道二值化图像
            line((Img_Store_p -> Img_OTSU),Point((Data_Path_p -> SideCoordinate_Eight[0][2]),(Data_Path_p -> SideCoordinate_Eight[0][3])),Point((Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[0])-1][0])+(JSON_TrackConfigData.TrackWidth),(Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[1])-1][3])),Scalar(255),4);
            
            break;
        }
    }
}


// 圆环准备入环步骤：补线
void CircleTrack_Step_IN_Prepare_Stright(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
    JSON_TrackConfigData JSON_TrackConfigData = Data_Path_p -> JSON_TrackConfigData_v[0];

    switch(Data_Path_p -> Previous_Circle_Kind)
    {
        case L_CIRCLE_TRACK:
        {
            // 准备左入环补线
            // 赛道彩色图像
            line((Img_Store_p -> Img_Track),Point((Data_Path_p -> SideCoordinate_Eight[0][0]),(Data_Path_p -> SideCoordinate_Eight[0][1])),Point((Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[1])-1][2])-(JSON_TrackConfigData.TrackWidth),(Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[0])-1][1])),Scalar(128,0,128),4);
            // 赛道二值化图像
            line((Img_Store_p -> Img_OTSU),Point((Data_Path_p -> SideCoordinate_Eight[0][0]),(Data_Path_p -> SideCoordinate_Eight[0][1])),Point((Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[1])-1][2])-(JSON_TrackConfigData.TrackWidth),(Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[0])-1][1])),Scalar(255),4);
            
            break;
        }
        case R_CIRCLE_TRACK:
        {
            // 准备右入环补线
            // 赛道彩色图像
            line((Img_Store_p -> Img_Track),Point((Data_Path_p -> SideCoordinate_Eight[0][2]),(Data_Path_p -> SideCoordinate_Eight[0][3])),Point((Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[0])-1][0])+(JSON_TrackConfigData.TrackWidth),(Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[1])-1][3])),Scalar(128,0,128),4);
            // 赛道二值化图像
            line((Img_Store_p -> Img_OTSU),Point((Data_Path_p -> SideCoordinate_Eight[0][2]),(Data_Path_p -> SideCoordinate_Eight[0][3])),Point((Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[0])-1][0])+(JSON_TrackConfigData.TrackWidth),(Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[1])-1][3])),Scalar(255),4);
            
            break;
        }
    }
}



// 圆环入环步骤：补线
void CircleTrack_Step_IN(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
    switch(Data_Path_p -> Track_Kind)
    {
        case L_CIRCLE_TRACK:
        {
            // 左入环补线
            // 赛道彩色图像
            line((Img_Store_p -> Img_Track),Point((Data_Path_p -> SideCoordinate_Eight[0][2]),(Data_Path_p -> SideCoordinate_Eight[0][3])),Point((Data_Path_p -> InflectionPointCoordinate[(Data_Path_p -> InflectionPointNum[0])-1][0]),(Data_Path_p -> InflectionPointCoordinate[(Data_Path_p -> InflectionPointNum[0])-1][1])),Scalar(128,0,128),4);
            // 赛道二值化图像
            line((Img_Store_p -> Img_OTSU),Point((Data_Path_p -> SideCoordinate_Eight[0][2]),(Data_Path_p -> SideCoordinate_Eight[0][3])),Point((Data_Path_p -> InflectionPointCoordinate[(Data_Path_p -> InflectionPointNum[0])-1][0]),(Data_Path_p -> InflectionPointCoordinate[(Data_Path_p -> InflectionPointNum[0])-1][1])),Scalar(255),4);
            
            break;
        }
        case R_CIRCLE_TRACK:
        {
            // 右入环补线
            // 赛道彩色图像
            line((Img_Store_p -> Img_Track),Point((Data_Path_p -> SideCoordinate_Eight[0][0]),(Data_Path_p -> SideCoordinate_Eight[0][1])),Point((Data_Path_p -> InflectionPointCoordinate[(Data_Path_p -> InflectionPointNum[1])-1][2]),(Data_Path_p -> InflectionPointCoordinate[(Data_Path_p -> InflectionPointNum[1])-1][3])),Scalar(128,0,128),4);
            // 赛道二值化图像
            line((Img_Store_p -> Img_OTSU),Point((Data_Path_p -> SideCoordinate_Eight[0][0]),(Data_Path_p -> SideCoordinate_Eight[0][1])),Point((Data_Path_p -> InflectionPointCoordinate[(Data_Path_p -> InflectionPointNum[1])-1][2]),(Data_Path_p -> InflectionPointCoordinate[(Data_Path_p -> InflectionPointNum[1])-1][3])),Scalar(255),4);

            break;
        }
    }
}


// 圆环出环步骤：打角
void CircleTrack_Step_OUT(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
    JSON_TrackConfigData JSON_TrackConfigData = Data_Path_p -> JSON_TrackConfigData_v[0];

    switch(Data_Path_p -> Track_Kind)
    {
        case L_CIRCLE_TRACK:
        {
            // 出左环打角
            Data_Path_p -> ServoDir = 0;
            Data_Path_p -> ServoAngle = JSON_TrackConfigData.CircleOutServoAngle;
            break;
        }
        case R_CIRCLE_TRACK:
        {
            // 出右环打角
            Data_Path_p -> ServoDir = 1;
            Data_Path_p -> ServoAngle = JSON_TrackConfigData.CircleOutServoAngle;
            break;
        }
    }
}
