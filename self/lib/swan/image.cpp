#include <iostream>
#include <cmath>
#include <thread>
#include <stdio.h>
#include <stdlib.h>    
#include <string.h>
#include <unistd.h>    
#include <sys/types.h>  
#include <sys/time.h>
#include <sys/stat.h>   
#include <fcntl.h>      
#include <termios.h>  
#include <errno.h>
#include <vector>
#include <opencv2/opencv.hpp>

#include "swan/image.h"

using namespace std;
using namespace cv;
using namespace swan;


// 图像色彩空间转化
void SwanImage::colorConvert(Mat& Img,cv::ColorConversionCodes ColorConversionCode)
{
	cvtColor(Img,Img,ColorConversionCode);
}


// 灰度图像二值化
void SwanImage::BGR::thresholdConvert(Mat& Img)
{
	threshold(Img , Img , 0 , 255 , THRESH_BINARY | THRESH_OTSU);   //灰度图像二值化
}


// BGR图像通道提取
void SwanImage::BGR::channelSplit(Mat& Img,_BGR_Channel_ BGR_Channel)
{
	vector<Mat> Channel;
	Mat BGR_Img;
	split(Img,Channel);	// 分离Img三通道
	switch(BGR_Channel)
	{
		case B_Channel:{ Channel[1] = Scalar(0); Channel[2] = Scalar(0); break;}
		case G_Channel:{ Channel[0] = Scalar(0); Channel[2] = Scalar(0); break;}
		case R_Channel:{ Channel[0] = Scalar(0); Channel[1] = Scalar(0); break;}
	}

	merge(Channel,BGR_Img);	// 合成Img三通道
}


// 峰值对焦
float SwanImage::BGR::focusPeaking(Mat& Img,_Focus_Core_Size_ FocusCoreSize,float FocusCoreVarianceLimit)
{
	SwanImage SwanImage;
	int Width = Img.cols;
	int Height = Img.rows;

	vector<float> FocusCoreGrayScaleVariances;	// 对焦区域对焦核灰度方差
	float FocusCoreGrayScaleVariance = 0;	// 对焦核灰度方差
	float FocusCoreGrayScaleAverage = 0;	// 对焦核灰度均值
	vector<int> PixelGrayScale;	// 对焦核内各个像素灰度值
	float FocusCoreGrayScaleVarianceAverage = 0;	// 对焦区域对焦核灰度方差均值

	SwanImage::_Point_ FocusCorePoint;
	vector<SwanImage::_Point_> FocusCorePoints;	// 对焦核左上角坐标
	vector<int> FocusCoreVariances;	// 对焦核方差

	Mat ImgFocus = Img.clone();
	SwanImage.colorConvert(ImgFocus,COLOR_BGRA2GRAY);

	for(int i = 0;i <= Width-FocusCoreSize+1;i++)
	{
		for(int j = 0;j <= Height-FocusCoreSize+1;j++)
		{
			FocusCoreGrayScaleVariance = 0;
			FocusCoreGrayScaleAverage = 0;
			PixelGrayScale.clear();
			for(int k = 0;k <= FocusCoreSize;k++)
			{	
				for(int l = 0;l <= FocusCoreSize;l++)
				{
					PixelGrayScale.push_back(ImgFocus.at<uchar>(j+k,i+l));
					FocusCoreGrayScaleAverage += ImgFocus.at<uchar>(j+k,i+l);
				}
			}
			FocusCoreGrayScaleAverage = FocusCoreGrayScaleAverage/pow((float)FocusCoreSize,2);
			for(int m = 0;m < PixelGrayScale.size();m++)
			{
				FocusCoreGrayScaleVariance += pow(PixelGrayScale[m]-FocusCoreGrayScaleAverage,2);
				FocusCoreGrayScaleVariance = FocusCoreGrayScaleVariance/PixelGrayScale.size();
			}
			if(FocusCoreGrayScaleVariance >= FocusCoreVarianceLimit)
			{
				FocusCorePoint.X = i;
				FocusCorePoint.Y = j;
				FocusCoreGrayScaleVariances.push_back(FocusCoreGrayScaleVariance);
				FocusCoreGrayScaleVarianceAverage += FocusCoreGrayScaleVariances[FocusCoreGrayScaleVariances.size()-1];
				FocusCorePoints.push_back(FocusCorePoint);
				circle(Img,cv::Point(FocusCorePoint.X,FocusCorePoint.Y),1,Scalar(0,0,255),1);	// 峰值对焦区域绘制
			}
		}
	}
	FocusCoreGrayScaleVarianceAverage = FocusCoreGrayScaleVarianceAverage/FocusCoreGrayScaleVariances.size();
	return FocusCoreGrayScaleVarianceAverage;
}


