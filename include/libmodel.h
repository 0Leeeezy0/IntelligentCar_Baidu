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

#endif