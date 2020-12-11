#include <LiquidCrystal.h>    //调用LCD1602库

int p = 0;//目前所处的亮度
int s = 0;//是否已经开机
int innt = 0; //距离的整数值(mm)
String distance = "";//距离String
int n = 0; //距离值(innt)的位数
int menubutt = 0;//菜单状态
long adelay = 0;

//A1电源按钮
//A2亮度/时间加
//A3亮度/时间减
//A4自动调节
//A5菜单
//A6确定
//A7退出


void kaideng()//以最低亮度开灯
{
  p = 1;
  digitalWrite(26, HIGH);//直火线继电器
  delay(150);
  digitalWrite(24, LOW);//调光器继电器
  delay(150);
  digitalWrite(30, LOW);
  digitalWrite(32, HIGH);
  digitalWrite(34, HIGH);
  digitalWrite(36, HIGH);
  digitalWrite(38, HIGH);
  digitalWrite(40, HIGH);
}

void guandeng()//关灯(断电)
{
  p = 0;
  digitalWrite(26, HIGH);//不直接与火线相接
  delay(200);
  digitalWrite(24, HIGH);//调光器继电器
  delay(200);
  digitalWrite(30, HIGH);
  digitalWrite(32, HIGH);
  digitalWrite(34, HIGH);
  digitalWrite(36, HIGH);
  digitalWrite(38, HIGH);
  digitalWrite(40, HIGH);
}

void liangduup()//8档亮度升高
{
  if (p == 0)
  {
    p++;
    kaideng();
  }
  else if (p == 1)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(150);
    digitalWrite(30, HIGH);
    delay(150);
    digitalWrite(32, LOW);
    p++;
  }
  else if (p == 2)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(150);
    digitalWrite(32, HIGH);
    delay(150);
    digitalWrite(34, LOW);
    p++;
  }
  else if (p == 3)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(150);
    digitalWrite(34, HIGH);
    delay(150);
    digitalWrite(36, LOW);
    p++;
  }
  else if (p == 4)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(150);
    digitalWrite(36, HIGH);
    delay(150);
    digitalWrite(38, LOW);
    p++;
  }
  else if (p == 5)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(150);
    digitalWrite(38, HIGH);
    delay(150);
    digitalWrite(40, LOW);
    p++;
  }
  else if (p == 6)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(150);
    digitalWrite(40, HIGH);
    delay(300);
    digitalWrite(24, HIGH);
    delay(300);
    digitalWrite(26, LOW);//最高亮度继电器
    p++;
  }
  else if (p == 7)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(150);
    digitalWrite(8, LOW);
    delay(100);
    digitalWrite(8, HIGH);//最高档再加，蜂鸣器提示错误
  }
}

void liangdudown()//8档亮度降低
{
  if (p == 0)
  {
    digitalWrite(8, LOW);
    delay(100);
    digitalWrite(8, HIGH);//最低档再减，蜂鸣器提示错误
  }
  else if (p == 1)
  {
    p--;
    guandeng();
  }
  else if (p == 2)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(150);
    digitalWrite(32, HIGH);
    delay(150);
    digitalWrite(30, LOW);
    p--;
  }
  else if (p == 3)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(150);
    digitalWrite(34, HIGH);
    delay(150);
    digitalWrite(32, LOW);
    p--;
  }
  else if (p == 4)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(150);
    digitalWrite(36, HIGH);
    delay(150);
    digitalWrite(34, LOW);
    p--;
  }
  else if (p == 5)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(150);
    digitalWrite(38, HIGH);
    delay(150);
    digitalWrite(36, LOW);
    p--;
  }
  else if (p == 6)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(150);
    digitalWrite(40, HIGH);
    delay(150);
    digitalWrite(38, LOW);
    p--;
  }
  else if (p == 7)
  {
    digitalWrite(26, HIGH);//直火线继电器
    delay(150);
    digitalWrite(24, LOW);//调光器继电器
    delay(500);
    digitalWrite(40, LOW);
    p--;
  }
}

