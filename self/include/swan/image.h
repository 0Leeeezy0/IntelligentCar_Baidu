#ifndef _IMAGE_H_
#define _IMAGE_H_

namespace swan   // Swan
{
    // 图像格式
    typedef enum _ImgFormat_
    {
        JPG = 0,
        JPEG = 1,
        PNG = 2
    }_ImgFormat_;

    // 图像通道
    typedef enum _BGR_Channel_
    {
        B_Channel = 0,  // B通道
        G_Channel = 1,  // G通道
        R_Channel = 2 // R通道
    }_BGR_Channel_;

    // 对焦核尺寸
    typedef enum _Focus_Core_Size_
    {
        FocusCore_M3 = 3,
        FocusCore_M5 = 5,
        FocusCore_M7 = 7
    }_Focus_Core_Size_;


    class SwanImage
    {
        public:
            
            /*
                图像色彩空间转化
                @参数说明
                Img 传入待转化的图像
                ColorConversionCode 转化方式(例如：COLOR_BGR2HSV等)
            */
            void colorConvert(cv::Mat& Img,cv::ColorConversionCodes ColorConversionCode);

            /*
                SOBEL算子边缘检测
                @参数说明
                Img 引用待进行边缘检测的图像
                sobelCore SOBEL边缘检测核大小
            */
            void Sobel(cv::Mat& Img,int sobelCore);

            /*
                SCHARR算子边缘检测
                @参数说明
                Img 引用待进行边缘检测的图像
                scharrCore SCHARR边缘检测核大小
            */
            void Scharr(cv::Mat& Img,int scharrCore);

            /*
                CANNY算子边缘检测
                @参数说明
                Img 引用待进行边缘检测的图像
            */
            void Canny(cv::Mat& Img,int Low,int High,int Size);

            /*
                高斯模糊
                @参数说明
                Img 引用待模糊图像
                BlurSize 高斯模糊卷积核大小
            */
            void GaussBlur(cv::Mat &Img,int BlurSize = 3);

            /*
                图像锐化
                @参数说明
                Img 引用待锐化图像
                BlurSize 高斯模糊卷积核大小
            */
            void Sharpen(cv::Mat &Img,int BlurSize = 5);

            /*
                图形学膨胀
            */
            void Dilate(cv::Mat& Img,int CoreSize);

            /*
                图形学腐蚀
            */
            void Erode(cv::Mat& Img,int CoreSize);

            /*
                透视变换
                @参数说明
                Img 引用待逆透视图像
                SrcPoints 原图像四个点
                DstPoints 透视变换图像对应的四个点
                @示例
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
            */
            void Unpivot(cv::Mat& Img,cv::Point2f *SrcPoints,cv::Point2f *DstPoints);

            /*
                图像读取
                @参数说明
                ImgRead_Path 图像读取路径
            */
            cv::Mat Read(const std::string ImgSave_Path);

            /*
                图像存储
                @参数说明
                Img 传入待存储图像
                ImgSave_Path 图像存储路径
                ImgFormat 图像存储格式
            */
            void Save(cv::Mat Img,const std::string ImgSave_Path,int ImgNum,_ImgFormat_ ImgFormat);

            /*
                轮廓寻找
                @参数说明
                Img 传入待寻找轮廓的图像
                Contours 轮廓点集
                Hierarchy 轮廓索引
                Mode 轮廓检索模式
                Method 轮廓近似方式
            */ 
            void findContours(cv::Mat Img,std::vector<std::vector<cv::Point>>& Contours,std::vector<cv::Vec4i>& Hierarchy,cv::RetrievalModes Mode,cv::ContourApproximationModes Method);

            /*
                轮廓绘制
                @参数说明
                Img 传入待绘制轮廓的图像
                Contours 轮廓点集
                Hierarchy 轮廓索引
                Color 轮廓颜色
            */
            void drawContours(cv::Mat& Img,std::vector<std::vector<cv::Point>> Contours,std::vector<cv::Vec4i> Hierarchy,cv::Scalar Color);

