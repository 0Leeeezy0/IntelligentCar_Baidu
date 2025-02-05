#include "common.h"

using namespace std;
using namespace cv;
using namespace swan;

// 所有容器初始化
void allVectorInit(vector<vector<Point>>& allContours,vector<RotatedRect>& outsideRect,vector<RotatedRect>& insideRectUnordered,vector<RotatedRect>& insideRect)
{
    allContours.clear();
    outsideRect.clear();
    insideRectUnordered.clear();
    insideRect.clear();
}


// ROI内所有轮廓矩形筛选获取
void allRectSift(vector<vector<Point>> allContours,vector<Vec4i> Hierarchy,vector<RotatedRect>& outsideRect,vector<RotatedRect>& insideRectUnordered)
{
    SwanImage SwanImage;

    vector<RotatedRect> Rect;
    vector<Point> Center;   // 所有外轮廓的中心点坐标
    // 寻找轮廓区域最小外接矩形
    SwanImage.findContoursRect(allContours,Rect);
    // 寻找棋盘中心点
    SwanImage.findContoursCenter(Rect,Center);
    for(int i = 0;i < Rect.size();i++)
    {
        // 判别是否在ROI区域内
        if(Center[i].x > ROI_X && Center[i].x < ROI_X+ROI_WIDTH)
        {
            if(Center[i].y > ROI_Y && Center[i].y < ROI_Y+ROI_HEIGHT)
            {
                // 外轮廓
                if(Hierarchy[i][3] == -1)
                {
                    outsideRect.push_back(Rect[i]);
                }
                // 内轮廓
                else
                {
                    insideRectUnordered.push_back(Rect[i]);
                }
            }
        }
    }
}


// ROI内所有棋子轮廓矩形筛选获取
void allBlackPieceRectSift(vector<vector<Point>> allContours,vector<Vec4i> Hierarchy,vector<RotatedRect>& BlackPieceRectUnordered)
{
    SwanImage SwanImage;

    vector<RotatedRect> Rect;
    vector<Point> Center;   // 所有外轮廓的中心点坐标
    // 寻找轮廓区域最小外接矩形
    SwanImage.findContoursRect(allContours,Rect);
    // 寻找棋盘中心点
    SwanImage.findContoursCenter(Rect,Center);
    for(int i = 0;i < Rect.size();i++)
    {
        // 判别是否在ROI区域内
        if(Center[i].x > PIECE_ROI_X && Center[i].x < PIECE_ROI_X+PIECE_ROI_WIDTH)
        {
            if(Center[i].y > BLACK_PIECE_ROI_Y && Center[i].y < BLACK_PIECE_ROI_Y+PIECE_ROI_HEIGHT)
            {
                BlackPieceRectUnordered.push_back(Rect[i]);
            }
        }
    }
}
void allWhitePieceRectSift(vector<vector<Point>> allContours,vector<Vec4i> Hierarchy,vector<RotatedRect>& WhitePieceRectUnordered)
{
    SwanImage SwanImage;

    vector<RotatedRect> Rect;
    vector<Point> Center;   // 所有外轮廓的中心点坐标
    // 寻找轮廓区域最小外接矩形
    SwanImage.findContoursRect(allContours,Rect);
    // 寻找棋盘中心点
    SwanImage.findContoursCenter(Rect,Center);
    for(int i = 0;i < Rect.size();i++)
    {
        // 判别是否在ROI区域内
        if(Center[i].x > PIECE_ROI_X && Center[i].x < PIECE_ROI_X+PIECE_ROI_WIDTH)
        {
            if(Center[i].y > WHITE_PIECE_ROI_Y && Center[i].y < WHITE_PIECE_ROI_Y+PIECE_ROI_HEIGHT)
            {
                WhitePieceRectUnordered.push_back(Rect[i]);
            }
        }
    }
}


