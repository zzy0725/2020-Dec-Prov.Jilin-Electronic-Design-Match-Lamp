#include <Wire.h>
#include <avr/io.h>
#include <avr/interrupt.h>
const int analogInPin = A0;  // 模拟输入引脚
const int analogOutPin = 9; //  PWM输出引脚
int button_on = 0;            //按键1 开关
int button_high = 0;            //亮
int button_low = 0;            //暗
int button_sensor = 0;            //自动调光按钮
int sensorValue = 0;        // 电位器电压值
int outputValue = 0;        // 模拟量输出值（PWM）
int sta = 0;
int sta1 = 0;
int m;
int mx = 255;
int mi = 130;
char Res;
char usart_rxcmd_flag;
char USART_RX_BUF[16];
char usart_rxcmd_cnt;
char usart_rx_timeout;

unsigned char dcalib_flag = 0;
unsigned short lenth_buf[10];
unsigned short lenth_cnt = 0;
unsigned short lenth, avelenth;
unsigned char calib_cnt;
unsigned char ok_flag;
unsigned char fail_flag;


unsigned short lenth_val = 0;
unsigned char i2c_rx_buf[16];
unsigned char dirsend_flag = 0;
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
  if (usart_rx_timeout > 0)
  {
    usart_rx_timeout--;
    if (usart_rx_timeout == 0)
    {
    }
  }
}

void SensorWrite(unsigned char addr, unsigned char* datbuf, unsigned char cnt)
{
  unsigned short result = 0;
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
void SensorRead(unsigned char addr, unsigned char* datbuf, unsigned char cnt)
{
  unsigned short result = 0;
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
  Wire.begin();        // join i2c bus (address optional for master)
  m = 4;
  pinMode(11, OUTPUT); //光耦控制端
  pinMode(2, INPUT); //按键
  pinMode(3, INPUT); //按键
  pinMode(4, INPUT); //按键
  pinMode(5, INPUT); //按键
  Serial.begin(9600, SERIAL_8N1);
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
  // put your main code here, to run repeatedly:
  button_on = digitalRead(2);
  delayMicroseconds(100);
  button_high = digitalRead(3);
  delayMicroseconds(100);
  button_low = digitalRead(4);
  delayMicroseconds(100);
  button_sensor = digitalRead(5);
  delayMicroseconds(100);

  //   int button_on=0;              //按键1 开关
  // int button_high=0;              //亮
  // int button_low=0;              //暗
  // int button_sensor=0;              //自动调光按钮

  if (button_on == 1)
  {
    delay(200);
    sta = !sta;
  }
  if (sta == 1)
  {
    //if(dirsend_flag)
    {
      SensorRead(0x00, i2c_rx_buf, 2);
      lenth_val = i2c_rx_buf[0];
      lenth_val = lenth_val << 8;
      lenth_val |= i2c_rx_buf[1];
      printf("i2c=%dmm\n", lenth_val); // print the lenth_val
      delay(30);
      if (lenth_val > 110)
      {
        if (dirsend_flag == 0)
        {
          dirsend_flag = 1;
          SensorWrite(0x09, &dirsend_flag, 1);
          printf("dir=%d\n", dirsend_flag); // print the dirsend_flag
          delay(30);
        }
      }
      else if (lenth_val < 90)
      {
        if (dirsend_flag == 1)
        {
          dirsend_flag = 0;
          SensorWrite(0x09, &dirsend_flag, 1);
          printf("dir=%d\n", dirsend_flag); // print the dirsend_flag
          delay(30);
        }
      }
    }    // digitalWrite(analogOutPin, 1);
    //  while(Serial.available()>0){
    //   Res =Serial.read();
    //   if(usart_rxcmd_flag==0)
    //   {
    //     USART_RX_BUF[usart_rxcmd_cnt]=Res;
    //     usart_rx_timeout=5;
    //     usart_rxcmd_cnt++;
    //     if(usart_rxcmd_cnt>(16-1))
    //       usart_rxcmd_cnt=0;
    //   }
    // }
    // if(usart_rxcmd_flag==1)
    // {
    //   CalDisplay_Length();
    //   usart_rxcmd_flag=0;
    // }

    if (button_sensor == 1)
    {
      delay(200);
      sta1 = !sta1;
    }
    if (sta1 == 1)
    {
      // 读取模拟量值
      sensorValue = analogRead(analogInPin);
      // 变换数据区间
      outputValue = map(sensorValue, 0, 1023, 0, 255);
      // 输出对应的PWM值
      //  analogWrite(analogOutPin, outputValue);

      Serial.print("sensor = " );
      Serial.print(sensorValue);
      Serial.print("\t output = ");
      Serial.println(outputValue);

      if (sensorValue > 140)analogWrite(analogOutPin, 50);
      //  if(sensorValue>100&&sensorValue<140)analogWrite(analogOutPin, 110);
      if (sensorValue > 60 && sensorValue < 140)analogWrite(analogOutPin, 150);
      if (sensorValue < 60)analogWrite(analogOutPin, 255);
      // 等待2ms进行下一个循环
      // 取保能稳定读取下一次数值
      delay(20);
    } else
    {
      if (button_high == 1)
      {
        delay(300);
        m = m + 1;
        if (m > 8) {
          m = 8;
        }
      }
      Serial.print(m);
      if (button_low == 1)
      {
        delay(300);
        m = m - 1;
        if (m < 1) {
          m = 1;
        }
      }
      switch (m)
      {
        case 1:
          digitalWrite(analogOutPin, 0);
          break;
        case 2:
          analogWrite(analogOutPin, 30);
          break;
        case 3:
          analogWrite(analogOutPin, 70);
          break;
        case 4:
          analogWrite(analogOutPin, 110);
          break;
        case 5:
          analogWrite(analogOutPin, 150);
          break;
        case 6:
          analogWrite(analogOutPin, 180);
          break;
        case 7:
          analogWrite(analogOutPin, 210);
          break;
        case 8:
          analogWrite(analogOutPin, 230);
          break;
        case 9:
          analogWrite(analogOutPin, 255);
          break;
        default:
          digitalWrite(analogOutPin, 0);
          break;
      }
    }

  }
  else
  {
    digitalWrite(analogOutPin, 0);
  }





}