            /*
                轮廓区域最小外接矩形类获取
                @参数说明
                Contours 轮廓点集
                Rect 区域最小外接矩形类
            */
           void findContoursRect(std::vector<std::vector<cv::Point>> Contours,std::vector<cv::RotatedRect>& Rect);

            /*
                轮廓中心点寻找
                @参数说明
                Contours 轮廓点集
                Centers 中心点坐标
            */
            void findContoursCenter(std::vector<cv::RotatedRect>& Rect,std::vector<cv::Point>& Centers);

            /*
                轮廓匹配矩形寻找
                @参数说明
                Rect 区域最小外接矩形类
                Centers 中心点坐标
            */
            void drawContoursRect(std::vector<cv::RotatedRect>& Rect);

            /*
                待合成图像添加
                @参数说明
                Img 待合成图像
                @返回值说明
                返回已添加的待合成图像数量
            */
            int Add(cv::Mat Img);

            /*
                合成图像并显示
                @返回值说明
                返回合成图像
            */
            cv::Mat Show();

            /*
                获取合成图像
                @返回值说明
                返回合成图像
            */
           cv::Mat getSynthesisImg();

            /*
                新建画布
                @参数说明
                Img 传入待新建画布的图像
                Width 待新建画布的宽度
                Height 待新建画布的高度
                ImgType 待新建画布的类型(例如：CV_8UC3等)
                Color 待新建画布的颜色
            */
            void newImg(cv::Mat& Img,int Width,int Height,int ImgType,cv::Scalar Color);


            /*
                图像FPS计算显示
                @参数说明
                Img 引用待显示FPS的图像
                X 显示FPS的横向位置
                Y 显示FPS的纵向位置
                FPSperiod FPS计算周期：默认5张图像计算一次周期
            */
            void FPS(cv::Mat& Img,int X,int Y,int FPSperiod = 5);

            class BGR
            {
                public:

                    /*
                        灰度图像二值化
                        @参数说明
                        Img 引用待二值化图像
                    */
                    void thresholdConvert(cv::Mat& Img);

                    /*
                        BGR图像通道分离
                        @参数说明
                        Img 传入需分离的图像
                        BGR_Channel 传入需分离的通道
                    */
                    void channelSplit(cv::Mat& Img,_BGR_Channel_ BGR_Channel);

                    /*
                        峰值对焦
                        反差对焦方法实现
                        @参数说明
                        Img 引用待显示峰值对焦结果的彩色图像
                        FocusCoreSize 对焦核尺寸
                        FocusCoreVarianceLimit 对焦核方差限制
                        @返回值说明
                        返回峰值对焦区域的对焦评估值(0~100)
                    */
                    float focusPeaking(cv::Mat& Img,_Focus_Core_Size_ FocusCoreSize,float FocusCoreVarianceLimit);

                private:

                    // 坐标
                    typedef struct _Point_
                    {
                        int X;
                        int Y;
                    }_Point_;
            };

            class HSV
            {
                public:

                    /*
                        HSV图像亮度直方图均衡化
                        对HSV图像的V(亮度)通道进行均衡化，使其亮度不会过于集中导致阈值难以调整
                        @参数说明
                        传入待均衡化调整的图像
                    */
                    void Equalize(cv::Mat& Img);

                    /*
                        HSV图像二值化
                        @参数说明
                        Img 传入需要二值化的图像
                        LowLimit 阈值下限
                        HighLimit 阈值上限
                        @注意
                        此处的Scalar类型传入的时H、S、V三通道阈值语GBR色彩空间图像不同
                        Scalar( , , )
                    */
                    void thresholdConvert(cv::Mat& Img,cv::Scalar LowLimit,cv::Scalar HighLimit);
            };

            
            
        private:

            // 待合成图像容器
            std::vector<cv::Mat> ImgSynthesis;

            // 坐标
            typedef struct _Point_
            {
                int X;
                int Y;
            }_Point_;
    };
};

#endif