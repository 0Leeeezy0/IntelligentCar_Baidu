#ifndef _CAMERA_H_
#define _CAMERA_H_

namespace swan   // swan
{
    // 视频流格式
    typedef enum _VideoCurrentFormat_    
    {
        MJPG = 0,
        MPEG = 1
    }_VideoCurrentFormat_;

    // 摄像头调用框架
    typedef enum _CameraFramework_
    {
        V4L2 = 0,
        COMMON = 1
    }_CameraFrame_;

    // 摄像头帧数
    typedef enum _CameraFPS_
    {
        FPS_5 = 5,
        FPS_10 = 10,
        FPS_15 = 15,
        FPS_20 = 20,
        FPS_25 = 25,
        FPS_30 = 30,
        FPS_60 = 60,
        FPS_90 = 90,
        FPS_120 = 120
        // 待拓展
    }_CameraFPS_;

    // 摄像头曝光度
    typedef enum _CameraExposure_
    {
        EXP_1 = -1,
        EXP_2 = -2,
        EXP_3 = -3,
        EXP_4 = -4,
        EXP_5 = -5,
        EXP_6 = -6,
        EXP_7 = -7,
        EXP_8 = -8,
        EXP_9 = -9,
        EXP_10 = -10,
        EXP_11 = -11,
        EXP_12 = -12,
        EXP_13 = -13,
        EXP_14 = -14
    }_CameraExposure_;

    // 摄像头分辨率
    // 横向
    typedef enum _CameraResolutionWidth_
    {
        WIDTH_320 = 320,
        WIDTH_640 = 640
        // 待拓展
    }_CameraResolutionWidth_;
    // 纵向
    typedef enum _CameraResolutionHeight_
    {
        HEIGHT_240 = 240,
        HEIGHT_480 = 480
        // 待拓展
    }_CameraResolutionHeight_;

    class SwanCamera
    {
        public:
            std::queue<cv::Mat> Img;    // 摄像头图像

            /*
                摄像头初始化
                @返回值说明
                返回摄像头初始化结果：-1.初始化失败 1.初始化成功
            */ 
            int Init();

            /*
                摄像头图像捕获
            */
            void Capture();

            /*
                摄像头初始化信息获取
                @参数说明
                CameraDevice 摄像头设备号
                CameraFramework 摄像头调用框架
                CameraFPS 摄像头帧率
                VideoCurrentFormat 视频流格式
                CameraResolutionWidth 摄像头横向分辨率
                CameraResolutionHeight 摄像头纵向分辨率
                CameraMultiThreading 摄像头采集和主程序的关系
            */
           void initInfosGet(std::string CameraDevice = "/dev/video0",_CameraFramework_ CameraFramework = COMMON,_CameraFPS_ CameraFPS = FPS_30,_CameraExposure_ CameraExposure = EXP_6,bool CameraAutoWhiteBlance = false,_VideoCurrentFormat_  VideoCurrentFormat = MJPG,_CameraResolutionWidth_ CameraResolutionWidth = WIDTH_320,_CameraResolutionHeight_ CameraResolutionHeight = HEIGHT_240,bool CameraMultiThreading = false);
           

            /*
                摄像头捕获图像获取
            */
           cv::Mat Get();
        private:

            // 摄像头初始化信息
            typedef struct _CameraInitParameter_
            {
                std::string CameraDevice; // 摄像头设备号
                _CameraFramework_ CameraFramework;   // 摄像头调用框架
                _CameraFPS_ CameraFPS;  // 摄像头帧数
                _CameraExposure_ CameraExposure; // 摄像头曝光度
                bool CameraAutoWhiteBalance;    // 摄像头自动白平衡
                _VideoCurrentFormat_  VideoCurrentFormat;    // 视频流格式
                _CameraResolutionWidth_ CameraResolutionWidth; // 摄像头横向分辨率
                _CameraResolutionHeight_ CameraResolutionHeight; // 摄像头纵向分辨率
                bool CameraMultiThreading;    // 多线程采集摄像头图像
            }_CameraInitParameter_;

            _CameraInitParameter_ CameraInitParameter;

            /*
                多线程摄像头图像捕获
                @参数说明
                Camera 引用VideoCapture类
            */
            void captureThread(cv::VideoCapture &Camera);

            cv::VideoCapture Camera; 
    };
}

#endif