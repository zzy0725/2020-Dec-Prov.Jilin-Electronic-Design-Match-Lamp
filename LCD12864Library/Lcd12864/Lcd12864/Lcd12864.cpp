/*
 * mini12864LCD 
 * August , 2012
 * created by haishen   
 * for details, see http://www.arduino.cn
*/


#include <arduino.h>
#include <SPI.h>
#include "Lcd12864.h"
// unsigned char i=0,j=0;
lcd12864::lcd12864()
{
}
/********************
 * ������λ  ���λ���룬
 ********************/
#if LCD_SPI==1
#else
void lcd12864::SendByte(unsigned char Dbyte)
{
  unsigned char ,a,TEMP; 
  TEMP=Dbyte;
  for(int i=0;i<8;i++)
  {
    digitalWrite(LSCK,LOW);
    TEMP=(Dbyte<<i)&0X80;
    digitalWrite(LSID,TEMP);
    digitalWrite(LSCK,HIGH);
  }
}
#endif
/****************
 * дָ��
 **************/
void lcd12864::write_cmd(unsigned char Cbyte)
{
  digitalWrite(LCS,LOW);
  digitalWrite(LAO,LOW);
#if LCD_SPI==1
  SPI.transfer(Cbyte);
#else
  SendByte(Cbyte);
#endif
}
/***************
 * д����
 ******************/
void lcd12864:: write_data(unsigned char Dbyte)
{
  digitalWrite(LCS,LOW);
  digitalWrite(LAO,HIGH);
#if LCD_SPI==1
  SPI.transfer(Dbyte);
#else
  SendByte(Dbyte);
#endif
}

/*******************
 * ��ʼ����
 *********************/
void lcd12864::LcmInit()
{
  //L ���� H  ����	 
  //Ƭѡ	 ����Ч
  //ʹ���źţ�E)	������Ч
  //	�����ź�
  //��λ
  pinMode(LAO,OUTPUT);
  pinMode(LCS,OUTPUT);  
  pinMode(LSCK,OUTPUT);
  pinMode(LSID,OUTPUT);
  pinMode(LRST,OUTPUT);  
#if LCD_SPI==1
  // initialize SPI:
  SPI.begin(); 
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
#endif	
  digitalWrite(LCS,LOW);
  digitalWrite(LRST,LOW);  
  delay(200);
  digitalWrite(LRST,HIGH);
  delay(1000);	
  write_cmd(0xe2);//system reset
  delay(200);

  write_cmd(0x24);//SET VLCD RESISTOR RATIO
  write_cmd(0xa2);//BR=1/9
  write_cmd(0xa0);//set seg direction
  write_cmd(0xc8);//set com direction
  write_cmd(0x2f);//set power control
  write_cmd(0x40);//set scroll line
  write_cmd(0x81);//SET ELECTRONIC VOLUME
  write_cmd(0x20);//set pm: ͨ���ı��������ֵ���ı��ѹ 
  //write_cmd(0xa6);//set inverse display	   a6 off, a7 on
  //write_cmd(0xa4);//set all pixel on
  write_cmd(0xaf);//set display enable
  LcmClear();
}
/*************************
 * 8*8�ַ���ȡģ˳��������ʽ��
 * ���ϵ��£���λ��ǰ�������ң�
 * ��ѡ��ҳ��ַ0-7����ѡ����0-130
 * ҳ����ֱ�Ӷ�ȡ8λ������Ϊ��ַ��
 * �����ȶ�ȡ����λ�����ȡ����λ��
 **********************/
void lcd12864::PUTchar8x8(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put)
{		
  unsigned int X=0;
  unsigned int i=0,j=0;
  write_cmd(0xb0+row);
  write_cmd(0x10+(8*col/16));		
  write_cmd(0x00+(8*col%16));
  for(j=0;j<count;j++)
    for(i=0;i<8;i++) write_data(put[X++]); 	

}
/*****************
 * 8*16�ַ���ȡģ˳��������ʽ��
 * ���ϵ��£���λ��ǰ�������ң�
 * ��ѡ��ҳ��ַ0-7����ѡ����0-127
 * ҳ����ֱ�Ӷ�ȡ8λ������Ϊ��ַ��
 * �����ȶ�ȡ����λ�����ȡ����λ��
 **********************/
