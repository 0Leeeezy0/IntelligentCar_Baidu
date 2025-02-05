#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>    
#include <string.h>
#include <mutex>
#include <unistd.h>    
#include <sys/types.h>  
#include <sys/stat.h>   
#include <fcntl.h>      
#include <termios.h>  
#include <errno.h>
#include <vector>
#include <opencv2/opencv.hpp>

#include "swan/camera.h"

using namespace std;
using namespace cv;
using namespace swan;

mutex CameraCapture_Mutex;  // 摄像头采集资源互斥锁

// 摄像头初始化
int SwanCamera::Init()
{
    int CameraInitResult;  // 摄像头初始化结果
    
    switch(CameraInitParameter.CameraFramework)
    {
        case V4L2:{ Camera.open(CameraInitParameter.CameraDevice,CAP_V4L2); break; }
        case COMMON:{ Camera.open(CameraInitParameter.CameraDevice); break; }
    }
    if(!Camera.isOpened())
    {
        CameraInitResult = -1;
    }
    else 
    {
        CameraInitResult = 1;
    }

    // 设置摄像头参数
    switch(CameraInitParameter.VideoCurrentFormat)
    {
        case MJPG:{ Camera.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G')); break; }  // 设置视频流格式
        case MPEG:{ Camera.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'P', 'E','G')); break; }  // 设置视频流格式
    }

    Camera.set(CAP_PROP_FRAME_WIDTH, CameraInitParameter.CameraResolutionWidth); // 设置摄像头横向分辨率
    Camera.set(CAP_PROP_FRAME_HEIGHT, CameraInitParameter.CameraResolutionHeight);  // 设置摄像头纵向分辨率
    Camera.set(CAP_PROP_FPS, CameraInitParameter.CameraFPS);    // 设置摄像头帧率
    Camera.set(CAP_PROP_EXPOSURE, CameraInitParameter.CameraExposure); // 设置摄像头曝光度

    // 摄像头初始化信息显示
    cout << "<------------------------------camera module init info------------------------------>" << endl;
    cout << "FPS: " << CameraInitParameter.CameraFPS << endl;
    cout << "exposure: " << CameraInitParameter.CameraExposure << endl;
    cout << "current format: "; 
    switch(CameraInitParameter.VideoCurrentFormat)
    {
        case MJPG:{ cout << "MJPG" << endl; break; }
        case MPEG:{ cout << "MPEG" << endl; break; }
    }
    cout << "res: " << CameraInitParameter.CameraResolutionWidth << "*" << CameraInitParameter.CameraResolutionHeight << endl; 
    cout << "multiple thread: ";
    switch(CameraInitParameter.CameraMultiThreading)
    {
        case true:{ cout << "OPEN" << endl; break; }
        case false:{ cout << "CLOSE" << endl; break; }
    }
    switch(CameraInitResult)
    {
        case -1:{ cout << "camera device: " + CameraInitParameter.CameraDevice + " open failed" << endl; break; }
        case 1:{ cout << "camera device: " + CameraInitParameter.CameraDevice + " open successful" << endl; break; }
    }
    cout << "<----------------------------------------------------------------------------------->" << endl;

    return CameraInitResult;
}


// 摄像头图像获取
void SwanCamera::Capture()
{
    Mat ImgCapture;
    // 将视频流转为图像流
    if(CameraInitParameter.CameraMultiThreading == true)
    {
        thread CameraCapture_Thread (&SwanCamera::captureThread,this,ref(Camera)); 
        CameraCapture_Thread.detach();
    }
    else
    {
        Camera >> ImgCapture;
        if(!Img.empty());
        Img.push(ImgCapture);
    }
}


// 摄像头初始化信息获取
void SwanCamera::initInfosGet(string CameraDevice,_CameraFramework_ CameraFramework,_CameraFPS_ CameraFPS,_CameraExposure_ CameraExposure,bool CameraAutoWhiteBalance,_VideoCurrentFormat_  VideoCurrentFormat,_CameraResolutionWidth_ CameraResolutionWidth,_CameraResolutionHeight_ CameraResolutionHeight,bool CameraMultiThreading)
{
    CameraInitParameter.CameraDevice = CameraDevice;
    CameraInitParameter.CameraFramework = CameraFramework;
    CameraInitParameter.CameraFPS = CameraFPS;
    CameraInitParameter.CameraExposure = CameraExposure;
    CameraInitParameter.CameraAutoWhiteBalance = CameraAutoWhiteBalance;
    CameraInitParameter.VideoCurrentFormat = VideoCurrentFormat;
    CameraInitParameter.CameraResolutionWidth = CameraResolutionWidth;
    CameraInitParameter.CameraResolutionHeight = CameraResolutionHeight;
    CameraInitParameter.CameraMultiThreading = CameraMultiThreading;
}


// 多线程摄像头图像获取
void SwanCamera::captureThread(VideoCapture &Camera)
{
    Mat ImgCapture;
    while(true)
    {
        Camera >> ImgCapture;
        CameraCapture_Mutex.lock();
        if(!Img.empty())
        {
            Img.pop();
        }
        Img.push(ImgCapture);
        CameraCapture_Mutex.unlock();
    }
}


// 图像获取
Mat SwanCamera::Get()
{
    Mat ImgReturn;
    while(Img.empty());
    CameraCapture_Mutex.lock();
    ImgReturn = Img.front().clone();
    // Img.pop();
    CameraCapture_Mutex.unlock();
    return ImgReturn;
}
