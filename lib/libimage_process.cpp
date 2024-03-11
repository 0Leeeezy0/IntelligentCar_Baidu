#include "common_system.h"
#include "common_program.h"

using namespace std;
using namespace cv;

/*
	ImgPrepare说明
	图像预处理
	先二值化使赛道边缘更为清晰
	然后用sobel算子检测边缘
	最后再次二值化
*/
void ImgProcess::ImgPrepare(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
	ImgProcess::ImgUnpivot((Img_Store_p -> Img_Color) , (Img_Store_p -> Img_Color_Unpivot));
    (Img_Store_p -> Img_Track_Unpivot) = (Img_Store_p -> Img_Color_Unpivot).clone();
	cvtColor((Img_Store_p -> Img_Color_Unpivot) , (Img_Store_p -> Img_Gray_Unpivot) , COLOR_BGR2GRAY);  //彩色图像灰度化
	// blur((Img_Store_p -> Img_Gray_Unpivot) , (Img_Store_p -> Img_Gray_Unpivot) , Size(18,18) , Point(-1,-1));	//均值滤波	
	threshold((Img_Store_p -> Img_Gray_Unpivot) , (Img_Store_p -> Img_OTSU_Unpivot) , 0 , 255 , THRESH_BINARY | THRESH_OTSU);   //灰度图像二值化
	ImgProcess::ImgSobel((Img_Store_p -> Img_OTSU_Unpivot));	//Sobel算子处理
	threshold((Img_Store_p -> Img_OTSU_Unpivot) , (Img_Store_p -> Img_OTSU_Unpivot) , 0 , 255 , THRESH_BINARY | THRESH_OTSU);   //灰度图像二值化
	ImgProcess::ImgSharpen((Img_Store_p -> Img_OTSU_Unpivot),5);
	// 加白框防止八邻域寻线出错
	line((Img_Store_p -> Img_OTSU_Unpivot),Point(0,0),Point(319,0),Scalar(255),3);
	line((Img_Store_p -> Img_OTSU_Unpivot),Point(319,0),Point(319,239),Scalar(255),3);
	line((Img_Store_p -> Img_OTSU_Unpivot),Point(319,239),Point(0,239),Scalar(255),3);
	line((Img_Store_p -> Img_OTSU_Unpivot),Point(0,239),Point(0,0),Scalar(255),3);
}


/*
	ImgSobel说明
	Sobel算子检测边缘
	将传入的图像用Sobel算子处理
*/
void ImgProcess::ImgSobel(Mat& Img)
{
	Mat ImgX;
	Mat ImgY;

	//对X方向微分
    Sobel(Img,ImgX,CV_16S,1,0,5); 	//x方向差分阶数 y方向差分阶数 核大小  
    convertScaleAbs(ImgX,ImgX);     //可将任意类型的数据转化为CV_8UC1
	//对Y方向微分
	Sobel(Img,ImgY,CV_16S,0,1,5); 	//x方向差分阶数 y方向差分阶数 核大小  
    convertScaleAbs(ImgY,ImgY);     //将任意类型的图像转化为CV_8UC1
    addWeighted(ImgX,0.5,ImgY,0.5,0,Img);	//图像的线性混合
}


/*
	ImgScharr说明
	Scharr算子检测边缘
	将传入的逆透视边缘二值化图像用Scharr算子处理
*/
void ImgProcess::ImgScharr(Mat& Img)
{
	Mat ImgX;
	Mat ImgY;

	//对X方向微分
    Scharr(Img,ImgX,CV_16S,1,0,3); 	//x方向差分阶数 y方向差分阶数 核大小  
    convertScaleAbs(ImgX,ImgX);     //可将任意类型的数据转化为CV_8UC1
	//对Y方向微分
	Scharr(Img,ImgY,CV_16S,1,0,3); 	//x方向差分阶数 y方向差分阶数 核大小  
    convertScaleAbs(ImgY,ImgY);     //将任意类型的图像转化为CV_8UC1
    addWeighted(ImgX,0.5,ImgY,0.5,0,Img);	//图像的线性混合
}


