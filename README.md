# 工程介绍
本工程是十九届智能车竞赛百度完全模型组的`EDGEBOARD赛事专用版`上位机软件开发
其需要与下位机`TC264`进行串口通信
后续会加入`TC264`的下位机软件

# 工程架构
```
├── build   编译文件
├── CMakeLists.txt  CMake文件
├── compile.sh  自动编译脚本
├── doc 文档文件夹
│   └── structure_of_program.md 工程架构
├── img 图像文件夹
│   ├── ImgAll  存放所存储的图像
│   └── sample.mp4  示例视频
├── include 头文件文件夹
│   ├── common_program.h    工程内头文件汇总头文件
│   ├── common_system.h 调用系统库总头文件
│   ├── libdata_process.h   数据处理头文件
│   ├── libdata_store.h 数据存储头文件
│   ├── libimage_process.h  图像处理头文件
│   ├── libuart_eb.h    工程串口头文件
│   ├── libuart_wz.h    串口库头文件
│   └── path.h
├── init.sh 自动初始化脚本
├── lib 库文件夹
│   ├── libdata_process.cpp 数据处理库
│   ├── libimage_process.cpp    图像处理库
│   ├── libuart_eb.cpp  工程串口库
│   └── libuart_wz.cpp  串口库
├── model   模型存放文件夹
└── src 工程主线库
    ├── main.cpp    主线
    ├── path_across.cpp 十字相关
    ├── path_circle.cpp 圆环相关
    └── path_side_search.cpp    赛道路径、边缘相关
```
# 库接口(API)
## libdata_process 数据处理库
### Judge类
TrackKind_Judge_Difference() 赛道循环类型决策：状态机决策状态函数(边线差值法，适用于中线寻边线)

TrackKind_Judge_Vector() 赛道循环类型决策：状态机决策状态函数(向量法，适用于八邻域寻边线)

ServoDirAngle_Judge() 运动参数计算：
```
1.舵机方向 
2.舵机角度 
3.电机速度 
```
### SYNC类
UartReceive_Bit_To_Change_SYNC() 串口接收`数据位->数据交换区`同步

UartSend_Change_To_Bit_SYNC() 串口发送`数据交换区->数据位`同步

UartReceive_Change_To_Program_SYNC() 串口接收`数据交换区->程序内部参数`同步

UartSend_Program_To_Change_SYNC() 串口发送`程序内部参数->数据交换区`同步
### 无类型
DataPrint() 程序内部参数和运动参数打印
```
<---------------------比赛模式--------------------->
<---------------------调试模式--------------------->
<---------------------程序参数--------------------->
前瞻点：
寻线起始点：
寻线结束点：
边线断点起始点：(必须要使用TrackKind_Judge_Difference())
边线断点结束点：(必须要使用TrackKind_Judge_Difference())
比赛状态：
<-------------------------------------------------->
<---------------------运动参数--------------------->
舵机方向：
舵机角度：
电机速度：
<-------------------------------------------------->
```
## libimage_process 图像处理库
### ImgProcess类
#### 公共
ImgPrepare() 图像预处理
```
图像二值化
图像sobel算子提取边缘
图像锐化
```
ImgCompress() 图像压缩

ImgShow() 图像合成显示并保存

ImgInterrupt() 图像边线断点绘制
#### 内部
ImgSharpen() 图像锐化

ImgForwardLine() 图像前瞻线绘制

ImgSynthesis() 多图像合成于同一画面

ImgSave() 图像存储

ImgSobel() SOBEL算子边缘检测

ImgScharr() SCHARR算子边缘检测

ImgUnpivot() 图像逆透视

ImgReferenceLine() 图像参考线绘制
## libuart_eb工程串口库
##3 Uart类
UartSend() 串口发送

UartReceive() 串口接收
## path_across 十字相关
AcrossTrack() 十字赛道补线
## path_circle 圆环相关
CircleTrack_Step_Judge() 圆环赛道步骤决策

CircleTrack_Step_IN_Prepare() 圆环赛道准备入环补线

CircleTrack_Step_IN() 圆环赛道入环补线
## path_side_search 赛道路径、边缘相关
ImgPathSearch() 使用中线法进项路径线规划

ImgSideSearch() 使用八邻域寻找边线