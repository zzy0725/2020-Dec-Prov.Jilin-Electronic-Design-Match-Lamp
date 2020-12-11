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
#define LAO 10      	   //L ���� H  ����	 
#define LCS 8	           //Ƭѡ ����Ч
#define LSCK 13		   //ʱ��     use SPI must connection to PIN 13
#define LSID 11		   //�����ź� use SPI must connection to PIN 11
#define LRST 9             //��λ����Һ����λ


#define  Display  0xAF     //��ʾ����
#define  Power    0x2F	   //��Դȫ��
#define  VO       0x24     //�Աȶȵ���	     
#define  AllPoint 0xA4	   //��ȫ����ʾ
#define  ADCset   0xA1    	//����131-4
#define  COMset   0xc0    	//com0-com63
#define  ELECTVO  0x81     //���ȵ���	 ������ɫ 	��ͬ��ģ����Ҫ��ͬ��  ELECTSET
#define  ELECTSET 0x2a	   //������ֵ	 ������ɫ 	��ͬ��ģ����Ҫ��ͬ��	��ֵ
#define  BIASSET  0xA2    //ռ�ձ�1/9

class lcd12864
{
   public:
    lcd12864();  //���캯�� 
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