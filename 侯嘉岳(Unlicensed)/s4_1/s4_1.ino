#include <Wire.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include<LiquidCrystal.h>
 int sta = 1;
  int sta1 = 0;
  int button_on=0;              //按键1 开关
int button_high=0;              //亮
int button_set=0;              //暗    
int button_sensor=0;              //自动调光按钮
unsigned long time;
unsigned long time_pr;
unsigned long time1;
unsigned long time_pr1;
unsigned long a;
unsigned long b;
// 创建lcd控制对象,并指定其引脚与Arduino控制板对应关系
const int rs=12,en=11,d4=5,d5=4,d6=3,d7=2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
int sensorValue; 
int i = 1;
//void setup()
//{
//  lcd.begin(16,2);
//  lcd.print("TaichiMaker!");
//}
// 
//void loop() {
//  // 设置光标位置在第0列,第1行
//  // 注意,行和列都是从0开始的
//  lcd.setCursor(0, 1);
//  //打印自开发板重置以来的秒数：
//  lcd.print(millis()/1000);
//}
 
//A4(SDA)  A5(SCL)
char Res;
char usart_rxcmd_flag;
char USART_RX_BUF[16];
char usart_rxcmd_cnt;
char usart_rx_timeout;

unsigned char ok_flag;
unsigned char fail_flag;

unsigned short lenth_val = 0;
unsigned char i2c_rx_buf[16];
unsigned char dirsend_flag=0;
int serial_putc( char c, struct __file * )
{
  Serial.write( c );
  return c;
}
void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}

ISR(TIMER1_COMPA_vect)
{
   digitalWrite(7, !digitalRead(7)); 
  if(usart_rx_timeout>0)
  {
    usart_rx_timeout--;
    if(usart_rx_timeout==0)
    {
    }
  }
}

void SensorWrite(unsigned char addr,unsigned char* datbuf,unsigned char cnt) 
{
  unsigned short result=0;
  // step 1: instruct sensor to read echoes
  Wire.beginTransmission(82); // transmit to device #82 (0x52)
  // the address specified in the datasheet is 164 (0xa4)
  // but i2c adressing uses the high 7 bits so it's 82
  Wire.write(byte(addr));      // sets distance data address (addr)
  // step 2: wait for address to write
  delay(1);                   // datasheet suggests at least 30uS
  // step 3: send data to sensor
  Wire.write(datbuf, cnt);    // write cnt bytes to slave device
  Wire.endTransmission();      // stop transmitting
}
void SensorRead(unsigned char addr,unsigned char* datbuf,unsigned char cnt) 
{
  unsigned short result=0;
  // step 1: instruct sensor to read echoes
  Wire.beginTransmission(82); // transmit to device #82 (0x52)
  // the address specified in the datasheet is 164 (0xa4)
  // but i2c adressing uses the high 7 bits so it's 82
  Wire.write(byte(addr));      // sets distance data address (addr)
  Wire.endTransmission();      // stop transmitting
  // step 2: wait for readings to happen
  delay(1);                   // datasheet suggests at least 30uS
  // step 3: request reading from sensor
  Wire.requestFrom((int)82, (int)cnt);    // request cnt bytes from slave device #82 (0x52)
  // step 5: receive reading from sensor
  if (cnt <= Wire.available()) { // if two bytes were received
    *datbuf++ = Wire.read();  // receive high byte (overwrites previous reading)
    *datbuf++ = Wire.read(); // receive low byte as lower 8 bits
  }
}
void setup() {
  // put your setup code here, to run once:
   lcd.begin(16,2);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(9,INPUT);
  pinMode(10,INPUT);
  pinMode(13,OUTPUT);
  Wire.begin();        // join i2c bus (address optional for master)
//  pinMode(6,OUTPUT);
//  pinMode(7,OUTPUT);
//  digitalWrite(6,HIGH);
//  digitalWrite(7,HIGH);
  Serial.begin(9600,SERIAL_8N1); 
  printf_begin();
  printf("start\n\r");
  // 初始化timer1
  cli();          // 进制全局中断
  TCCR1A = 0;     //设置TCCR1A全为0
  TCCR1B = 0;     // [size=1em]设置TCCR1B全为0

  //设置比较的值得大小
  OCR1A = 15;
  // 开启CTC模式
  TCCR1B |= (1 << WGM12);
  // 设置 CS10和CS12位
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // 允许比较中断
  TIMSK1 |= (1 << OCIE1A);
  sei();          // 允许全局中断
  printf("s5-1#");
}
void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(12,LOW);
  button_on=digitalRead(8);
  delayMicroseconds(100);
  button_high=digitalRead(9);
  delayMicroseconds(100);
  button_set=digitalRead(10);
  delayMicroseconds(100);
  button_sensor=digitalRead(A1);
  delayMicroseconds(100);
