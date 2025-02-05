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
    PPNC_ModelDetection_Thread说明
    PPNC 多线程模型预测
    在模型预测结束后会对模型推理使能
    若模型预测未结束则不对模型推理使能
*/
void PPNC_ModelDetection_Thread(PPNCDetection& PPNCDetection,Img_Store *Img_Store_p,Function_EN *Function_EN_p,bool& JSON_ModelDetection_EN)
{
    unordered_map<string, NDTensor> Run_Tensor;   // 模型推理所需张量
    vector<int64_t> InputSize = {320,320};  // 设置图像输入模型的尺寸，即张量尺寸
    if(JSON_ModelDetection_EN == true)
    {
        while(1)
        {
            while(Function_EN_p -> ThreadModelDetection_EN == false)
            {
                auto Run_Tensor = PPNCDetection.preprocess(Img_Store_p -> Img_Color,InputSize);
                PPNCDetection.run(*Run_Tensor); // 模型输入图像预处理
                PPNCDetection.render(); // 模型推理
                PPNCDetection.drawBox(Img_Store_p -> Img_Color); // 识别结果画框
                Function_EN_p -> ThreadModelDetection_EN = true;
            }
        }
    }
}

