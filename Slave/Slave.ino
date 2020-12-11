#include <stdio.h>
#include <LiquidCrystal.h>

String str = "";
String p =  "";
String distance =  "";
String voltage =  "";
String current =  "";
String watt =  "";
int longsit = 30000;//久坐时间设置(ms),setable
int disttolight = 100;//距离设置(mm),setable
int dist = 20;
int volt = 0;
int curr = 0;
int wat = 0;
long s;

// 初始化LCD针脚
LiquidCrystal LCD(12, 11, 5, 4, 3, 2);

void serialread()
{
  str = "";
  p =  "";
  distance =  "";
  voltage =  "";
  current =  "";
  watt =  "";
  while (Serial.available() > 0)
  {
    str += char(Serial.read());
    delay(2);
  }
  if (str.length() > 0 ) //
  {
    p = str.substring(0, 1);
    distance = str.substring(1, 5);
    voltage = str.substring(8, 3);
    current = str.substring(13, 3);
    watt = str.substring(15, 2);
    dist = distance.toInt();
    volt = voltage.toInt();
    curr = current.toInt();
    wat = watt.toInt();
  }
}

void distancecomp()
{
  if (dist < disttolight)//这是个全局变量,unit mm
  {
    digitalWrite(13, LOW);
    delay(80);
    digitalWrite(13, HIGH);
    delay(80);
    digitalWrite(13, LOW);
    delay(80);
    digitalWrite(13, HIGH);
    delay(80);
  }
}

void norUI()
{
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Mode:");
  LCD.print(p);
  LCD.print(" ");
  LCD.print("Time:");
  long sw = (millis() / 1000);
  long s = (sw % 100);
  LCD.print(s);
  LCD.print("s");
  LCD.setCursor(0, 1);
  LCD.print("Dist:");
  LCD.print(distance);
  LCD.print("mm");
  LCD.print(" ");
  LCD.print("MENU");
  delay(500);
}

void menuUI()//10确定，9下一个，8上一个
{
firstmenu:
  delay(100);
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("*****MENU1******");
  LCD.setCursor(0, 1);
  LCD.print("  Set Distance");
  while (1)
  {
    if (digitalRead(10) == 1)//进入
    {
      delay(500);
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print(" Edit Distance:");
      disttolight = 20;
      LCD.setCursor(0, 1);
      LCD.print(disttolight);
      while (1)
      {
        delay(500);
        if (digitalRead(8) == 1)//+
        {
          delay(280);
          disttolight = disttolight + 10;
          LCD.setCursor(0, 1);
          LCD.print(disttolight);
        }
        if (digitalRead(9) == 1)//-
        {
          delay(280);
          disttolight = disttolight - 10;
          LCD.setCursor(0, 1);
          LCD.print(disttolight);
        }
        if (digitalRead(10) == 1) //确定
        {
          delay(280);
          break;
        }
        break;
      }
    }
    if (digitalRead(8) == 1)//下一条
    {
      delay(280);
      goto secondmenu;
    }
    if (digitalRead(9) == 1)//上一条
    {
      delay(280);
      digitalWrite(13, LOW);
      delay(100);
      digitalWrite(13, HIGH);
    }
  }
  delay(100);
secondmenu:
  delay(280);
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("*****MENU2******");
  LCD.setCursor(0, 1);
  LCD.print("  Set Reminder");
  while (1)//循环扫描用户动作
  {
    if (digitalRead(10) == 1)//进入
    {
      delay(500);
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print(" Edit Reminder:");
      LCD.setCursor(0, 1);
      longsit = 1000;//ms
      LCD.setCursor(0, 1);
      LCD.print(longsit);
      while (1)
      {
        delay(280);
        if (digitalRead(8) == 1)//+
        {
          delay(280);
          longsit = longsit + 50;
          LCD.setCursor(0, 1);
          LCD.print(longsit);
        }
        if (digitalRead(9) == 1)//-
        {
          delay(280);
          longsit = longsit - 50;
          LCD.setCursor(0, 1);
          LCD.print(longsit);
        }
        if (digitalRead(10) == 1)//确定
        {
          delay(280);
          break;
        }
        break;
      }
    }
    if (digitalRead(8) == 1)//下一条
    {
      delay(280);
      goto thirdmenu;
    }
    if (digitalRead(9) == 1)//上一条
    {
      delay(280);
      goto firstmenu;
    }
  }
  delay(100);
thirdmenu:
  delay(280);
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("*****MENU3******");
  LCD.setCursor(0, 1);
  LCD.print("  POWER Detect");
  while (1)
  {
    if (digitalRead(10) == 1)//进入
    {
      delay(500);
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("U=");
      LCD.print(volt);
      LCD.print("V");
      LCD.print("  ");
      LCD.print("I=");
      LCD.print(curr);
      LCD.print("mA");
      LCD.setCursor(0, 1);
      LCD.print("P=");
      LCD.print(wat);
      LCD.print("W");
      delay(200);
    }
    if (digitalRead(8) == 1)//下一条
    {
      delay(280);
      loop();
    }
    if (digitalRead(9) == 1)//上一条
    {
      delay(280);
      goto secondmenu;
    }
  }
}

void setup()
{
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(13, OUTPUT);//
  LCD.begin(16, 2); //初始化,设置列行
  Serial.begin(9600);
  digitalWrite(13, HIGH);
}

void loop()
{
  if (digitalRead(7) == 1)
  {
    delay(200);
    menuUI();
  }
  serialread();
  norUI();
  distancecomp();
}
