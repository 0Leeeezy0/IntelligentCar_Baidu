// CheckerBoardDemo.cpp : 定义控制台应用程序的入口点。
//

//#include <stdafx.h>
// #include <opencv2\imgproc\types_c.h>
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;

Mat image, img_gray;
int BOARDSIZE[2]{ 6,9 };//棋盘格每行每列角点个数
int main()
{
	vector<vector<Point3f>> objpoints_img;//保存棋盘格上角点的三维坐标
	vector<Point3f> obj_world_pts;//三维世界坐标
	vector<vector<Point2f>> images_points;//保存所有角点
	vector<Point2f> img_corner_points;//保存每张图检测到的角点
	vector<String> images_path;//创建容器存放读取图像路径

    string ImgPath;
	cout << "Img path: ";
	cin >> ImgPath;
	glob(ImgPath, images_path);//读取指定文件夹下图像

	//转世界坐标系
	for (int i = 0; i < BOARDSIZE[1]; i++)
	{
		for (int j = 0; j < BOARDSIZE[0]; j++)
		{
			obj_world_pts.push_back(Point3f(j, i, 0));
		}
	}

	for (int i = 0; i < images_path.size(); i++)
	{
		image = imread(images_path[i]);
		cvtColor(image, img_gray, COLOR_BGR2GRAY);
		//检测角点
		bool found_success = findChessboardCorners(img_gray, Size(BOARDSIZE[0], BOARDSIZE[1]),
			img_corner_points,
			CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);

		//显示角点
		if (found_success)
		{
			//迭代终止条件
			TermCriteria criteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.001);

			//进一步提取亚像素角点
			cornerSubPix(img_gray, img_corner_points, Size(11, 11),
				Size(-1, -1), criteria);

			//绘制角点
			drawChessboardCorners(image, Size(BOARDSIZE[0], BOARDSIZE[1]), img_corner_points,
				found_success);

			objpoints_img.push_back(obj_world_pts);//从世界坐标系到相机坐标系
			images_points.push_back(img_corner_points);
		}
		//char *output = "image";
		char text[] = "image";
		char *output = text;
		imshow(output, image);
		waitKey(200);

	}

	/*
	计算内参和畸变系数等
	*/

	Mat cameraMatrix, distCoeffs, R, T;//内参矩阵，畸变系数，旋转量，偏移量
	calibrateCamera(objpoints_img, images_points, img_gray.size(),
		cameraMatrix, distCoeffs, R, T);

	cout << "cameraMatrix:" << endl;
	cout << cameraMatrix << endl;

	cout << "*****************************" << endl;
	cout << "distCoeffs:" << endl;
	cout << distCoeffs << endl;
	cout << "*****************************" << endl;

	cout << "Rotation vector:" << endl;
	cout << R << endl;

	cout << "*****************************" << endl;
	cout << "Translation vector:" << endl;
	cout << T << endl;

	///*
	//畸变图像校准
	//*/
    for(int i = 0;i < images_path.size();i++)
    {
        Mat src, dst;
        src = imread(images_path[i]); 
        undistort(src, dst, cameraMatrix, distCoeffs);

        char texts[] = "image_dst";
        char *dst_output = texts;
        //char *dst_output = "image_dst";
        imshow(dst_output, dst);
        waitKey(100);
        string SavePath = "../img/LDCoutput/" + to_string(i+1) + ".jpg";
        imwrite(SavePath, dst);

        destroyAllWindows();//销毁显示窗口
        system("pause");
    }
	
	return 0;
}

