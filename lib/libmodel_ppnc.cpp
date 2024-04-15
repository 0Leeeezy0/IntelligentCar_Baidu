#include "common_system.h"
#include "common_program.h"

using namespace std;
using namespace cv;


/*
    PPNC_ModelInit说明
    初始化模型
*/
void PPNC_ModelInit(PPNCDetection& PPNCDetection,Function_EN *Function_EN_p,bool JSON_ModelDetection_EN)
{
    if(JSON_ModelDetection_EN == true)
    {
        PPNCDetection.init("../model");
        Function_EN_p -> ThreadModelDetection_EN = true;
    }
}


/*
    PPNC_ModelTensorTrans说明
    PPNC Mat转化为Tensor
*/
void PPNC_ModelTensorTrans(PPNCDetection& PPNCDetection,Mat Img,bool JSON_ModelDetection_EN)
{
    NDTensor Tensor;
    unordered_map<string, NDTensor> Run_Tensor;   // 设置传入run()中的参数
    vector<int64_t> InputSize = {320,320};  // 设置图像输入模型的尺寸，即张量尺寸
    if(JSON_ModelDetection_EN == true)
    {
        auto Run_Tensor = PPNCDetection.preprocess(Img,InputSize);
        PPNCDetection.run(*Run_Tensor); // 模型输入图像预处理
    }
}


/*
    PPNC_ModelDetection_Thread说明
    PPNC 多线程模型预测
    在模型预测结束后会对模型推理使能
    若模型预测未结束则不对模型推理使能
*/
void PPNC_ModelDetection_Thread(PPNCDetection& PPNCDetection,Function_EN *Function_EN_p,Mat& Img,bool& JSON_ModelDetection_EN)
{
    if(JSON_ModelDetection_EN == true)
    {
        Function_EN_p -> ThreadModelDetection_EN = false;
        PPNCDetection.render(); // 模型推理
        PPNCDetection.drawBox(Img); // 识别结果画框
        if(PPNCDetection.results.size() != 0)
        {
            Function_EN_p -> ThreadModelDetection_EN = true;
        }
    }
}

