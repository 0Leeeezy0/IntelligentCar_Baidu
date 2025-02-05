#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <cmath>
#include <thread>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>    
#include <mutex>
#include <string.h>
#include <unistd.h>    
#include <sys/types.h>  
#include <sys/stat.h>   
#include <fcntl.h>      
#include <termios.h>  
#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <opencv2/opencv.hpp>
// #include "onnxruntime_cxx_api.h"

#include "swan/camera.h"
#include "swan/control.h"
#include "swan/uart.h"
#include "swan/image.h"
// #include "swan/model.h"

// 棋盘坐标ROI区域
#define ROI_X 120
#define ROI_Y 70
#define ROI_WIDTH 130
#define ROI_HEIGHT (239-2*ROI_Y)
// 黑白棋子坐标ROI区域
#define PIECE_ROI_X 115
#define BLACK_PIECE_ROI_Y 25
#define WHITE_PIECE_ROI_Y 174
#define PIECE_ROI_WIDTH 140
#define PIECE_ROI_HEIGHT 40

// 帧头
#define HEAD_1 0XA0
#define HEAD_2 0XA1
#define TAIL 0XA2

// 圆周率
#define PI 3.1415926

typedef enum _Step_
{ 
    Nine_Square_Grid_Recognition = 0,   // 九宫格识别阶段
    Black_Chess_Piece_Recognition = 1,    // 黑棋子识别阶段
    White_Chess_Piece_Recognition = 2,    // 白棋子识别阶段
    Chess_Piece_Placement_Prepare = 3,  // 棋子放置前检测阶段
    Black_Chess_Piece_Placement = 4,   // 黑棋子放置阶段
    White_Chess_Piece_Placement = 5,   // 白棋子放置阶段
    Player_Rigged = 6,  // 玩家作弊
    ERROR = 7
}_Step_;

typedef enum _ChessPieceColor_
{   
    WHITE = 0,  // 白棋
    BLACK = 1,   // 黑棋
    BACKGROUND = 2  // 背景 
}_ChessPieceColor_;

typedef enum _ResultPK_
{
    BLACK_WIN = 0,
    WHITE_WIN = 1,
    DRAW = 2
}_ResultPK_;

typedef enum _State_
{
    COMMON_CHESS = 0,
    ERROR_CHESS = 1
}_State_;

#include "point_process.h"
#include "img_process.h"
#include "uart.h"
#include "auto_pk.h"

#endif