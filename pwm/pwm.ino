#include<Arduino.h>
#include <Servo.h>  

Servo myservo1;
Servo myservo2;
Servo myservo3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  myservo1.attach(5);
  myservo2.attach(6);
  myservo3.attach(9);
}

int sg90(unsigned char command) {
  switch(command){
    case 0x01:
      Serial.write(0xEE);
    break;
    case 0x02:
      Serial.write(0xEE);
    break;
    case 0x03:
      Serial.write(0xEE);
    break;
    case 0x04:
      Serial.write(0xEE);
    break;
    case 0x05:
      Serial.write(0xEE);
    break;
    case 0x06:
      Serial.write(0xEE);
    break;
    case 0x07:
      Serial.write(0xEE);
    break;
    case 0x08:
      Serial.write(0xEE);
    break;
    case 0x0F:
      Serial.write(0xEE);
    break;
    case 0xFF:
      Serial.write(0xEE);
    break;
    default:
      Serial.write(0xDD);
    break;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char data;
  unsigned char data_r;
  int i=0;
  int status=0;
  while(1){
    while(Serial.available()>0) {
      data_r=Serial.read();
      switch(i){
        case 0:
          if(data_r==0xF0)  i=1;    //起始位1
        break;
        case 1:
          if(data_r==0xA5)  i=2;    //起始位2
        break;
        case 2:
          if(data_r==0xF5){         //如果在接收到数据位之前接收到停止位，丢弃此帧数据
            i=0;
            status=3;               //数据位丢失status为3
          }
          else{
            data=data_r;               //数据位赋值
            status=1;               //成功接收到数据位status为1
            i=3;
          }
        break;
        case 3:
          if(data_r==0xF5){
            i=0;
            status++;               //同时成功接收数据位和停止位status为2
          }
          else{
            i=0;
            status=4;
          }
        break;
      }
      if(status==4){
        Serial.write(0xCC);
        status=0;
      }
      if(status==3){
        Serial.write(0xBB);
        status=0;
      }
      if(status==2){
        Serial.write(0XAA);
        status=0;
        sg90(data);
      }
    }
    if(status!=0){
      if(i==3){
        Serial.write(0xCC);
        status=0;
      }

    }
  }
}
