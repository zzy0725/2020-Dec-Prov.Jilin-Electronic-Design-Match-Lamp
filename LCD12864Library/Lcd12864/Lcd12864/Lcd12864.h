/*
 * mini12864LCD 
 * August , 2012
 * created by haishen   
 * for details, see http://www.arduino.cn
*/

#ifndef LCD12864_H_
#define LCD12864_H_

#if LCD_SPI==1
// inslude the SPI library:
#include <SPI.h>
#endif

#define LCD_SPI 1          //use SPI 1:use SPI 0;IO simulate
#define LCD_TEST 1         //LCD_TEST 1:to test 12864. 0 no test
#define LAO 10      	   //L 命令 H  数据	 
#define LCS 8	           //片选 低有效
#define LSCK 13		   //时钟     use SPI must connection to PIN 13
#define LSID 11		   //数据信号 use SPI must connection to PIN 11
#define LRST 9             //复位，给液晶复位


#define  Display  0xAF     //显示开启
#define  Power    0x2F	   //电源全开
#define  VO       0x24     //对比度调节	     
#define  AllPoint 0xA4	   //非全屏显示
#define  ADCset   0xA1    	//负向，131-4
#define  COMset   0xc0    	//com0-com63
#define  ELECTVO  0x81     //亮度调节	 调节颜色 	不同的模块需要不同的  ELECTSET
#define  ELECTSET 0x2a	   //亮度数值	 调节颜色 	不同的模块需要不同的	数值
#define  BIASSET  0xA2    //占空比1/9

class lcd12864
{
   public:
    lcd12864();  //构造函数 
    void SendByte(unsigned char Dbyte);
    void write_cmd(unsigned char Cbyte);
    void write_data(unsigned char Dbyte);
    void LcmInit();
    void PUTchar8x8(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put);
    void PUTchar8x16(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put);
    void PUTchar16x16(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put);
    void PUTchar24x24(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put);
    void PUTBMP(unsigned char const *put);
    void PUTREVERSEBMP(unsigned char const *put);
    void LcmClear();
};
#endif