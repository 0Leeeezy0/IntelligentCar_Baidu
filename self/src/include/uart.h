#ifndef _USR_UART_H_
#define _USR_UART_H_


// 串口接收
_Step_ getInfo(swan::SwanUart SwanUart,_ChessPieceColor_& First_Chess_Piece_Color);


// 串口发送
void sendInfo(swan::SwanUart SwanUart,std::vector<cv::RotatedRect> Rect);
void sendInfo(swan::SwanUart SwanUart,unsigned char NumOld,unsigned char NumNew);

#endif