// HSV图像亮度直方图均衡化
void SwanImage::HSV::Equalize(cv::Mat& Img)
{
	vector<Mat> HSV_Channel;
	split(Img,HSV_Channel);
	equalizeHist(HSV_Channel[2],HSV_Channel[2]);
	merge(HSV_Channel,Img);
}


// HSV图像二值化
void SwanImage::HSV::thresholdConvert(Mat& Img,Scalar LowLimit,Scalar HighLimit)
{
	inRange(Img,LowLimit,HighLimit,Img);	// 该函数可以实现多阈值二值化
}


// SOBEL算子边缘检测
void SwanImage::Sobel(Mat& Img,int sobelCore)
{
	Mat ImgX;
	Mat ImgY;

	//对X方向微分
    cv::Sobel(Img,ImgX,CV_16S,1,0,sobelCore); 	//x方向差分阶数 y方向差分阶数 核大小  
    convertScaleAbs(ImgX,ImgX);     //可将任意类型的数据转化为CV_8UC1
	//对Y方向微分
	cv::Sobel(Img,ImgY,CV_16S,0,1,sobelCore); 	//x方向差分阶数 y方向差分阶数 核大小  
    convertScaleAbs(ImgY,ImgY);     //将任意类型的图像转化为CV_8UC1

    addWeighted(ImgX,0.5,ImgY,0.5,0,Img);	//图像的线性混合
}


// SCHARR算子边缘检测
void SwanImage::Scharr(Mat& Img,int scharrCore)
{
	Mat ImgX;
	Mat ImgY;

	//对X方向微分
    cv::Scharr(Img,ImgX,CV_16S,1,0,scharrCore); 	//x方向差分阶数 y方向差分阶数 核大小  
    convertScaleAbs(ImgX,ImgX);     //可将任意类型的数据转化为CV_8UC1
	//对Y方向微分
	cv::Scharr(Img,ImgY,CV_16S,0,1,scharrCore); 	//x方向差分阶数 y方向差分阶数 核大小  
    convertScaleAbs(ImgY,ImgY);     //将任意类型的图像转化为CV_8UC1

    addWeighted(ImgX,0.5,ImgY,0.5,0,Img);	//图像的线性混合
}


// CANNY算子边缘检测
void SwanImage::Canny(Mat& Img,int Low,int High,int Size)
{
	cv::Canny(Img,Img,Low,High,Size,false);
}


// 高斯模糊
void SwanImage::GaussBlur(Mat& Img,int BlurSize)
{
	GaussianBlur(Img,Img,Size(BlurSize,BlurSize),3,3);
}


// 图像锐化
void SwanImage::Sharpen(Mat& Img,int BlurSize)
{
	Mat Img_Gauss;
	GaussianBlur(Img,Img_Gauss,Size(BlurSize,BlurSize),3,3);
	addWeighted(Img,2,Img_Gauss,-1,0,Img);
}


// 图形学膨胀
void SwanImage::Dilate(Mat& Img,int CoreSize)
{
	Mat Core = getStructuringElement(MORPH_RECT,Size(CoreSize,CoreSize));
	dilate(Img,Img,Core);
}


// 图形学腐蚀
void SwanImage::Erode(Mat& Img,int CoreSize)
{
	Mat Core = getStructuringElement(MORPH_RECT,Size(CoreSize,CoreSize));
	erode(Img,Img,Core);
}


// 透视变换
void SwanImage::Unpivot(Mat& Img,Point2f *SrcPoints,Point2f *DstPoints)
{
	Mat UnpivotMat = getPerspectiveTransform(SrcPoints , DstPoints);

    warpPerspective(Img,Img,UnpivotMat,Size(320,240),INTER_LINEAR);
}


// 图像读取
cv::Mat Read(const std::string ImgSave_Path)
{
	return imread(ImgSave_Path);
}


// 图像存储
void SwanImage::Save(Mat Img,const string ImgSave_Path,int ImgNum,_ImgFormat_ ImgFormat)
{
    string ImgSavePath;
    switch(ImgFormat)
    {
        case JPG:{ ImgSavePath = ImgSave_Path + "/" + to_string(ImgNum) + ".jpg"; break; }
        case JPEG:{ ImgSavePath = ImgSave_Path + "/" + to_string(ImgNum) + ".jpeg"; break; }
        case PNG:{ ImgSavePath = ImgSave_Path + "/" + to_string(ImgNum) + ".png"; break; }
    }
	//建立一个字符串用于存储图片存储路径
	//使用字符串定义图片存储路径
	//必须要加后缀 否则编译会报错

	imwrite(ImgSavePath , Img);
	//存储图片流

	cout << "photo has been store at: " << ImgSave_Path << endl;
}


