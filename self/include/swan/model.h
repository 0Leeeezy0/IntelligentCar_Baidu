#ifndef _LIBMODEL_H_
#define _LIBMODEL_H_

namespace swan // Swan
{
    class SwanModel
    {
        public:
            typedef enum Lable
            {
                Cat = 0,
                Dog = 1
            }Lable;

            typedef struct PredictResult
            {
                swan::SwanModel::Lable PredictLable;   // 模型预测结果标签
                int CenterCoorCoordinate[2] = {0};  // 模型预测结果目标中心坐标(x,y)
                int DrawBoxSize[4] = {0};   // 模型预测结果目标画框尺寸
                int Num;    // 模型预测次数
            }PredictResult;

            std::vector<PredictResult> PredictResults;

            // 模型预测结果保存
            void PredictResultSave(std::vector<PredictResult>& PredictResults,PredictResult* PredictResult);

        private:

            
    };
};

using Lable = swan::SwanModel::Lable;
using PredictResult = swan::SwanModel::PredictResult;

#endif