void lcd12864::PUTchar8x16(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put)
{		
  unsigned int X=0;
 unsigned int i=0,j=0;
  write_cmd(0xb0+row);
  write_cmd(0x10+(8*col/16));		
  write_cmd(0x00+(8*col%16));
  for(j=0;j<count;j++)
  { 
    for(i=0;i<8;i++) write_data(put[X++]); 
    write_cmd(0xb1+row);	
    write_cmd(0x10+(8*col/16));		
    write_cmd(0x00+(8*col%16));
    for(i=0;i<8;i++) write_data(put[X++]);
    write_cmd(0xb0+row);
    col=col+1; 
  } 

}
/*****************
 * 16*16�ַ���ȡģ˳��������ʽ��
 * ���ϵ��£���λ��ǰ�������ң�
 * ��ѡ��ҳ��ַ0-7����ѡ����0-127
 * ҳ����ֱ�Ӷ�ȡ8λ������Ϊ��ַ��
 * �����ȶ�ȡ����λ�����ȡ����λ��
 **********************/
void lcd12864::PUTchar16x16(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put)
{		
  unsigned int X=0;
 unsigned int i=0,j=0;
  write_cmd(0xb0+row);
  write_cmd(0x10+(8*col/16));		
  write_cmd(0x00+(8*col%16));
  for(j=0;j<count;j++)
  { 
    for(i=0;i<16;i++) write_data(put[X++]); 
    write_cmd(0xb1+row);	
    write_cmd(0x10+(8*col/16));		
    write_cmd(0x00+(8*col%16));
    for(i=0;i<16;i++) write_data(put[X++]);
    write_cmd(0xb0+row);	 
    col=col+2; 
  }

}
/*****************
 * 24*24�ַ���ȡģ˳��������ʽ��
 * ���ϵ��£���λ��ǰ�������ң�
 * ��ѡ��ҳ��ַ0-7����ѡ����0-127
 * ҳ����ֱ�Ӷ�ȡ8λ������Ϊ��ַ��
 * �����ȶ�ȡ����λ�����ȡ����λ��
 **********************/
void lcd12864::PUTchar24x24(unsigned char row,unsigned char col,unsigned char count,unsigned char const *put)
{		
  unsigned int X=0;
 unsigned int i=0,j=0;
  write_cmd(0xb0+row);
  write_cmd(0x10+(8*col/16));		
  write_cmd(0x00+(8*col%16));
  for(j=0;j<count;j++)
  { 
    for(i=0;i<24;i++) write_data(put[X++]); 
    write_cmd(0xb1+row);	
    write_cmd(0x10+(8*col/16));		
    write_cmd(0x00+(8*col%16));
    for(i=0;i<24;i++) write_data(put[X++]);
    write_cmd(0xb2+row);	
    write_cmd(0x10+(8*col/16));		
    write_cmd(0x00+(8*col%16));
    for(i=0;i<24;i++) write_data(put[X++]);
    write_cmd(0xb0+row);
    col=col+3; 
  }
}
/*****************
 * ͼƬ��ȡģ˳��������ʽ��
 * ���ϵ��£�����ǰ�������ң�
 * ��ѡ��ҳ��ַ0-7����ѡ����0-127
 * ҳ����ֱ�Ӷ�ȡ8λ������Ϊ��ַ��
 * �����ȶ�ȡ����λ�����ȡ����λ��
 **********************/
void lcd12864::PUTBMP(unsigned char const *put)
{		
  unsigned int X=0;
 unsigned int i=0,j=0;
  for(j=0;j<8;j++)
  {
    write_cmd(0xb0+j); 
    write_cmd(0x10);		
    write_cmd(0x00);
    for(i=0;i<128;i++) write_data(put[X++]); 
  }	

}
/*****************
 * ͼƬ���ԣ�ȡģ˳��������ʽ��
 * ���ϵ��£�λ��ǰ�������ң�
 * ��ѡ��ҳ��ַ0-7����ѡ����0-127
 * ҳ����ֱ�Ӷ�ȡ8λ������Ϊ��ַ��
 * �����ȶ�ȡ����λ�����ȡ����λ��
 **********************/
void lcd12864::PUTREVERSEBMP(unsigned char const *put)
{		
  unsigned int X=0;
 unsigned int i=0,j=0;
  for(j=0;j<8;j++)
  {
    write_cmd(0xb0+j); 
    write_cmd(0x10);		
    write_cmd(0x00);
    for(i=0;i<128;i++) write_data(~put[X++]); 
  }	

}
/*****************
 * ������ȡģ˳��������ʽ��
 * ���ϵ��£���λ��ǰ�������ң�
 * ��ѡ��ҳ��ַ0-7����ѡ����0-127
 * ҳ����ֱ�Ӷ�ȡ8λ������Ϊ��ַ��
 * �����ȶ�ȡ����λ�����ȡ����λ��
 **********************/
void lcd12864::LcmClear()
{	 
  unsigned char x,y;
  for(y=0;y<8;y++)
  {    
    write_cmd(0xb0+y);
    write_cmd(0x10);		
    write_cmd(0x00);
    for(x=0;x<132;x++)  write_data(0); 
  }	
}	 
