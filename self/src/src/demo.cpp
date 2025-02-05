#include "common.h"

using namespace std;
using namespace cv;
// using namespace Ort;
using namespace swan;

int main()
{
    // 类定义
    Mat ImgColor;
    Mat ImgGray;
    Mat ImgOTSU;
    SwanCamera SwanCamera;
    SwanImage SwanImage;
    SwanImage::BGR SwanImageBGR;
    SwanImage::HSV SwanImageHSV;
    SwanControl SwanControl;
    SwanUart SwanUart;
    _ChessPiece_ ChessPiece;
    _NumRank_ NumRank;
    _ROI_ allROI;
    FxxkHuman FxxkHuman;
    // SwanModel SwanModel;

    SwanUart.initInfosGet("/dev/ttyUSB0",B_115200,ARM_64);
    SwanUart.Init();
    SwanCamera.initInfosGet("/dev/video0",V4L2,FPS_60,EXP_8,false,MJPG,WIDTH_320,HEIGHT_240,true);
    SwanCamera.Init();
    SwanCamera.Capture();

    int ROI[4] = {ROI_X,ROI_Y,ROI_WIDTH,ROI_HEIGHT};
    int ROI_Black[4] = {PIECE_ROI_X,BLACK_PIECE_ROI_Y,PIECE_ROI_WIDTH,PIECE_ROI_HEIGHT};
    int ROI_White[4] = {PIECE_ROI_X,WHITE_PIECE_ROI_Y,PIECE_ROI_WIDTH,PIECE_ROI_HEIGHT};
    allROI.roiGet(ROI,1);
    allROI.roiGet(ROI_Black,1);
    allROI.roiGet(ROI_White,1);

    Mat Img;
    Mat Img_LDC;
    Mat Img_Nano;
    Mat Img_Gray;
    Mat Img_OTSU;
    Mat Img_HSV_OTSU_Black;
    Mat Img_HSV_OTSU_White;
    Mat ImgSynthesis;

    vector<RotatedRect> outsideRect;   // 外部轮廓矩形
    vector<RotatedRect> insideRect;    // 内部轮廓矩形
    vector<Vec4i> Hierarchy;    // 轮廓索引
    vector<Vec4i> Hierarchy_Black;    // 轮廓索引
    vector<Vec4i> Hierarchy_White;    // 轮廓索引
    _ChessPieceColor_ ChessPieceColor = BACKGROUND;

    _Step_ Step = Nine_Square_Grid_Recognition;

    // 图像序号
    int Num = 0;

    while(true)
    {
        // Img = SwanCamera.Get();
        vector<vector<Point>> allContours;  // 所有轮廓
        vector<RotatedRect> insideRectUnordered; // 内部轮廓矩形无序
        vector<vector<Point>> allWhitePieceContours;  // 所有白色棋子轮廓
        vector<vector<Point>> allBlackPieceContours;  // 所有黑色色棋子轮廓
        vector<RotatedRect> WhitePieceRectUnordered; // 白色棋子轮廓矩形无序
        vector<RotatedRect> WhitePieceRect; // 白色棋子轮廓矩形
        vector<RotatedRect> BlackPieceRectUnordered; // 黑色棋子轮廓矩形无序
        vector<RotatedRect> BlackPieceRect; // 黑色棋子轮廓矩形

        Step = getInfo(SwanUart,ChessPieceColor);
        // cout << Num++ << endl;
        // cin >> Step_int;
        // Step = _Step_(Step_int);
        // 图像获取
        Img = SwanCamera.Get();
        // 图像存储
        SwanImage.Save(Img,"/home/edgeboard/Desktop/SWAN/tool/HSV_tool/img",1,JPG);
        // // 图像预处理
        imgProcess(Img,Img_LDC,Img_Gray,Img_OTSU,Img_HSV_OTSU_Black,Img_HSV_OTSU_White);
        // 新建画布
        SwanImage.newImg(Img_Nano,320,240,CV_8UC3,Scalar(50,200,50));

        switch(Step)
        {
            // 九宫格识别阶段
            case Nine_Square_Grid_Recognition:
            {
                cout << "Mode: 0" << endl;
                FxxkHuman.Init();
                ChessPiece.ChessInit();
                outsideRect.clear();
                // 棋盘轮廓寻找
                SwanImage.findContours(Img_OTSU,allContours,Hierarchy,RETR_CCOMP,CHAIN_APPROX_NONE);

                // ROI内所有轮廓外接矩形筛选
                allRectSift(allContours,Hierarchy,outsideRect,insideRectUnordered);

                // 内轮廓所有轮廓矩形排序
                insideRectSort(outsideRect,insideRectUnordered,insideRect);

                drawImg(Img,Img_Nano,outsideRect,insideRect,Step);

                sendInfo(SwanUart,insideRect);
                
                break;
            }
            // 黑色棋子识别阶段
            case Black_Chess_Piece_Recognition:
            {
                cout << "Mode: 1" << endl;
                FxxkHuman.Init();
                ChessPiece.ChessInit();
                // 黑色棋子轮廓寻找
                SwanImage.findContours(Img_HSV_OTSU_Black,allBlackPieceContours,Hierarchy_Black,RETR_CCOMP,CHAIN_APPROX_NONE);
                
                // ROI内所有黑色棋子外接矩形筛选
                allBlackPieceRectSift(allBlackPieceContours,Hierarchy_Black,BlackPieceRectUnordered);
                
                drawImg(Img,Img_Nano,outsideRect,insideRect,Step);
                
                // 棋子绘制
                for(int i = 0;i < BlackPieceRectUnordered.size();i++)
                {
                    circle(Img,BlackPieceRectUnordered[i].center,10,Scalar(0,0,0),5);  // 黑色
                    circle(Img_Nano,BlackPieceRectUnordered[i].center,10,Scalar(0,0,0),5);  //黑色
                }

                sendInfo(SwanUart,BlackPieceRectUnordered);

                break;
            }
            // 白色棋子识别阶段
            case White_Chess_Piece_Recognition:
            {
                cout << "Mode: 2" << endl;
                FxxkHuman.Init();
                ChessPiece.ChessInit();
                // 白色棋子轮廓寻找
                SwanImage.findContours(Img_HSV_OTSU_White,allWhitePieceContours,Hierarchy_White,RETR_CCOMP,CHAIN_APPROX_NONE);

                // ROI内所有白色棋子外接矩形筛选
                allWhitePieceRectSift(allWhitePieceContours,Hierarchy_White,WhitePieceRectUnordered);

                drawImg(Img,Img_Nano,outsideRect,insideRect,Step);

                for(int i = 0;i < WhitePieceRectUnordered.size();i++)
                {
                    circle(Img,WhitePieceRectUnordered[i].center,10,Scalar(255,255,255),5);  // 白色
                    circle(Img_Nano,WhitePieceRectUnordered[i].center,10,Scalar(255,255,255),5);  // 白色
                }

                sendInfo(SwanUart,WhitePieceRectUnordered);

                break;
            }
            // 棋子放置前检测阶段
            case Chess_Piece_Placement_Prepare:
            {
                cout << "Mode: 3" << endl;
                _Step_ StepNext = ERROR;
                FxxkHuman.ChessPieceColor = ChessPieceColor;
                switch(ChessPieceColor)
                {
                    case BLACK:{ FxxkHuman.PlayerChessPieceColor = WHITE; break; }
                    case WHITE:{ FxxkHuman.PlayerChessPieceColor = BLACK; break; }
                }
                // 棋子识别
                ChessPiece.ChessPieceRecognition(Img_HSV_OTSU_Black,Img_HSV_OTSU_White,insideRect);
                // 棋子绘制
                for(int i = 0;i < insideRect.size();i++)
                {
                    NumRank.Num2Rank(i);
                    if(ChessPiece.Num[NumRank.Row][NumRank.Col] == WHITE)
                    {
                        cout << "("+to_string(NumRank.Row+1)+","+to_string(NumRank.Col+1)+")   WHITE" << endl;
                        circle(Img,insideRect[i].center,10,Scalar(255,255,255),5);  // 白色
                        circle(Img_Nano,insideRect[i].center,10,Scalar(255,2550,255),5);  //白色
                    }
                    if(ChessPiece.Num[NumRank.Row][NumRank.Col] == BLACK)
                    {
                        cout << "("+to_string(NumRank.Row+1)+","+to_string(NumRank.Col+1)+")   BLACK" << endl;
                        circle(Img,insideRect[i].center,10,Scalar(0,0,0),5);  // 黑色
                        circle(Img_Nano,insideRect[i].center,10,Scalar(0,0,0),5);  //黑色
                    }
                }
                // 玩家作弊检测
                bool Rigged = ChessPiece.PlayerRiggedCheck(ChessPieceColor);
                // 上一次检索更新并返回更新点序号
                int NumInput = ChessPiece.NumUpdate(ChessPieceColor,Rigged);
                drawImg(Img,Img_Nano,outsideRect,insideRect,Step);
                switch(ChessPieceColor)
                {
                    case BLACK:{ StepNext = Black_Chess_Piece_Placement; break; }
                    case WHITE:{ StepNext = White_Chess_Piece_Placement; break; }
                }
                if(Rigged == true)
                {
                    StepNext = ERROR;
                }
                unsigned char COMMON = 0xff;
                switch(StepNext)
                {
                    // 黑棋子放置阶段
                    case Black_Chess_Piece_Placement:
                    {
                        cout << "Mode: 3 Black" << endl;
                        cout << "装置先行" << endl;
                        cout << NumInput << endl;
                        int NumOutput = FxxkHuman.PK(NumInput);
                        cout << NumOutput << endl;
                        NumRank.Num2Rank(NumOutput);
                        ChessPiece.Num_Mechine[NumRank.Row][NumRank.Col] = ChessPieceColor;
                        sendInfo(SwanUart,(unsigned char)NumOutput,COMMON);

                        break;
                    }
                    // 白棋子放置阶段
                    case White_Chess_Piece_Placement:
                    {
                        cout << "Mode: 3 White" << endl;
                        cout << "玩家先行" << endl;
                        cout << NumInput << endl;
                        int NumOutput = FxxkHuman.PK(NumInput);
                        cout << NumOutput << endl;
                        NumRank.Num2Rank(NumOutput);
                        ChessPiece.Num_Mechine[NumRank.Row][NumRank.Col] = ChessPieceColor;
                        sendInfo(SwanUart,(unsigned char)NumOutput,COMMON);

                        break;
                    }
                    case ERROR:
                    {
                        cout << "Mode: 3 Error" << endl;
                        cout << "狗日的，爱作弊是吧，因为这个我决策写死了，我测" << endl;
                        int NumOld = 0;
                        int NumNew = 0;

                        for(int i = 0;i < 3;i++)
                        {
                            for(int j = 0;j < 3;j++)
                            {
                                if(ChessPiece.Num[i][j] != ChessPieceColor && ChessPiece.Num_Mechine[i][j] == ChessPieceColor)
                                {
                                    NumOld = 3*i+j;
                                }
                                if(ChessPiece.Num[i][j] == ChessPieceColor && ChessPiece.Num_Mechine[i][j] != ChessPieceColor)
                                {
                                    NumNew = 3*i+j;
                                }
                            }
                        }
                        sendInfo(SwanUart,NumOld,NumNew);

                        break;
                    }
                }
            }
        }

        SwanImage.Add(Img_LDC);
        SwanImage.Add(Img_Gray);
        SwanImage.Add(Img_OTSU);
        SwanImage.Add(Img_HSV_OTSU_Black);
        SwanImage.Add(Img_HSV_OTSU_White);
        SwanImage.Add(Img_Nano);
        ImgSynthesis = SwanImage.Show().clone();

        // circle(Img,Point(160,120),4,Scalar(0,0,0),1);  // 黑色
        // circle(Img,Point(120,90),4,Scalar(0,0,0),1);  // 黑色
        // imshow("asd",Img);

        waitKey(1);
    }

    return 0;
}