// 内轮廓矩形排序(1 2 3 -- 4 5 6 -- 7 8 9)(左上角开始)
void insideRectSort(vector<RotatedRect>& outsideRect,vector<RotatedRect>& insideRectUnordered,vector<RotatedRect>& insideRect)
{
    insideRect.clear();
    // 九宫格点序号
    int Num[insideRectUnordered.size()] = {0}; 

    int RevolveDir = 0; // 0：45度 1：逆时针 2：顺时针

    // 判断棋盘旋转方向
    Point2f PointRect[4];
    for(int i = 0;i < outsideRect.size();i++)
    {
        outsideRect[i].points(PointRect);
        if(PointRect[0].x > outsideRect[i].center.x)
        {
            RevolveDir = 1;
        }
        else if(PointRect[0].x < outsideRect[i].center.x)
        {
            RevolveDir = 2;
        }
        else
        {
            RevolveDir = 0;
        }
    }

    // 角度计算
    Point RefVectorPoint;
    float Angle_Rect = 0;   // 右边的夹角
    if (RevolveDir == 0)
    {
        Angle_Rect = 45;
    }
    else if (RevolveDir == 1)
    {
        Angle_Rect = outsideRect[0].angle;
    }
    else if (RevolveDir == 2)
    {
        Angle_Rect = 90 - abs(outsideRect[0].angle);
    }
    if(abs(outsideRect[0].angle < 0.1 && abs(outsideRect[0].angle) > 89.9)
    {
        Angle_Rect = 0;
    }
    // 参考向量
    RefVectorPoint.x = (int)(20*cos(Angle_Rect/(180/PI)));
    RefVectorPoint.y = (int)(20*sin(Angle_Rect/(180/PI)));
    // cout << Angle_Rect  << "  " << RefVectorPoint.x << "  " << RefVectorPoint.y << endl;
    // RefVectorPoint.x = 20;
    // RefVectorPoint.y = 0;

    int Vector_ScalarProduct = 0;  // 内轮廓中心点和外轮廓中心点构成的向量与x正向的向量点乘
    int Vector_OuterProduct = 0;    // 内轮廓中心点和外轮廓中心点构成的向量与x正向的向量叉乘
    double Vector_Module[2] = {0};   // 内轮廓中心点和外轮廓中心点构成的向量与x正向的向量的模
    double AngleVector = 0; // 向量夹角(角度制)
    int Vector_Dir = 0;   // 向量和参考向量的位置关系

    // 寻找中心轮廓中心点
    for(int i = 0;i < insideRectUnordered.size();i++)
    {
        if(insideRectUnordered[i].center.x > outsideRect[0].center.x-20 && insideRectUnordered[i].center.x < outsideRect[0].center.x+20 && insideRectUnordered[i].center.y > outsideRect[0].center.y-20 && insideRectUnordered[i].center.y < outsideRect[0].center.y+20)
        {
            Num[4] = i;
            break;
        }
    }
    for(int j = 0;j < insideRectUnordered.size();j++)
    {
        if(j != Num[4])
        {
            Point VectorPoint;
            VectorPoint.x = insideRectUnordered[j].center.x-outsideRect[0].center.x;
            VectorPoint.y = insideRectUnordered[j].center.y-outsideRect[0].center.y;
            Vector_Module[0] = sqrt(pow(VectorPoint.x,2)+pow(VectorPoint.y,2));
            Vector_Module[1] = 20;
            Vector_ScalarProduct = VectorPoint.x*RefVectorPoint.x+VectorPoint.y*RefVectorPoint.y;
            AngleVector = acos(Vector_ScalarProduct/(Vector_Module[0]*Vector_Module[1]))*(180/PI);
            
            // 计算向量叉乘
            Vector_OuterProduct = VectorPoint.x*RefVectorPoint.y-RefVectorPoint.x*VectorPoint.y;
            if(Vector_OuterProduct > 0)
            {
                Vector_Dir = 1;   // 参考向量右侧
            }
            else
            {
                Vector_Dir = -1;  // 参考向量左侧
            }
            // 1
            if(AngleVector > 30 && AngleVector < 60 && Vector_Dir == 1)   
            {
                Num[0] = j;
            }
            // 2
            else if(AngleVector < 25)    
            {
                Num[1] = j;
            }
            // 3
            else if(AngleVector > 30 && AngleVector < 60 && Vector_Dir == -1)	
            {
                Num[2] = j;
            }
            // 4
            else if(AngleVector > 75 && AngleVector < 105 && Vector_Dir == 1)	
            {
                Num[3] = j;
            }
            // 6
            else if(AngleVector > 75 && AngleVector < 105 && Vector_Dir == -1)	
            {
                Num[5] = j;
            }
            // 7
            else if(AngleVector > 120 && AngleVector < 150 && Vector_Dir == 1)	
            {
                Num[6] = j;
            }
            // 8
            else if(AngleVector > 155)	
            {
                Num[7] = j;
            }
            // 9
            else if(AngleVector > 120 && AngleVector < 150 && Vector_Dir == -1)	
            {
                Num[8] = j;
            }
        }
    }
    
    for(int k = 0;k < insideRectUnordered.size();k++)
    {
        insideRect.push_back(insideRectUnordered[Num[k]]);
    }
}


// 棋盘初始化
void _ChessPiece_::ChessInit()
{
    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 3;j++)
        {
            Num[i][j] = BACKGROUND;
            Num_Last[i][j] = BACKGROUND;
            Num_Mechine[i][j] = BACKGROUND;
        }
    }
}


