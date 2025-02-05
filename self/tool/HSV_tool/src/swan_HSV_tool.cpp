#include<opencv2/opencv.hpp>
using namespace cv;
#include<iostream>
#include<string>
using namespace std;
//输入图像
Mat Img;
//灰度值归一化
Mat BGR;
//HSV图像
Mat HSV;
//输出图像
Mat Dst;
//色相
int hmin = 0;
int hmin_Max = 180;
int hmax = 180;
int hmax_Max = 180;
//饱和度
int smin = 0;
int smin_Max = 255;
int smax = 255;
int smax_Max = 255;
//亮度
int vmin = 106;
int vmin_Max = 255;
int vmax = 255;
int vmax_Max = 255;
//显示原图的窗口
string srcWindowName = "INITIAL IMAGE";
//显示HSV图像的窗口
string HSVwindowName = "HSV IMAGE";
//输出图像的显示窗口
string ThresholdWindowName = "HSV THRESHOLD IMAGE";

//回调函数
void callBack(int, void*)
{
	//输出图像分配内存
	Dst = Mat::zeros(Img.size(), Img.type());
	//掩码
	Mat mask;
	inRange(HSV, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), mask);
	// 掩模到原图的转换
	for (int r = 0; r < BGR.rows; r++)
	{
		for (int c = 0; c < BGR.cols; c++)
		{
			if (mask.at<uchar>(r, c) == 255)
			{
				Dst.at<Vec3b>(r, c) = BGR.at<Vec3b>(r, c);
			}
		}
	}
	//输出图像
	imshow(ThresholdWindowName, mask);
	//保存图像
	//Dst.convertTo(Dst, CV_8UC3, 255.0, 0);
	// imwrite("HSV_inRange.jpg", Dst);
}

int main()
{
	string ImgPath;
	cout << "Img path: ";
	cin >> ImgPath;

	//输入图像
	Img = imread(ImgPath);
	if (!Img.data || Img.channels() != 3)
		return -1;
	imshow(srcWindowName, Img);
	BGR = Img.clone();

	//颜色空间转换
	cvtColor(BGR, HSV, COLOR_BGR2HSV);
	
	// HSV图像亮度阈值调整
	// vector<Mat> HSV_Channel;
	// split(HSV,HSV_Channel);
	// equalizeHist(HSV_Channel[2],HSV_Channel[2]);
	// merge(HSV_Channel,HSV);

	imshow(HSVwindowName, HSV);
	// cout << HSV << endl;
	//定义输出图像的显示窗口
	namedWindow(ThresholdWindowName, WINDOW_GUI_EXPANDED);
	//调节色相 H
	createTrackbar("hmin", ThresholdWindowName, &hmin, hmin_Max, callBack);
	createTrackbar("hmax", ThresholdWindowName, &hmax, hmax_Max, callBack);
	//调节饱和度 S
	createTrackbar("smin", ThresholdWindowName, &smin, smin_Max, callBack);
	createTrackbar("smax", ThresholdWindowName, &smax, smax_Max, callBack);
	//调节亮度 V
	createTrackbar("vmin", ThresholdWindowName, &vmin, vmin_Max, callBack);
	createTrackbar("vmax", ThresholdWindowName, &vmax, vmax_Max, callBack);
	callBack(0, 0);
	waitKey(0);
	return 0;
}