void zidong()//自动亮度设置
{
  if (analogRead(9) > 300)//1
  {
    digitalWrite(26, HIGH);//最高亮度继电器
    delay(200);
    digitalWrite(24, LOW);//调光器继电器
    delay(100);
    digitalWrite(30, LOW);
    digitalWrite(32, HIGH);
    digitalWrite(34, HIGH);
    digitalWrite(36, HIGH);
    digitalWrite(38, HIGH);
    digitalWrite(40, HIGH);
    p = 1;
    s = 1;
  }
  if (analogRead(9) > 50 && analogRead(9) < 300)//2
  {
    digitalWrite(26, HIGH);//最高亮度继电器
    delay(200);
    digitalWrite(24, LOW);//调光器继电器
    delay(100);
    digitalWrite(30, HIGH);
    digitalWrite(32, LOW);
    digitalWrite(34, HIGH);
    digitalWrite(36, HIGH);
    digitalWrite(38, HIGH);
    digitalWrite(40, HIGH);
    p = 2;
    s = 1;
  }
  if (analogRead(9) > 35 && analogRead(9) < 50)//3
  {
    digitalWrite(26, HIGH);//最高亮度继电器
    delay(200);
    digitalWrite(24, LOW);//调光器继电器
    delay(100);
    digitalWrite(30, HIGH);
    digitalWrite(32, HIGH);
    digitalWrite(34, LOW);
    digitalWrite(36, HIGH);
    digitalWrite(38, HIGH);
    digitalWrite(40, HIGH);
    p = 3;
    s = 1;
  }
  if (analogRead(9) > 21 && analogRead(9) < 35)//4
  {
    digitalWrite(26, HIGH);//最高亮度继电器
    delay(200);
    digitalWrite(24, LOW);//调光器继电器
    delay(100);
    digitalWrite(30, HIGH);
    digitalWrite(32, HIGH);
    digitalWrite(34, HIGH);
    digitalWrite(36, LOW);
    digitalWrite(38, HIGH);
    digitalWrite(40, HIGH);
    p = 4;
    s = 1;
  }
  if (analogRead(9) > 14 && analogRead(9) < 21)//5
  {
    digitalWrite(26, HIGH);//最高亮度继电器
    delay(200);
    digitalWrite(24, LOW);//调光器继电器
    delay(100);
    digitalWrite(30, HIGH);
    digitalWrite(32, HIGH);
    digitalWrite(34, HIGH);
    digitalWrite(36, HIGH);
    digitalWrite(38, LOW);
    digitalWrite(40, HIGH);
    p = 5;
    s = 1;
  }
  if (analogRead(9) > 7 && analogRead(9) < 14)//6
  {
    digitalWrite(26, HIGH);//最高亮度继电器
    delay(200);
    digitalWrite(24, LOW);//调光器继电器
    delay(100);
    digitalWrite(30, HIGH);
    digitalWrite(32, HIGH);
    digitalWrite(34, HIGH);
    digitalWrite(36, HIGH);
    digitalWrite(38, HIGH);
    digitalWrite(40, LOW);
    p = 6;
    s = 1;
  }
  if (analogRead(9) < 7)//7
  {
    digitalWrite(24, HIGH);//调光器继电器
    delay(200);
    digitalWrite(26, LOW);//最高亮度继电器
    delay(100);
    digitalWrite(30, HIGH);
    digitalWrite(32, HIGH);
    digitalWrite(34, HIGH);
    digitalWrite(36, HIGH);
    digitalWrite(38, HIGH);
    digitalWrite(40, HIGH);
    p = 7;
    s = 1;
  }
}

void julijiance()//距离检测并报警+传输
{
  innt = 0;
  n = 0;
  String distance = "";
  while (Serial3.available() > 0)
  {
    distance += char(Serial3.read());
    delay(2);
  }
  innt = distance.toInt();
}

void sleepmode()//最低亮度睡眠模式
{
  if (analogRead(15) < 100 && s == 1)
  {
    kaideng();
    delay(1000);
  }
}

void button()//基础按钮扫描
{
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);

  if (analogRead(1) >= 1000)//电源按钮
  {
    if (analogRead(1) >= 1000)//去抖
    {
      if (s == 0)
      {
        s = 1;
        kaideng();
        delay(90);
      }
      else if (s != 0)
      {
        s = 0;
        guandeng();
        delay(90);
      }
    }
  }
  if (analogRead(2) >= 1000)//亮度/时间加
  {
    delay(90);
    if (analogRead(2) >= 1000)//去抖
    {
      if (s != 0 && menubutt == 0)
      {
        liangduup();
      }
      if (s != 0 && menubutt == 1)
      {
        digitalWrite(5, HIGH);
        delay(90);
        digitalWrite(5, LOW);
      }
      delay(100);
    }
  }
  if (analogRead(3) >= 1000)//亮度,时间减
  {
    delay(90);
    if (analogRead(3) >= 1000)//去抖
    {
      if (s != 0 && menubutt == 0)
      {
        liangdudown();
      }
      if (s != 0 && menubutt == 1)
      {
        digitalWrite(6, HIGH);
        delay(90);
        digitalWrite(6, LOW);
      }
      delay(100);
    }
  }
  if (analogRead(4) >= 1000)//自动调节
  {
    delay(90);
    if (analogRead(4) >= 1000)//去抖
    {
      zidong();
      delay(90);
    }
  }
  if (analogRead(5) >= 1000)//确定
  {
    delay(90);
    if (analogRead(5) >= 1000)//去抖
    {
      digitalWrite(7, HIGH);
      delay(90);
      digitalWrite(7, LOW);
    }
  }
  if (analogRead(6) >= 1000)//取消
  {
    delay(90);
    if (analogRead(6) >= 1000)//去抖
    {
      if (s != 0)
        menubutt = 0;
    }
  }
  if (analogRead(7) >= 1000)//菜单
  {
    delay(90);
    if (analogRead(7) >= 1000)//去抖
    {
      if (s != 0 && menubutt == 0)
      {
        digitalWrite(4, HIGH);//4电平为高时显示菜单
        menubutt = 1;//菜单状态
      }
      else if (s != 0 && menubutt == 1)
      {
        delay(100);
        digitalWrite(4, LOW);//4电平为低时隐藏菜单
        menubutt = 0;//菜单状态
      }
    }
  }
}

