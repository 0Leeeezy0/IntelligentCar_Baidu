#include "common_program.h"
#include "common_system.h"

using namespace std;
using namespace Ort;
using namespace cv;

/*
    ONNX_Model_Init说明
    初始化ONNX模型
*/
void ONNX_Model_Init(const char* ONNX_Model_Dir)
{
    Env env(ORT_LOGGING_LEVEL_WARNING,"ONNX_MODEL_OF_INTELLIGENT_CAR");
    SessionOptions Session_Options;
    Session session(env,ONNX_Model_Dir,Session_Options);
    size_t num_input_nodes = session.GetInputCount();
    size_t num_output_nodes = session.GetOutputCount();
}


/*
    ONNX_Model_Information说明
    模型信息获取
*/
void ONNX_Model_Information(const char* ONNX_Model_Dir)
{
    // 模型初始化
    Env env(ORT_LOGGING_LEVEL_WARNING,"ONNX_MODEL_OF_INTELLIGENT_CAR");
    SessionOptions Session_Options;
    Session session(env,ONNX_Model_Dir,Session_Options);
    size_t num_input_nodes = session.GetInputCount();
    size_t num_output_nodes = session.GetOutputCount();

    // 获取模型信息
    ModelMetadata Model_Metadata = session.GetModelMetadata();
    // cout << "模型信息：" << Model_Metadata << endl;
}