/*
	ImgSharpen说明
	通过原图像和高斯滤波图像进行融合进行图像锐化
*/
void ImgProcess::ImgSharpen(Mat &Img,int blursize = 5)
{
	Mat Img_Gauss;
	GaussianBlur(Img,Img_Gauss,Size(blursize,blursize),3,3);
	addWeighted(Img,2,Img_Gauss,-1,0,Img);
}
/*
	ImgUnpivot说明
	逆透视
*/
void ImgProcess::ImgUnpivot(Mat Img,Mat& Img_Unpivot)
{
    Point2f SrcPoints[] = { 
		Point2f(0,240),
		Point2f(320,240),
		Point2f(115,25),
		Point2f(205,25) };
 
	Point2f DstPoints[] = {
		Point2f(80,240),
		Point2f(240,240),
		Point2f(80,0),
		Point2f(240,0) };
 
	Mat UnpivotMat = getPerspectiveTransform(SrcPoints , DstPoints);

    warpPerspective(Img , Img_Unpivot , UnpivotMat , Size(320,240) , INTER_LINEAR);
}


/*
	ImgSynthesis说明
	将多个图像合成在同一窗口
*/
void ImgProcess::ImgSynthesis(Img_Store *Img_Store_p)
{
	int ImgAllWidth = (Img_Store_p -> Img_Color).cols;	//宽度
	int ImgAllHeight = (Img_Store_p -> Img_Color).rows; //高度
	Mat ImgAll = Mat(ImgAllHeight,ImgAllWidth*3+18,CV_8UC3,Scalar(0,0,0));	//显示全部画面的画布

	//统一图像的类型为8UC3
	cvtColor((Img_Store_p -> Img_OTSU_Unpivot) , (Img_Store_p -> Img_OTSU_Unpivot) ,COLOR_GRAY2RGB);

	//Rect roi(ImgAllWidth*i,0,ImgAllWidth,ImgAllHeight);  
	//定义一个矩形roi
	//将img_tmp复制到img中roi指定的矩形位置
	//此处简化
    
	(Img_Store_p -> Img_Color).copyTo(ImgAll(Rect(0,0,ImgAllWidth,ImgAllHeight))); 
	(Img_Store_p -> Img_Track_Unpivot).copyTo(ImgAll(Rect(ImgAllWidth+6,0,ImgAllWidth,ImgAllHeight)));  
	(Img_Store_p -> Img_OTSU_Unpivot).copyTo(ImgAll(Rect(ImgAllWidth*2+12,0,ImgAllWidth,ImgAllHeight))); 

    (Img_Store_p -> Img_All) = ImgAll;
	imshow("CAMERA",(Img_Store_p -> Img_All));
}


/*
	ImgSave说明
	以Mat形式传入待存储图像
*/
void ImgProcess::ImgSave(Img_Store *Img_Store_p)
{
	string ImgWritePath = "../img/ImgAll/" + to_string(Img_Store_p -> ImgNum) + ".jpg";
	//建立一个字符串用于存储图片存储路径
	//使用字符串定义图片存储路径
	//必须要加后缀 否则编译会报错

	imwrite(ImgWritePath , (Img_Store_p -> Img_All));
	//存储图片流
}


/*
	ImgForwardLine说明
	前瞻点画线
*/
void ImgProcess::ImgForwardLine(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
    line((Img_Store_p -> Img_Track_Unpivot),Point(160,300),Point((Data_Path_p -> TrackCoordinate[(Data_Path_p -> Forward)-(Data_Path_p -> Path_Search_Start)][0]),(Data_Path_p -> TrackCoordinate[(Data_Path_p -> Forward)-(Data_Path_p -> Path_Search_Start)][1])),Scalar(255,0,0),3);
	putText((Img_Store_p -> Img_Track_Unpivot),to_string(abs(160-(Data_Path_p -> TrackCoordinate[(Data_Path_p -> Forward)-(Data_Path_p -> Path_Search_Start)][0]))),Point((Data_Path_p -> TrackCoordinate[(Data_Path_p -> Forward)-(Data_Path_p -> Path_Search_Start)][0]),(Data_Path_p -> TrackCoordinate[(Data_Path_p -> Forward)-(Data_Path_p -> Path_Search_Start)][1])),FONT_HERSHEY_COMPLEX,0.6,(255,255,255),1);
}


/*
	ImgCompress说明
	图像压缩
	将图像压缩至320*240大小
*/
void ImgProcess::ImgCompress(Mat& Img,bool ImgCompress_EN)
{
	Mat ImgCompress;
	if(ImgCompress_EN == true)
	{
		Size size = Size(320,240);
		resize(Img,ImgCompress,size,0,0,INTER_AREA);
		//将图像压缩为320*240大小
		Img = ImgCompress;
	}
}


