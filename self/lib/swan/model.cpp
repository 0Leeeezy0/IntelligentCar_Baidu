#include <iostream>
#include <cmath>
#include <thread>
#include <stdio.h>
#include <stdlib.h>    
#include <string.h>
#include <unistd.h>    
#include <sys/types.h>  
#include <sys/time.h>
#include <sys/stat.h>   
#include <fcntl.h>      
#include <termios.h>  
#include <errno.h>
#include <vector>
#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>

#include "swan/model.h"

using namespace std;
using namespace cv;
using namespace Ort;
using namespace swan;

// 模型预测结果保存
void SwanModel::PredictResultSave(vector<PredictResult>& PredictResults_Save,PredictResult* PredictResult)
{   
    static int SaveRecord = 0;  // 模型预测结果保存记录
    PredictResult -> Num = SaveRecord;
    PredictResults_Save.push_back(*PredictResult);  // 写入
    SaveRecord++;
}