//  sensorValue = analogRead(A0);
// lcd.print(sensorValue);
  
    
   if(button_on==1)
  {
    delay(200);
    sta = !sta;
    lcd.clear();
  }
   if(sta==1)
  {
    //if(dirsend_flag)
  {
    SensorRead(0x00,i2c_rx_buf,2);
    lenth_val=i2c_rx_buf[0];
    lenth_val=lenth_val<<8;
    lenth_val|=i2c_rx_buf[1];
    printf("i2c=%dmm\n",lenth_val);// print the lenth_val
    delay(300); 
    if(lenth_val>110)
    {
      if(dirsend_flag==0)
      {
        dirsend_flag=1;
        SensorWrite(0x09,&dirsend_flag,1);
        printf("dir=%d\n",dirsend_flag);// print the dirsend_flag
        delay(300);
      }
    }
    else if(lenth_val<90)
    {
      if(dirsend_flag==1)
      {
        dirsend_flag=0;
        SensorWrite(0x09,&dirsend_flag,1);
        printf("dir=%d\n",dirsend_flag);// print the dirsend_flag
        delay(300);
      }
    }
    lcd.setCursor(0, 0);
   lcd.print("juli:");
   if(lenth_val<100)
   {
   lcd.print(lenth_val/10);
   lcd.print("cm");
   lcd.print("  ");
  }else if(lenth_val<1000&&lenth_val>100){
  lcd.print(lenth_val/10);
  lcd.print("cm");
  lcd.print(" ");
  }else {
  lcd.print(lenth_val/10);
  lcd.print("cm");
  }}
  if(lenth_val<500) lcd.print("short");
  if(lenth_val>=500) lcd.print("       ");
}
else if(sta==0){
    lcd.print("the time is ");
    lcd.print(i);
    if(button_high==1)
      {
        i=i+1;
        delay(200);
        lcd.clear();
      }
      if(button_set==1)
      {
        delay(200);
        sta1 = !sta1;
        lcd.clear();
        time_pr=millis();
      }
      if(sta1==1){
        // time_pr=millis();
        
      while(i>=a)
      {
        Serial.println(a);
         time = millis();
         
          lcd.setCursor(0, 0);
          lcd.print("the time is ");
          lcd.print(a);
          lcd.print("s");
          delay(20);
          b=i-a;
          if (b==1)//beep
          {
            // time1 = millis();
            // time_pr1 = millis();
            // while(time1-time_pr1<1000)
            // {
            //   time_pr1 = millis(); 
              digitalWrite(13, 1);
            // }
            
          //   a=time_pr1-t
          //   if (time1==time_pr1)
          //  digitalWrite(A1, 1);
          //  delay(998);
            // sta1 =0;
            // break;
          }else if (i==a)//beep
          {
            // time1 = millis();
            // time_pr1 = millis();
            // while(time1-time_pr1<1000)
            // {
            //   time_pr1 = millis(); 
              // digitalWrite(13, 1);
            // }
            
          //   a=time_pr1-t
          //   if (time1==time_pr1)
          //  digitalWrite(A1, 1);
          //  delay(998);
          digitalWrite(13, 0);
            sta1 =0;
            break;
          }
        a = (time-time_pr)/1000;
          lcd.clear();



        
      }
    }else{
      // if (a==i)//beep
      //     {
      //      time1 = millis();
      //       time_pr1 = millis();
      //       while(time1-time_pr1<1000)
      //       {
      //         time_pr1 = millis(); 
      //         digitalWrite(A1, 1);
      //       }
      //       sta1 =0;
      //       a=0;
      //       // break;
      //     }
      lcd.setCursor(0, 0);
      lcd.print("the time is ");
    lcd.print(i);
    }
  }
}
