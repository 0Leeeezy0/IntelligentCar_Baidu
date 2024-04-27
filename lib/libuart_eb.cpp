#include "common_system.h"
#include "common_program.h"

using namespace std;

/*
	UartSend说明
	串口按照本工程协议发送数据
*/
void Uart::UartSend(UartSendProtocol *UartSendProtocol_p,bool UartEN)
{
	WzSerialPort EB;
	SYNC SYNC;

	if(UartEN == true)
	{
		if(EB.open("/dev/ttyUSB0" , (UartSendProtocol_p -> Baudrate) , 0 , 8 , 1 , 1));
		else if(EB.open("/dev/ttyUSB1" , (UartSendProtocol_p -> Baudrate) , 0 , 8 , 1 , 1));
		else
		{
			cout << "<---------------------串口开启失败--------------------->";
       	 	abort();
		}
		SYNC.UartSend_Change_To_Bit_SYNC(UartSendProtocol_p);
		//发送帧头
		EB.send(&(UartSendProtocol_p -> Head_1),1);
		EB.send(&(UartSendProtocol_p -> Head_2),1);
		//发送数据
		EB.send(&(UartSendProtocol_p -> Data_1),1);
		EB.send(&(UartSendProtocol_p -> Data_2),1);
		EB.send(&(UartSendProtocol_p -> Data_3),1);
		EB.send(&(UartSendProtocol_p -> Data_4),1);
		//发送校验位
		EB.send(&(UartSendProtocol_p -> CRC16),1);
	}
	EB.close();

	//串口使能时才发送
	//方便开发端调试机器视觉
	//注意 此处要加&地址符
}


/*
	UartReceive说明
	串口按照本工程协议接收数据
*/
void Uart::UartReceive(UartReceiveProtocol *UartReceiveProtocol_p,bool UartEN)
{
	WzSerialPort EB;
	SYNC SYNC;
	unsigned char UartBuff;
	unsigned int UartReceiveCount = 0;

	if(UartEN == true)
	{
		if(EB.open("/dev/ttyUSB0", (UartReceiveProtocol_p -> Baudrate) , 0 , 8 , 1, 2));
		else if(EB.open("/dev/ttyUSB1", (UartReceiveProtocol_p -> Baudrate) , 0 , 8 , 1, 2));
		else
		{
			cout << "<---------------------串口开启失败--------------------->";
			abort();
		}
		while(UartReceiveCount <= 8)
		{
			switch(UartReceiveCount)
			{
				// 帧首校验
				case 0:
				{
					while(EB.receive(&UartBuff , 1) == 0);
					if(UartBuff == 0xA0)
					{
						UartReceiveCount++;
						// cout << "HEAD_1" << endl;
					}
					break;
				}
				case 1:
				{
					while(EB.receive(&UartBuff , 1) == 0);
					while(UartBuff == 0);
					if(UartBuff == 0xA1)
					{
						UartReceiveCount++;
						// cout << "HEAD_2" << endl;
					}
					break;
				}
				//数据位接收
				case 2:{ while(EB.receive(&UartBuff,1) == 0); UartReceiveProtocol_p -> Data_1 = UartBuff; SYNC.UartReceive_Bit_To_Change_SYNC(UartReceiveProtocol_p); UartReceiveCount++; break; }
				case 3:{ while(EB.receive(&UartBuff,1) == 0); UartReceiveProtocol_p -> Data_2 = UartBuff; SYNC.UartReceive_Bit_To_Change_SYNC(UartReceiveProtocol_p); UartReceiveCount++; break; }
				case 4:{ while(EB.receive(&UartBuff,1) == 0); UartReceiveProtocol_p -> Data_3 = UartBuff; SYNC.UartReceive_Bit_To_Change_SYNC(UartReceiveProtocol_p); UartReceiveCount++; break; }
				case 5:{ while(EB.receive(&UartBuff,1) == 0); UartReceiveProtocol_p -> Data_4 = UartBuff; SYNC.UartReceive_Bit_To_Change_SYNC(UartReceiveProtocol_p); UartReceiveCount++; break; }
				case 6:{ while(EB.receive(&UartBuff,1) == 0); UartReceiveProtocol_p -> Data_5 = UartBuff; SYNC.UartReceive_Bit_To_Change_SYNC(UartReceiveProtocol_p); UartReceiveCount++; break; }
				case 7:{ while(EB.receive(&UartBuff,1) == 0); UartReceiveProtocol_p -> Data_6 = UartBuff; SYNC.UartReceive_Bit_To_Change_SYNC(UartReceiveProtocol_p); UartReceiveCount++; break; }
				case 8:{ while(EB.receive(&UartBuff,1) == 0); UartReceiveProtocol_p -> Data_7 = UartBuff; SYNC.UartReceive_Bit_To_Change_SYNC(UartReceiveProtocol_p); UartReceiveCount++; break; }
				//校验位校验
				case 9:
				{ 
					while(EB.receive(&UartBuff,1) == 0);
					if(UartBuff == 0xA2)
					{
						UartReceiveCount++; 
						// cout << "CRC16" << endl;
					}
					break;
				}
			}
		}	
	}
    else
    {
		UartReceiveProtocol_p -> Forward = Forward_Default;
        UartReceiveProtocol_p -> Path_Search_Start = Path_Search_Start_Default;
        UartReceiveProtocol_p -> Path_Search_End = Path_Search_End_Default;
		UartReceiveProtocol_p -> Side_Search_Start = Side_Search_Start_Default;
        UartReceiveProtocol_p -> Side_Search_End = Side_Search_End_Default;
		UartReceiveProtocol_p -> Gyroscope_EN = false;
        UartReceiveProtocol_p -> Game_EN = Game_EN_Default;
    }
	EB.close();
}