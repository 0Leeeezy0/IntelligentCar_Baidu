#include "common.h"

using namespace std;
using namespace cv;
using namespace swan;

// 图像预处理
void imgProcess(Mat& Img,Mat& Img_LDC,Mat& Img_Gray,Mat& Img_OTSU,Mat& Img_HSV_OTSU_Black,Mat& Img_HSV_OTSU_White)
{
    SwanImage SwanImage;
    SwanImage::BGR SwanImageBGR;
    SwanImage::HSV SwanImageHSV;

    Mat Img_HSV;
    Mat CameraMatrix = Mat::eye(3, 3, CV_64F);   // 相机内参矩阵
	CameraMatrix.at<double>(0,0) = 397.8647660464503;
	CameraMatrix.at<double>(0,1) = 0;
	CameraMatrix.at<double>(0,2) = 173.587762233143;
    CameraMatrix.at<double>(1,0) = 0;
	CameraMatrix.at<double>(1,1) = 397.0748966396999;
	CameraMatrix.at<double>(1,2) = 115.2831958031909;
    CameraMatrix.at<double>(2,0) = 0;
	CameraMatrix.at<double>(2,1) = 0;
	CameraMatrix.at<double>(2,2) = 1;
    Mat distCoeffs = Mat::zeros(5, 1, CV_64F);  // 畸变系数
    distCoeffs.at<double>(0,0) = -0.8544944665246371;
    distCoeffs.at<double>(1,0) = -15.71705045557308;
    distCoeffs.at<double>(2,0) = 0.004949277852811135;
    distCoeffs.at<double>(3,0) = -0.0203538377857174;
    distCoeffs.at<double>(4,0) = 213.2896765584135;

    undistort(Img,Img_LDC,CameraMatrix,distCoeffs);
    Img_Gray = Img_LDC.clone();
    SwanImage.colorConvert(Img_Gray,COLOR_BGR2GRAY);
    SwanImage.Sobel(Img_Gray,3);
    SwanImage.GaussBlur(Img_Gray,5);
    Img_OTSU = Img_Gray.clone();
    SwanImageBGR.thresholdConvert(Img_OTSU);
    Img_HSV = Img_LDC.clone();
    SwanImage.colorConvert(Img_HSV,COLOR_BGR2HSV);
    Img_HSV_OTSU_Black = Img_HSV.clone();
    Img_HSV_OTSU_White = Img_HSV.clone();
    SwanImage.GaussBlur(Img_HSV_OTSU_Black,5);
    SwanImage.GaussBlur(Img_HSV_OTSU_White,5);
    // HSV亮度通道均一化
    // SwanImageHSV.Equalize(Img_HSV_OTSU_Black);
    // SwanImageHSV.Equalize(Img_HSV_OTSU_White);
    // HSV图像二值化
    SwanImageHSV.thresholdConvert(Img_HSV_OTSU_Black,Scalar(0,0,0),Scalar(180,255,102)); // 二值化黑色
    SwanImageHSV.thresholdConvert(Img_HSV_OTSU_White,Scalar(0,0,100),Scalar(180,70,255)); // 二值化白色:背景色黄色

    // 图像开运算
    SwanImage.Dilate(Img_OTSU,6);
    SwanImage.Erode(Img_OTSU,5);
    // 图像闭运算
    SwanImage.Dilate(Img_HSV_OTSU_Black,2);
    SwanImage.Erode(Img_HSV_OTSU_Black,2);
    SwanImage.Dilate(Img_HSV_OTSU_White,2);
    SwanImage.Erode(Img_HSV_OTSU_White,2);
}


