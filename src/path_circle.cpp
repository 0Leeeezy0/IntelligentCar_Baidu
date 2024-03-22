#include "common_program.h"
#include "common_system.h"

using namespace std;
using namespace cv;

/*
    CircleTrack_Step_*说明
    圆环赛道步骤
*/
// 圆环步骤决策
CircleTrackStep CircleTrack_Step_Judge(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
    CircleTrackStep CircleTrack_Step;
    static int CircleStepFlag; // 圆环步骤决策标志位
    static TrackKind CircleRecord;  // 圆环类型记录，用于判断是入环点还是出环点
    switch(Data_Path_p -> Track_Kind)
    {
        case L_CIRCLE_TRACK:
        {
            for(int i = -20;i <= 15;i++)
            {
                if((Img_Store_p -> Img_OTSU_Unpivot).at<uchar>((Data_Path_p -> InterruptCoordinate[0][1]-30),(Data_Path_p -> InterruptCoordinate[0][0])+i) == 255)
                {
                    if(CircleRecord == L_CIRCLE_TRACK)
                    {
                        CircleStepFlag = 1;
                    }
                    else
                    {
                        CircleStepFlag = 2;
                    }
                    break;
                }
                else
                {
                    if(CircleStepFlag != 1 && CircleStepFlag != 2)
                    {
                        CircleStepFlag = 0;
                    }
                }
            }
            break;
        }
        case R_CIRCLE_TRACK:
        {
            for(int i = -15;i <= 20;i++)
            {
                if((Img_Store_p -> Img_OTSU_Unpivot).at<uchar>((Data_Path_p -> InterruptCoordinate[0][3]-30),(Data_Path_p -> InterruptCoordinate[0][2])+i) == 255)
                {
                    if(CircleRecord == R_CIRCLE_TRACK)
                    {
                        CircleStepFlag = 1;
                    }
                    else
                    {
                        CircleStepFlag = 2;
                    }
                    break;
                }
                else
                {
                    if(CircleStepFlag != 1 && CircleStepFlag != 2)
                    {
                        CircleStepFlag = 0;
                    }
                }
            }
            break;
        }
    }

    switch(CircleStepFlag)
    {
        case 0:
        { 
            CircleTrack_Step = IN_PREPARE; 
            CircleRecord = (Data_Path_p -> Track_Kind);
            break;
        }
        case 1:{ CircleTrack_Step = IN; break; }
        case 2:{ CircleTrack_Step = OUT; CircleStepFlag = 0; break; }
    }

    return CircleTrack_Step;
}


// 圆环准备入环步骤：补线
void CircleTrack_Step_IN_Prepare(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
    switch(Data_Path_p -> Track_Kind)
    {
        case L_CIRCLE_TRACK:
        {
            // 准备左入环补线
            // 赛道彩色图像
            line((Img_Store_p -> Img_Track_Unpivot),Point((Data_Path_p -> SideCoordinate_Eight[0][0]),(Data_Path_p -> SideCoordinate_Eight[0][1])),Point((Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[1])-1][2])-(Data_Path_p -> Side_Width),(Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[0])-1][1])),Scalar(128,0,128),4);
            // 赛道二值化图像
            line((Img_Store_p -> Img_OTSU_Unpivot),Point((Data_Path_p -> SideCoordinate_Eight[0][0]),(Data_Path_p -> SideCoordinate_Eight[0][1])),Point((Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[1])-1][2])-(Data_Path_p -> Side_Width),(Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[0])-1][1])),Scalar(255),4);
            
            break;
        }
        case R_CIRCLE_TRACK:
        {
            // 准备右入环补线
            // 赛道彩色图像
            line((Img_Store_p -> Img_Track_Unpivot),Point((Data_Path_p -> SideCoordinate_Eight[0][2]),(Data_Path_p -> SideCoordinate_Eight[0][3])),Point((Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[0])-1][0])+(Data_Path_p -> Side_Width),(Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[1])-1][3])),Scalar(128,0,128),4);
            // 赛道二值化图像
            line((Img_Store_p -> Img_OTSU_Unpivot),Point((Data_Path_p -> SideCoordinate_Eight[0][2]),(Data_Path_p -> SideCoordinate_Eight[0][3])),Point((Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[0])-1][0])+(Data_Path_p -> Side_Width),(Data_Path_p -> SideCoordinate_Eight[(Data_Path_p -> NumSearch[1])-1][3])),Scalar(255),4);
            
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
            line((Img_Store_p -> Img_Track_Unpivot),Point((Data_Path_p -> SideCoordinate_Eight[0][2]),(Data_Path_p -> SideCoordinate_Eight[0][3])),Point((Data_Path_p -> InterruptCoordinate[(Data_Path_p -> InterruptNum[0])-1][0]),(Data_Path_p -> InterruptCoordinate[(Data_Path_p -> InterruptNum[0])-1][1])),Scalar(128,0,128),4);
            // 赛道二值化图像
            line((Img_Store_p -> Img_OTSU_Unpivot),Point((Data_Path_p -> SideCoordinate_Eight[0][2]),(Data_Path_p -> SideCoordinate_Eight[0][3])),Point((Data_Path_p -> InterruptCoordinate[(Data_Path_p -> InterruptNum[0])-1][0]),(Data_Path_p -> InterruptCoordinate[(Data_Path_p -> InterruptNum[0])-1][1])),Scalar(255),4);
            
            break;
        }
        case R_CIRCLE_TRACK:
        {
            // 右入环补线
            // 赛道彩色图像
            line((Img_Store_p -> Img_Track_Unpivot),Point((Data_Path_p -> SideCoordinate_Eight[0][0]),(Data_Path_p -> SideCoordinate_Eight[0][1])),Point((Data_Path_p -> InterruptCoordinate[(Data_Path_p -> InterruptNum[1])-1][2]),(Data_Path_p -> InterruptCoordinate[(Data_Path_p -> InterruptNum[1])-1][3])),Scalar(128,0,128),4);
            // 赛道二值化图像
            line((Img_Store_p -> Img_OTSU_Unpivot),Point((Data_Path_p -> SideCoordinate_Eight[0][0]),(Data_Path_p -> SideCoordinate_Eight[0][1])),Point((Data_Path_p -> InterruptCoordinate[(Data_Path_p -> InterruptNum[1])-1][2]),(Data_Path_p -> InterruptCoordinate[(Data_Path_p -> InterruptNum[1])-1][3])),Scalar(255),4);

            break;
        }
    }
}