// 待合成图像添加
int SwanImage::Add(cv::Mat Img)
{
	// 图像归一化处理
	// 将所有非8UC3类型的图像转为8UC3
	if(Img.type() != CV_8UC3)
	{
		cvtColor(Img,Img,COLOR_GRAY2RGB);
	}
	ImgSynthesis.push_back(Img.clone());
	return ImgSynthesis.size();
}


// 合成图像并显示
Mat SwanImage::Show()
{
	int ImgWidth = ImgSynthesis[0].cols;
	int ImgHeight = ImgSynthesis[0].rows;
	Mat Img = Mat(ImgHeight,ImgWidth*ImgSynthesis.size()+ImgSynthesis.size()-1,CV_8UC3,Scalar(0,0,0));	//显示全部画面的画布
	for(int i = 0;i < ImgSynthesis.size();i++)
	{
		ImgSynthesis[i].copyTo(Img(Rect(ImgWidth*i+i,0,ImgWidth,ImgHeight))); 
	}
	imshow("Img",Img);
	ImgSynthesis.clear();
	return Img;
}


// 获取合成图像
Mat SwanImage::getSynthesisImg()
{
	int ImgWidth = ImgSynthesis[0].cols;
	int ImgHeight = ImgSynthesis[0].rows;
	Mat Img = Mat(ImgHeight,ImgWidth*ImgSynthesis.size()+ImgSynthesis.size()-1,CV_8UC3,Scalar(0,0,0));	//显示全部画面的画布
	for(int i = 0;i < ImgSynthesis.size();i++)
	{
		ImgSynthesis[i].copyTo(Img(Rect(ImgWidth*i+i,0,ImgWidth,ImgHeight))); 
	}
	ImgSynthesis.clear();
	return Img;
}


// 轮廓寻找
void SwanImage::findContours(Mat Img,vector<vector<Point>>& Contours,vector<Vec4i>& Hierarchy,RetrievalModes Mode,ContourApproximationModes Method)
{
	cv::findContours(Img,Contours,Hierarchy,Mode,Method,Point(0,0));
}


// 轮廓绘制
void SwanImage::drawContours(Mat& Img,vector<vector<Point>> Contours,vector<Vec4i> Hierarchy,Scalar Color)
{
	for(int i = 0;i < Contours.size();i++)
	{
		cv::drawContours(Img,Contours,i,Color,2,8,Hierarchy,0);
	}
}


// 轮廓区域最小外接矩形类获取
void SwanImage::findContoursRect(vector<vector<Point>> Contours,vector<RotatedRect>& Rect)
{
	Rect.clear();
	for(int i = 0;i < Contours.size();i++)
	{
		Rect.push_back(minAreaRect(Contours[i]));
	}
}


// 轮廓中心点寻找
void SwanImage::findContoursCenter(vector<RotatedRect>& Rect,vector<Point>& Centers)
{
	Centers.clear();
	for(int i = 0;i < Rect.size();i++)
	{
		Centers.push_back(Rect[i].center);
	}
}


// 轮廓匹配矩形绘制
void SwanImage::drawContoursRect(vector<RotatedRect>& Rect)
{
	// 矩形四角点坐标解析
	
}


// 图像FPS计算显示
void SwanImage::FPS(Mat& Img,int X,int Y,int FPSperiod)
{
	struct timeval tv;
	static vector<double> StartTime;
	int FPS;

	// 记录FPS计算周期内的时间
	StartTime.push_back(tv.tv_usec);
	if(StartTime.size() >= FPSperiod)
	{
		double us2s = (tv.tv_usec-StartTime[0])/1000000;	// 将FPS计算周期内的时间间隔转换成s
		FPS = int(FPSperiod/(us2s));	// 计算FPS
		StartTime.erase(StartTime.begin());	// 删除首元素
	}
	cout << "FPS: " << FPS << endl;
	putText(Img,to_string(FPS),cv::Point(X,Y),FONT_HERSHEY_COMPLEX,1,(255,0,255),1);
}


// 新建画布
void SwanImage::newImg(Mat& Img,int Width,int Height,int ImgType,cv::Scalar Color)
{
	Img = Mat(Height,Width,ImgType,Color);	//显示全部画面的画布
}

// 图像压缩
// Mat libImage::ImgCompress(Mat& Img)
// {
// 	Mat ImgCompress;
//     Size size = Size(320,240);
//     resize(Img,ImgCompress,size,0,0,INTER_AREA);
//     //将图像压缩为320*240大小
//     Img = ImgCompress;
// }
