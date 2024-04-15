#include "common_program.h"
#include "common_system.h"

#ifndef _LIBMODEL_H_
#define _LIBMODEL_H_

/*
    初始化ONNX模型
    @参数说明
    ONNX_Model_Dir 传入ONNX模型路径
*/
void ONNX_Model_Init(const char* ONNX_Model_Dir);


/*
    模型信息获取
    @参数说明
    ONNX_Model_Dir 传入ONNX模型路径
*/
void ONNX_Model_Information(const char* ONNX_Model_Dir);


/*
    初始化模型
    PPNCDetection 传入PPNCDetection的类声明
    Function_EN_p 函数使能结构体指针
    JSON_ModelDetection_EN JSON内存储的模型推理使能设置参数
*/
void PPNC_ModelInit(PPNCDetection& PPNCDetection,Function_EN *Function_EN_p,bool JSON_ModelDetection_EN);


/*
    PPNC Mat转化为Tensor
    @参数说明
    PPNCDetection 传入PPNCDetection的类声明
    Img 传入待转化为Tensor的图像
    InputSize 传入输入的图像大小
    Run_Tensor 传入待转化出的张量
    JSON_ModelDetection_EN JSON内存储的模型推理使能设置参数
*/
void PPNC_ModelTensorTrans(PPNCDetection& PPNCDetection,cv::Mat Img,bool JSON_ModelDetection_EN);


/*
    PPNC 多线程模型预测
    在模型预测结束后会对模型推理使能
    若模型预测未结束则不对模型推理使能
    @参数说明
    PPNCDetection 传入PPNCDetection的类声明
    Function_EN_p 函数使能结构体指针
    Img 传入待绘制检测结果框的图像
    JSON_ModelDetection_EN JSON内存储的模型推理使能设置参数
*/
void PPNC_ModelDetection_Thread(PPNCDetection& PPNCDetection,Function_EN *Function_EN_p,cv::Mat& Img,bool& JSON_ModelDetection_EN);

#endif