// 棋盘棋子检测
void _ChessPiece_::ChessPieceRecognition(Mat Img_HSV_Black,Mat Img_HSV_White,vector<RotatedRect> insideRect)
{
    _NumRank_ NumRank;

    for(int i = 0;i < insideRect.size();i++)
    {
        NumRank.Num2Rank(i);
        if(Img_HSV_Black.at<uchar>(insideRect[i].center.y,insideRect[i].center.x) == 255)
        {
            Num[NumRank.Row][NumRank.Col] = BLACK;
        }
        else if(Img_HSV_White.at<uchar>(insideRect[i].center.y,insideRect[i].center.x) == 255)
        {
            Num[NumRank.Row][NumRank.Col] = WHITE;
        }
        else
        {
            Num[NumRank.Row][NumRank.Col] = BACKGROUND;
        }
    }
}


// 玩家作弊检测
bool _ChessPiece_::PlayerRiggedCheck(_ChessPieceColor_ ChessPieceColor)
{
    bool Result = false;
    int NumChess = 0;
    for(int i = 0;i < 3;i++)
    {
        for(int j = 0;j < 3;j++)
        {
            if(Num[i][j] == BACKGROUND)
            {
                NumChess++;
            }
            // 提取出棋子中为装置颜色的棋子
            if(Num[i][j] == ChessPieceColor && Num[i][j] != Num_Mechine[i][j] && NumChess > 1)
            {
                Result = true;
            }
        }
    }
    return Result;
}


// 棋盘终端显示
void _ChessPiece_::ChessPieceShow()
{
    for(int i = 0;i < 3;i++)
    {
        cout << "[" << Num[i][0] << "]" << "[" << Num[i][1] << "]" << "[" << Num[i][02] << "]" << endl;
    }
}


// 索引更新
int _ChessPiece_::NumUpdate(_ChessPieceColor_ ChessPieceColor,bool Rigged)
{
    int NumInput = 0;
    if(Rigged);
    else
    {
        _ChessPieceColor_ PlayerChessPieceColor;
        switch(ChessPieceColor)
        {
            case BLACK:{ PlayerChessPieceColor = WHITE; break; }
            case WHITE:{ PlayerChessPieceColor = BLACK; break; }
        }
        
        for(int i = 0;i < 3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                // 若本次与上次出现不一样的棋子
                if(Num[i][j] != Num_Last[i][j] && Num[i][j] == PlayerChessPieceColor)
                {   
                    NumInput = 3*i+j;
                    cout << "("+to_string(i+1)+","+to_string(j+1)+")   USER CHESS" << endl;
                }
            }
        }
        for(int i = 0;i < 3;i++)
        {
            for(int j = 0;j < 3;j++)
            {
                // 更新索引
                if(Num[i][j] == PlayerChessPieceColor)
                {   
                    Num_Last[i][j] = Num[i][j];
                }
            }
        }
    }

    return NumInput;
}


// 序号转行列
void _NumRank_::Num2Rank(int Num)
{
    Row = floor(Num/3);
    Col = Num-Row*3;
}


// 行列转序号
void _NumRank_::Rank2Num(int row,int col)
{
    Num = 3*row+col;
}


// ROI区域获取
void _ROI_::roiGet(int* _ROI_,int Kind)
{
    switch(Kind)
    {
        case 1:{ for(int i = 0;i < 4;i++){ ROI[i] = _ROI_[i]; } break; }
        case 2:{ for(int i = 0;i < 4;i++){ ROI_BLACK_PIECE[i] = _ROI_[i]; } break; }
        case 3:{ for(int i = 0;i < 4;i++){ ROI_BLACK_PIECE[i] = _ROI_[i]; } break; }
    }
}