void VAP()
{

}


//主到辅助单片机之间串口数据格式
//主—>辅UART[档位(1bit)][当前距离cm(4bit)][电压V(3bit)][电流mA(3bit)][总功率W(2bit)]
//pin4—>7,pin5—>8,pin6—>9,pin7—>10

void SerialEvent()//10bit
{
  Serial1.print(p);
  if (innt >= 0 && innt <= 9) //0
  {
    Serial1.print("0");
    Serial1.print("0");
    Serial1.print("0");
    Serial1.print(innt);
  }
  if (innt >= 10 && innt <= 99)//00
  {
    Serial1.print("0");
    Serial1.print("0");
    Serial1.print(innt);
  }
  if (innt >= 100 && innt <= 999)//000
  {
    Serial1.print("0");
    Serial1.print(innt);
  }
  if (innt >= 1000 && innt <= 2000)//0000
  {
    Serial1.print(innt);
  }
  //V
  Serial1.print('2');
  Serial1.print('3');
  Serial1.print(random(1, 10));
  // I+P
  if (p == 0)
  {
    Serial1.print('0');
    Serial1.print('3');
    Serial1.print('4');
    Serial1.print('0');
    Serial1.println('5');
  }
  if (p == 1)
  {
    Serial1.print('0');
    Serial1.print('9');
    Serial1.print('1');
    Serial1.print('1');
    Serial1.println('3');
  }
  if (p == 2)
  {
    Serial1.print('1');
    Serial1.print('2');
    Serial1.print('8');
    Serial1.print('1');
    Serial1.println('9');
  }
  if (p == 3)
  {
    Serial1.print('1');
    Serial1.print('4');
    Serial1.print('4');
    Serial1.print('3');
    Serial1.println('7');
  }
  if (p == 4)
  {
    Serial1.print('1');
    Serial1.print('9');
    Serial1.print('5');
    Serial1.print('4');
    Serial1.println('2');
  }
  if (p == 5)
  {
    Serial1.print('2');
    Serial1.print('2');
    Serial1.print('1');
    Serial1.print('5');
    Serial1.println('5');
  }
  if (p == 6)
  {
    Serial1.print('2');
    Serial1.print('8');
    Serial1.print('4');
    Serial1.print('5');
    Serial1.println('9');
  }
  if (p == 7)
  {
    Serial1.print('3');
    Serial1.print('4');
    Serial1.print('7');
    Serial1.print('6');
    Serial1.println('5');
  }
}

void setup()
{
  pinMode(4, OUTPUT);//NANO 7
  pinMode(5, OUTPUT);//NANO 8
  pinMode(6, OUTPUT);//NANO 9
  pinMode(7, OUTPUT);//NANO 10
  pinMode(24, OUTPUT);//调光器切换
  pinMode(26, OUTPUT);//最高亮度
  pinMode(30, OUTPUT);//1档
  pinMode(32, OUTPUT);//2档
  pinMode(34, OUTPUT);//3档
  pinMode(36, OUTPUT);//4档
  pinMode(38, OUTPUT);//5档
  pinMode(40, OUTPUT);//6档
  pinMode(8, OUTPUT);//蜂鸣器
  digitalWrite(24, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(30, HIGH);
  digitalWrite(32, HIGH);
  digitalWrite(34, HIGH);
  digitalWrite(36, HIGH);
  digitalWrite(38, HIGH);
  digitalWrite(40, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(4, LOW);//NANO 7
  digitalWrite(5, LOW);//NANO 8
  digitalWrite(6, LOW);//NANO 9
  digitalWrite(7, LOW);//NANO 10
  Serial.begin(9600);//保留
  Serial1.begin(9600);//定时辅助Arduino
  Serial2.begin(9600);//功率模块
  Serial3.begin(9600);//激光雷达
}

void loop()
{
  button();
  julijiance();
  sleepmode();
  SerialEvent();
  delay(90);
}
