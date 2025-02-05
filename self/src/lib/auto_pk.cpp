#include "common.h"

using namespace std;
using namespace cv;
using namespace swan;


// 棋盘初始化
void FxxkHuman::Init()
{
	for(int i = 0;i < 3;i++)
	{
		for(int j = 0;j < 3;j++)
		{
			Num[i][j] = BACKGROUND;
		}
	}
    Count = 0;
}


// 画棋盘
void FxxkHuman::Draw()
{
    for(int i = 0;i < 3;i++)
    {
        cout << "[" << Num[i][0] << "]" << "[" << Num[i][1] << "]" << "[" << Num[i][2] << "]" << endl;
    }
} 


// 胜负结果判断：1：装置获胜 0；玩家获胜 -1；平局
int FxxkHuman::Result()
{
    int result = -1;
	if( (Num[0][0] == BLACK && Num[1][0] == BLACK && Num[2][0] == BLACK) ||
        (Num[0][1] == BLACK && Num[1][1] == BLACK && Num[2][1] == BLACK) || 
        (Num[0][2] == BLACK && Num[1][2] == BLACK && Num[2][2] == BLACK) ||
        (Num[0][0] == BLACK && Num[0][1] == BLACK && Num[0][2] == BLACK) ||
        (Num[1][0] == BLACK && Num[1][1] == BLACK && Num[1][2] == BLACK) ||
        (Num[2][0] == BLACK && Num[2][1] == BLACK && Num[2][2] == BLACK) ||
        (Num[0][0] == BLACK && Num[1][1] == BLACK && Num[2][2] == BLACK) ||
        (Num[0][2] == BLACK && Num[1][1] == BLACK && Num[2][0] == BLACK) )
    {
        if(ChessPieceColor == BLACK)
        {
            result = 1;
        }
        if(ChessPieceColor == WHITE)
        {
            result = 0;
        }
    }
    if( (Num[0][0] == WHITE && Num[1][0] == WHITE && Num[2][0] == WHITE) ||
        (Num[0][1] == WHITE && Num[1][1] == WHITE && Num[2][1] == WHITE) || 
        (Num[0][2] == WHITE && Num[1][2] == WHITE && Num[2][2] == WHITE) ||
        (Num[0][0] == WHITE && Num[0][1] == WHITE && Num[0][2] == WHITE) ||
        (Num[1][0] == WHITE && Num[1][1] == WHITE && Num[1][2] == WHITE) ||
        (Num[2][0] == WHITE && Num[2][1] == WHITE && Num[2][2] == WHITE) ||
        (Num[0][0] == WHITE && Num[1][1] == WHITE && Num[2][2] == WHITE) ||
        (Num[0][2] == WHITE && Num[1][1] == WHITE && Num[2][0] == WHITE) )
    {
        if(ChessPieceColor == BLACK)
        {
            result = 0;
        }
        if(ChessPieceColor == WHITE)
        {
            result = 1;
        }
    }
    return result;
} 


// 评估函数,当电脑赢的时候，评估为空格数+1,玩家赢的时候评估为-空格数-1,平局的时候为0 
int FxxkHuman::eval()
{
	int res = 0;
	for(int i = 0;i < 3;i ++)
    {
        for(int j = 0;j < 3;j ++)	
        {
            if(Num[i][j] == BACKGROUND) 
            {
                res ++;
            }
        }
    }
	int flag = Result();
	if(flag == 1) return res + 1;
	else if(flag == 0) return -(res + 1);
	else if(flag == -1) return 0;
}


// MINMAX递归单元：step用来记录层数,奇数层为电脑操作,偶数层为玩家操作 
int FxxkHuman::MinMaxSearch(int &idx,int step) 
{
	
	int val;//极值
	if(step & 1)
    {
        val = -100;//奇数层取极大值
    }  
	else 
    {
        val = 100;//偶数层取极小值
    }

	vector<int> positions;//记录空格
	for(int i = 0;i < 3;i ++)
    {
        for(int j = 0;j < 3;j ++)	
        {
            if(Num[i][j] == BACKGROUND)
            {
                positions.push_back(i * 3 + j); 
            }
        }	 
    }
	
	if(positions.size() == 0) 
    {
        return eval();//平局的情况 
    }

	for(int i = 0;i < positions.size();i ++)
	{
		int x = positions[i];
		Num[x / 3][x % 3] = (step & 1) ? ChessPieceColor : PlayerChessPieceColor;
		int Sonval = MinMaxSearch(x,step + 1);
		Num[x / 3][x % 3] = BACKGROUND;
		
		if(step & 1) 
		{
			if(val < Sonval)
			{
				val = Sonval;
				if(step == 1) idx = positions[i];
			}
		}
		else 
		{
			if(val > Sonval) 
			{
				val = Sonval;
			}
		}
	}
	return val;
}


// 下棋
int FxxkHuman::com_play()
{
	int x;//电脑下的位置
	MinMaxSearch(x,1);
	Num[x / 3][x % 3] = ChessPieceColor;
    return x;
}


int FxxkHuman::PK(int NumInput)
{
    int NumOutput = 0;
    // 装置先手，第一步
    if(ChessPieceColor == BLACK && Count == 0)
    {   
        NumOutput = com_play();
        Draw();
    }
    else // 之后都是玩家先下，装置再下
    {
        Draw();
        cout << NumInput << endl;
        Num[NumInput / 3][NumInput % 3] = PlayerChessPieceColor;
        NumOutput = com_play();
        Draw();
    }  
    int Winer = Result();//看是否有人赢了
    if(Winer >= 0)
    {
        Draw();

        if(Winer) printf("装置获胜\n");
        else printf("玩家获胜\n");
    } 
    Count++;
    return NumOutput;
}