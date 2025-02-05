#ifndef _POINT_PROCESS_H_
#define _POINT_PROCESS_H_

/*
    所有容器初始化
    @参数说明
    allContours 所有轮廓点集
    outsideRect 外轮廓外接矩形集合
    insideRectUnordered 内轮廓外接矩形集合(无序)
    insideRect 内轮廓外接矩形集合
*/
void allVectorInit(std::vector<std::vector<cv::Point>>& allContours,std::vector<cv::RotatedRect>& outsideRect,std::vector<cv::RotatedRect>& insideRectUnordered,std::vector<cv::RotatedRect>& insideRect);


/*
    ROI内所有轮廓矩形筛选
    @参数说明
    allContours 所有轮廓点集
    Hierarchy 轮廓索引
    outsideRect 外轮廓外接矩形集合
    insideRectUnordered 内轮廓外接矩形集合(无序)
*/
void allRectSift(std::vector<std::vector<cv::Point>> allContours,std::vector<cv::Vec4i> Hierarchy,std::vector<cv::RotatedRect>& outsideRect,std::vector<cv::RotatedRect>& insideRectUnordered);


/*
    ROI内所有棋子轮廓矩形筛选
    @参数说明
    allContours 所有轮廓点集
    Hierarchy 轮廓索引
    *PieceRectUnordered 黑白棋子轮廓外接矩形集合(无序)
*/
void allBlackPieceRectSift(std::vector<std::vector<cv::Point>> allContours,std::vector<cv::Vec4i> Hierarchy,std::vector<cv::RotatedRect>& BlackPieceRectUnordered);
void allWhitePieceRectSift(std::vector<std::vector<cv::Point>> allContours,std::vector<cv::Vec4i> Hierarchy,std::vector<cv::RotatedRect>& WhitePieceRectUnordered);


/*
    内轮廓矩形排序(1 2 3 -- 4 5 6 -- 7 8 9)(左上角开始)
    @参数说明
    outsideRect 外轮廓外接矩形集合
    insideRectUnordered 内轮廓外接矩形集合(无序)
    insideRect 内轮廓外接矩形集合
*/
void insideRectSort(std::vector<cv::RotatedRect>& outsideRect,std::vector<cv::RotatedRect>& insideRectUnordered,std::vector<cv::RotatedRect>& insideRect);


class _ChessPiece_
{
    public:
    _ChessPieceColor_ Num_Last[3][3] = {BACKGROUND}; // 棋子前一次索引
    _ChessPieceColor_ Num[3][3] = {BACKGROUND}; // 棋子索引
    _ChessPieceColor_ Num_Mechine[3][3] = {BACKGROUND}; // 装置棋子索引

    // 棋盘初始化
    void ChessInit();

    // 棋盘棋子检测
    void ChessPieceRecognition(cv::Mat Img_HSV_Black,cv::Mat Img_HSV_White,std::vector<cv::RotatedRect> insideRect);

    // 棋子坐标复制
    void ChessPieceCopy();

    // 玩家作弊检测(true：作弊 false：没作弊)
    bool PlayerRiggedCheck(_ChessPieceColor_ ChessPieceColor);

    // 棋盘终端显示
    void ChessPieceShow();

    // 索引更新
    int NumUpdate(_ChessPieceColor_ ChessPieceColor,bool Rigged);
};

class _NumRank_
{
    public:
    // 序号转行列
    void Num2Rank(int Num);
    // 行列转序号
    void Rank2Num(int row,int col);
    int Row;    // 行
    int Col;    // 列
    int Num;    // 序号
};

class _ROI_
{
    public:
    void roiGet(int* _ROI_,int Kind);

    // x、y、width、height
    int ROI[4] = {0};
    int ROI_BLACK_PIECE[4] = {0};
    int ROI_WHITE_PIECE[4] = {0};
};

#endif  