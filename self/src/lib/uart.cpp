#include "common.h"

using namespace std;
using namespace cv;
using namespace swan;


// 串口接收，模式选择
_Step_ getInfo(SwanUart SwanUart,_ChessPieceColor_& ChessPieceColor)
{
    unsigned char Data;
    int Num = 0;
    _Step_ Step = ERROR;
    while(Num <= 3)
    {
        switch(Num)
        {
            case 0:{ SwanUart.Receive(Data); if(Data == 0XA0){ Num = 1; }else{ Num = 0; } break; }
            case 1:{ SwanUart.Receive(Data); if(Data == 0XA1){ Num = 2; }else{ Num = 0; } break; }
            case 2:{ SwanUart.Receive(Data); Step = (_Step_)Data; Num = 3; break; }
            case 3:{ SwanUart.Receive(Data); ChessPieceColor = (_ChessPieceColor_)Data; Num = 4; break; }
            case 4:{ SwanUart.Receive(Data); if(Data == 0XA2){ Num = 5; }else{ Step = ERROR; Num = 0; } break;}
        }
    }
    
    return Step;
}


// 串口发送
void sendInfo(SwanUart SwanUart,vector<RotatedRect> Rect)
{
    unsigned char Frame[3] = { HEAD_1 , HEAD_2 , TAIL };
    for(int i = 0;i < Rect.size();i++)
    {
        SwanUart.sendInfosClear();
        SwanUart.sendInfoAdd(Frame[0]);
        SwanUart.sendInfoAdd(Frame[1]);
        SwanUart.sendInfoAdd(i);
        SwanUart.sendInfoAdd((int)(Rect[i].center.x*100));  // x
        SwanUart.sendInfoAdd((int)(Rect[i].center.y*100));  // y
        SwanUart.sendInfoAdd(Frame[2]);
        SwanUart.Send();
    }
}

void sendInfo(SwanUart SwanUart,unsigned char Num)
{
    unsigned char Frame[3] = { HEAD_1 , HEAD_2 , TAIL };
    SwanUart.sendInfosClear();
    SwanUart.sendInfoAdd(Frame[0]);
    SwanUart.sendInfoAdd(Frame[1]);
    SwanUart.sendInfoAdd(Num);
    SwanUart.sendInfoAdd(Frame[2]);
    SwanUart.Send();
}

void sendInfo(SwanUart SwanUart,unsigned char NumOld,unsigned char NumNew)
{
    unsigned char Frame[3] = { HEAD_1 , HEAD_2 , TAIL };
    SwanUart.sendInfosClear();
    SwanUart.sendInfoAdd(Frame[0]);
    SwanUart.sendInfoAdd(Frame[1]);
    SwanUart.sendInfoAdd(NumOld);
    SwanUart.sendInfoAdd(NumNew);
    SwanUart.sendInfoAdd(Frame[2]);
    SwanUart.Send();
}
