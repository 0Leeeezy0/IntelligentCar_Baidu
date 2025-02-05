#ifndef _IMG_PROCESS_H_
#define _IMG_PROCESS_H_

/*
    图像预处理
*/
void imgProcess(cv::Mat& Img,cv::Mat& Img_LDC,cv::Mat& Img_Gray,cv::Mat& Img_OTSU,cv::Mat& Img_HSV_OTSU_Black,cv::Mat& Img_HSV_OTSU_White);

/*
    图像绘制
*/
void drawImg(cv::Mat& Img,cv::Mat& Img_Nano,std::vector<cv::RotatedRect> outsideRect,std::vector<cv::RotatedRect> insideRect,_Step_ Step);

#endif