/*
	ImgShow说明
	图像合成显示并保存
*/
void ImgProcess::ImgShow(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
	ImgProcess::ImgInterrupt(Img_Store_p,Data_Path_p); 
	ImgProcess::ImgForwardLine(Img_Store_p,Data_Path_p);
	ImgProcess::ImgReferenceLine(Img_Store_p,Data_Path_p);
	ImgProcess::ImgSynthesis(Img_Store_p);
	ImgProcess::ImgSave(Img_Store_p);
}


/*
    ImgInterrupt说明
	图像边线断点绘制
	限制绘制数目
*/
void ImgProcess::ImgInterrupt(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
	// 变量设置
	int i = 0;
	int j = 0;
	// 左边线中断点绘制
	if((Data_Path_p -> InterruptNum[0]) >= 1)
	{
		for(i = 0;i <= (Data_Path_p -> InterruptNum[0])-1;i++)
		{
			if(i == 0)
			{
				putText((Img_Store_p -> Img_Track_Unpivot),to_string(Data_Path_p -> InterruptNum[0]),Point((Data_Path_p -> InterruptCoordinate[i][0]),(Data_Path_p -> InterruptCoordinate[i][1])),FONT_HERSHEY_COMPLEX,0.6,(255,0,255),1);
				circle((Img_Store_p -> Img_Track_Unpivot),Point((Data_Path_p -> InterruptCoordinate[i][0]),(Data_Path_p -> InterruptCoordinate[i][1])),6,Scalar(0,255,255),2);	// 左边线断点画点：黄色
			}
			else
			{
				circle((Img_Store_p -> Img_Track_Unpivot),Point((Data_Path_p -> InterruptCoordinate[i][0]),(Data_Path_p -> InterruptCoordinate[i][1])),6,Scalar(255,255,0),2);	// 左边线断点画点：蓝色
			}
		}
	}
	// 右边线中断点绘制
	if((Data_Path_p -> InterruptNum[1]) >= 1)
	{
		for(j = 0;j <= (Data_Path_p -> InterruptNum[1])-1;j++)
		{
			if(j == 0)
			{
				putText((Img_Store_p -> Img_Track_Unpivot),to_string(Data_Path_p -> InterruptNum[1]),Point((Data_Path_p -> InterruptCoordinate[j][2]),(Data_Path_p -> InterruptCoordinate[j][3])),FONT_HERSHEY_COMPLEX,0.6,(255,0,255),1);
				circle((Img_Store_p -> Img_Track_Unpivot),Point((Data_Path_p -> InterruptCoordinate[j][2]),(Data_Path_p -> InterruptCoordinate[j][3])),6,Scalar(0,255,255),2);	// 右边线断点画点：黄色
			}
			else
			{
				circle((Img_Store_p -> Img_Track_Unpivot),Point((Data_Path_p -> InterruptCoordinate[j][2]),(Data_Path_p -> InterruptCoordinate[j][3])),6,Scalar(255,255,0),2);	// 右边线断点画点：蓝色
			}
		}
	}
}


/*
	ImgReferenceLine说明
	图像参考线绘制
	1.边线断点起始线
	2.边线断点结束线
	3.中心竖线
*/
void ImgProcess::ImgReferenceLine(Img_Store *Img_Store_p,Data_Path *Data_Path_p)
{
	line((Img_Store_p -> Img_Track_Unpivot),Point(0,239-(Data_Path_p -> Path_Search_Start)),Point(319,239-(Data_Path_p -> Path_Search_Start)),Scalar(0,0,255),1);	// 寻路径起始线
	line((Img_Store_p -> Img_Track_Unpivot),Point(0,239-(Data_Path_p -> Path_Search_End)),Point(319,239-(Data_Path_p -> Path_Search_End)),Scalar(0,0,255),1);	// 寻路径结束线
	line((Img_Store_p -> Img_Track_Unpivot),Point(0,239-(Data_Path_p -> Side_Search_Start)),Point(319,239-(Data_Path_p -> Side_Search_Start)),Scalar(255,0,255),1);	// 寻边线起始线
	line((Img_Store_p -> Img_Track_Unpivot),Point(0,239-(Data_Path_p -> Side_Search_End)),Point(319,239-(Data_Path_p -> Side_Search_End)),Scalar(255,0,255),1);	// 寻边线结束线
	line((Img_Store_p -> Img_Track_Unpivot),Point(160,239-(Data_Path_p -> Path_Search_Start)),Point(160,239-(Data_Path_p -> Path_Search_End)),Scalar(0,0,255),1);	// 中心竖线
}