// 图像绘制
void drawImg(Mat& Img_LDC,Mat& Img_Nano,vector<RotatedRect> outsideRect,vector<RotatedRect> insideRect,_Step_ Step)
{
    Point2f rectLine[4];

    // ROI区域绘制
    rectangle(Img_LDC,Point(ROI_X,ROI_Y),Point(ROI_X+ROI_WIDTH,ROI_Y+ROI_HEIGHT),Scalar(0,0,0));
    rectangle(Img_Nano,Point(ROI_X,ROI_Y),Point(ROI_X+ROI_WIDTH,ROI_Y+ROI_HEIGHT),Scalar(0,0,0));
    rectangle(Img_LDC,Point(PIECE_ROI_X,BLACK_PIECE_ROI_Y),Point(PIECE_ROI_X+PIECE_ROI_WIDTH,BLACK_PIECE_ROI_Y+PIECE_ROI_HEIGHT),Scalar(0,0,0));
    rectangle(Img_Nano,Point(PIECE_ROI_X,BLACK_PIECE_ROI_Y),Point(PIECE_ROI_X+PIECE_ROI_WIDTH,BLACK_PIECE_ROI_Y+PIECE_ROI_HEIGHT),Scalar(0,0,0));
    rectangle(Img_LDC,Point(PIECE_ROI_X,WHITE_PIECE_ROI_Y),Point(PIECE_ROI_X+PIECE_ROI_WIDTH,WHITE_PIECE_ROI_Y+PIECE_ROI_HEIGHT),Scalar(0,0,0));
    rectangle(Img_Nano,Point(PIECE_ROI_X,WHITE_PIECE_ROI_Y),Point(PIECE_ROI_X+PIECE_ROI_WIDTH,WHITE_PIECE_ROI_Y+PIECE_ROI_HEIGHT),Scalar(0,0,0));

    // 模式
    putText(Img_Nano,"MODE:"+to_string(Step),Point(10,210),FONT_ITALIC,0.5,Scalar(0,255,255)); // 黄色
    for(int i = 0;i < outsideRect.size();i++)
    {
        // 绘制棋盘外轮廓中心
        circle(Img_LDC,outsideRect[0].center,5,Scalar(0,255,0),1);  // 绿色
        circle(Img_Nano,outsideRect[0].center,5,Scalar(0,255,0),1);  // 绿色
        // 棋盘角度值
        putText(Img_Nano,to_string(outsideRect[0].angle),Point(10,230),FONT_ITALIC,0.5,Scalar(0,255,255)); // 黄色
    }
    // 绘制棋盘角点内轮廓中心
    for(int i = 0;i < insideRect.size();i++)
    {
        circle(Img_LDC,insideRect[i].center,3,Scalar(0,255,255),2);  // 黄色
        putText(Img_LDC,to_string(i+1),insideRect[i].center,FONT_ITALIC,0.5,Scalar(0,255,255));  // 黄色
        circle(Img_Nano,insideRect[i].center,3,Scalar(0,255,255),2);  // 黄色
        putText(Img_Nano,to_string(i+1),insideRect[i].center,FONT_ITALIC,0.5,Scalar(0,255,255)); // 黄色 
    }
    // 绘制棋盘角点外轮廓矩形框
    for(int j = 0;j < outsideRect.size();j++)
    {
        outsideRect[j].points(rectLine);
        for(int k=0;k<4;k++)
        {
            line(Img_Nano,rectLine[k],rectLine[(k+1)%4],Scalar(0,255,255),2);
        }
    }
    // 绘制棋盘角点内轮廓矩形框
    for(int j = 0;j < insideRect.size();j++)
    {
        insideRect[j].points(rectLine);
        for(int k=0;k<4;k++)
        {
            line(Img_Nano,rectLine[k],rectLine[(k+1)%4],Scalar(0,255,255),2);
        }
    }
    // 绘制参考向量
    for(int i = 0;i < outsideRect.size();i++)
    {
        Point RefVectorPoint;
        float Angle_Rect = 0;   // 右边的夹角
        int RevolveDir = 0; // 0：45度 1：逆时针 2：顺时针

        // 判断棋盘旋转方向
        Point2f PointRect[4];
        for(int i = 0;i < outsideRect.size();i++)
        {
            outsideRect[i].points(PointRect);
            if(PointRect[0].x > outsideRect[i].center.x)
            {
                RevolveDir = 1;
            }
            else if(PointRect[0].x < outsideRect[i].center.x)
            {
                RevolveDir = 2;
            }
            else
            {
                RevolveDir = 0;
            }
        }
        // 角度计算
        if(outsideRect[0].angle != 0)
        {
            if (RevolveDir == 0)
            {
                Angle_Rect = 45;
            }
            else if (RevolveDir == 1)
            {
                Angle_Rect = outsideRect[0].angle;
            }
            else if (RevolveDir == 2)
            {
                Angle_Rect = 90 - abs(outsideRect[0].angle);
            }
            if (abs(outsideRect[0].angle < 0.1 && abs(outsideRect[0].angle) > 89.9)
            {
                Angle_Rect = 0;
            }
        }
        else
        {
            Angle_Rect = 90;
        }
        // 参考向量
        RefVectorPoint.x = (int)(20*cos(Angle_Rect/(180/PI)));
        RefVectorPoint.y = (int)(20*sin(Angle_Rect/(180/PI)));
        
        line(Img_Nano,outsideRect[0].center,Point(outsideRect[0].center.x+4*RefVectorPoint.x,outsideRect[0].center.y+4*RefVectorPoint.y),Scalar(0,0,0),2);
    }
}