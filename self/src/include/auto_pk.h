#ifndef _AUTO_PK_H_
#define _AUTO_PK_H_

class FxxkHuman
{
    public:
        _ChessPieceColor_ Num[3][3] = {BACKGROUND};
        _ChessPieceColor_ ChessPieceColor;
        _ChessPieceColor_ PlayerChessPieceColor;
        
        int Count = 0;

        // 棋盘初始化
        void Init();

        // pk
        int PK(int NumInput);

    private:
        // 画棋盘
        void Draw();

        // 胜负结果判断：1：装置获胜 0；玩家获胜 -1；平局
        int Result();

        // 评估函数,当电脑赢的时候，评估为空格数+1,玩家赢的时候评估为-空格数-1,平局的时候为0 
        int eval();

        // MINMAX递归单元：step用来记录层数,奇数层为电脑操作,偶数层为玩家操作 
        int MinMaxSearch(int &idx,int step);

        // 下棋
        int com_play();
};

#endif