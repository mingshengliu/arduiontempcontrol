/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  Modified by Roger Clark. www.rogerclark.net for Maple mini 25th April 2015 , where the LED is on PB1
  

 pinMode(PB0, PWM);
 pinMode(PB6, PWM);
 pinMode(PB7, PWM);
        pinMode(PA10, PWM);
        pinMode(PA9, PWM);
        pinMode(PA8, PWM);
        pinMode(PA7, PWM);
        pinMode(PA6, PWM);
        pinMode(PA3, PWM);
        pinMode(PA2, PWM);
        pinMode(PA1, PWM);
        pinMode(PA0, PWM);
        
 */        
//#include <LiquidCrystal_PCF8574.h>
//#include <Wire.h>        

#define IntakeLED   PA0
#define CycleLED    PA1
#define AutoLED     PA2
#define Slider1LED  PA3
#define Slider2LED  PA6
#define Slider3LED  PA7
#define Slider4LED  PA8
#define Slider5LED  PA9
#define Slider6LED  PA10
#define PM25LED     PB8


#define beepout     PB9

#define IntakeKey   PB12
#define CycleKey    PB13
#define AutoKey     PB14
#define Slider1Key  PB15
#define Slider2Key  PB3
#define Slider3Key  PB4
#define Slider4Key  PB5
#define Slider5Key  PB6
#define Slider6Key  PB7



//LiquidCrystal_PCF8574 lcd(0x3F);
//struct timer periodic_timer, arp_timer , led_timer;
typedef enum {
  intakekey = 0,
  cyclekey,
  autokey,
  slider1key,
  slider2key,
  slider3key,
  slider4key,
  slider5key,
  slider6key,
  num_key,
}key_num;
const uint16 LED_ON =65535;
const uint16 LED_MID =32768;
const uint16 LED_OFF =0;

#define pm25timeout 300000
uint8 keypressFlag = 255;
uint8 pm25ledflashflag = 0; //for pm2.5 LED flsh 3 times
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  pinMode(IntakeLED, PWM);
  pinMode(CycleLED, PWM);
  pinMode(AutoLED, PWM);
  pinMode(Slider1LED, PWM);
  pinMode(Slider2LED, PWM);
  pinMode(Slider3LED, PWM);
  pinMode(Slider4LED, PWM);
  pinMode(Slider5LED, PWM);
  pinMode(Slider6LED, PWM);
  pinMode(PM25LED, PWM);
  
  pinMode(beepout, OUTPUT);
  pinMode(PC13, OUTPUT);
  
  pwmWrite(IntakeLED, LED_OFF);   // turn the LED off (HIGH is the voltage level)
  pwmWrite(AutoLED, LED_OFF);
  pwmWrite(CycleLED, LED_OFF);
  pwmWrite(Slider1LED, LED_OFF);
  pwmWrite(Slider2LED, LED_OFF);
  pwmWrite(Slider3LED, LED_OFF);
  pwmWrite(Slider4LED, LED_OFF);
  pwmWrite(Slider5LED, LED_OFF);
  pwmWrite(Slider6LED, LED_OFF);
  pwmWrite(PM25LED, LED_OFF);
  
  
  pinMode(IntakeKey,INPUT_PULLUP);
  pinMode(CycleKey,INPUT_PULLUP);
  pinMode(AutoKey,INPUT_PULLUP);
  pinMode(Slider1Key,INPUT_PULLUP);
  pinMode(Slider2Key,INPUT_PULLUP);
  pinMode(Slider3Key,INPUT_PULLUP);
  pinMode(Slider4Key,INPUT_PULLUP);
  pinMode(Slider5Key,INPUT_PULLUP);
  pinMode(Slider6Key,INPUT_PULLUP);
  beep3();
  for(int i =0;i<3;i++)
  {
    LED_All_ON();
    delay(300);
    
    LED_All_OFF();
    delay(300);
  }
  /*
  lcd.begin(16, 2);
  lcd.setBacklight(255);
   lcd.home();
  lcd.print("          Hello!");
   for (int i = 0; i < 5; i++)
  {
    lcd.scrollDisplayLeft();
    delay(500);
    
  }*/
}
unsigned long previous_time;
uint8 keepkey=0xff;
// the loop function runs over and over again forever
void loop() {
  unsigned long current_time = millis();
  unsigned long time_passed;
  static uint ledlevel = 0;
  if(current_time > previous_time ){
    time_passed = current_time - previous_time;
  }
  else{
    time_passed = 0;//ULONG_MAX - previous_time + current_time;
  }

 if(time_passed >= 100){
    //beep3();
    update4(time_passed);
    updatepm25LED(time_passed);
    if(keypressFlag){
      
      keypressFlag = 0;
    }
    previous_time = current_time;
    
    
  }
  
  if((KEY_Scan()!= keepkey) && keypressFlag && (KEY_Scan()< num_key) ){
    keepkey = KEY_Scan();
    beep1();
    keypressFlag = 0;
    ledlevel = 0;
    }
    if(!keypressFlag){
      
      LED_SELECT(keepkey);
      keypressFlag = 1;
      previous_time = current_time;
      
    }
  
}
void LED_All_OFF(void)
{
  pwmWrite(IntakeLED, LED_OFF);   // turn the LED off (HIGH is the voltage level)
  pwmWrite(AutoLED, LED_OFF);
  pwmWrite(CycleLED, LED_OFF);
  pwmWrite(Slider1LED, LED_OFF);
  pwmWrite(Slider2LED, LED_OFF);
  pwmWrite(Slider3LED, LED_OFF);
  pwmWrite(Slider4LED, LED_OFF);
  pwmWrite(Slider5LED, LED_OFF);
  pwmWrite(Slider6LED, LED_OFF);
  pwmWrite(PM25LED, LED_OFF);
}
void LED_All_ON(void)
{
  pwmWrite(IntakeLED, LED_ON);   // turn the LED off (HIGH is the voltage level)
  pwmWrite(AutoLED, LED_ON);
  pwmWrite(CycleLED, LED_ON);
  pwmWrite(Slider1LED, LED_ON);
  pwmWrite(Slider2LED, LED_ON);
  pwmWrite(Slider3LED, LED_ON);
  pwmWrite(Slider4LED, LED_ON);
  pwmWrite(Slider5LED, LED_ON);
  pwmWrite(Slider6LED, LED_ON);
  pwmWrite(PM25LED, LED_ON);
}
uint8 KEY_Scan(void)
{
    if(!digitalRead(IntakeKey))
    {
       delay(5);
       if(!digitalRead(IntakeKey))
         { 
           return intakekey;
         }
     }
      if(!digitalRead(AutoKey))
      {
         delay(5);
         if(!digitalRead(AutoKey))
           { 
             return autokey;
           }
       }
    if(!digitalRead(CycleKey))
    {
       delay(5);
       if(!digitalRead(CycleKey))
         { 
           return cyclekey;
         }
     }
    if(!digitalRead(Slider1Key))
    {
       delay(5);
       if(!digitalRead(Slider1Key))
         { 
           return slider1key;
         }
     }
    if(!digitalRead(Slider2Key))
    {
       delay(5);
       if(!digitalRead(Slider2Key))
         { 
           return slider2key;
         }
     }
    if(!digitalRead(Slider3Key))
    {
       delay(5);
       if(!digitalRead(Slider3Key))
         { 
           return slider3key;
         }
     }
    if(!digitalRead(Slider4Key))
    {
       delay(5);
       if(!digitalRead(Slider4Key))
         { 
           return slider4key;
         }
     }
    if(!digitalRead(Slider5Key))
    {
       delay(5);
       if(!digitalRead(Slider5Key))
         { 
           return slider5key;
         }
     }
    if(!digitalRead(Slider6Key))
    {
       delay(5);
       if(!digitalRead(Slider6Key))
         { 
           return slider6key;
         }
     }
    return 255;
}

void LED_SELECT(uint8 key)
{
    switch(key)
    {
      case intakekey:
        //beep1();
        pwmWrite(IntakeLED,LED_ON);
        pwmWrite(CycleLED,LED_OFF);
        break;
      case cyclekey:
        //beep2();
        pwmWrite(IntakeLED,LED_OFF);
        pwmWrite(CycleLED,LED_ON);
        break;
      case autokey:
        //beep2();
        pwmWrite(AutoLED,LED_MID);
        pwmWrite(Slider1LED,LED_MID);
        pwmWrite(Slider2LED,LED_MID);
        pwmWrite(Slider3LED,LED_MID);
        pwmWrite(Slider4LED,LED_MID);
        pwmWrite(Slider5LED,LED_MID);
        pwmWrite(Slider6LED,LED_MID);
        break;
      case slider1key:
        //beep2();
        pwmWrite(AutoLED,LED_OFF);
        pwmWrite(Slider1LED,LED_ON);
        pwmWrite(Slider2LED,LED_OFF);
        pwmWrite(Slider3LED,LED_OFF);
        pwmWrite(Slider4LED,LED_OFF);
        pwmWrite(Slider5LED,LED_OFF);
        pwmWrite(Slider6LED,LED_OFF);
        break;
      case slider2key:
        //beep2();
        pwmWrite(AutoLED,LED_OFF);
        pwmWrite(Slider1LED,LED_ON);
        pwmWrite(Slider2LED,LED_ON);
        pwmWrite(Slider3LED,LED_OFF);
        pwmWrite(Slider4LED,LED_OFF);
        pwmWrite(Slider5LED,LED_OFF);
        pwmWrite(Slider6LED,LED_OFF);
        break;
      case slider3key:
        //beep2();
        pwmWrite(AutoLED,LED_OFF);
        pwmWrite(Slider1LED,LED_ON);
        pwmWrite(Slider2LED,LED_ON);
        pwmWrite(Slider3LED,LED_ON);
        pwmWrite(Slider4LED,LED_OFF);
        pwmWrite(Slider5LED,LED_OFF);
        pwmWrite(Slider6LED,LED_OFF);
        break;
      case slider4key:
        //beep2();
        pwmWrite(AutoLED,LED_OFF);
        pwmWrite(Slider1LED,LED_ON);
        pwmWrite(Slider2LED,LED_ON);
        pwmWrite(Slider3LED,LED_ON);
        pwmWrite(Slider4LED,LED_ON);
        pwmWrite(Slider5LED,LED_OFF);
        pwmWrite(Slider6LED,LED_OFF);
        break;
      case slider5key:
        //beep2();
        pwmWrite(AutoLED,LED_OFF);
        pwmWrite(Slider1LED,LED_ON);
        pwmWrite(Slider2LED,LED_ON);
        pwmWrite(Slider3LED,LED_ON);
        pwmWrite(Slider4LED,LED_ON);
        pwmWrite(Slider5LED,LED_ON);
        pwmWrite(Slider6LED,LED_OFF);
        break;
      case slider6key:
        //beep2();
        pwmWrite(AutoLED,LED_OFF);
        pwmWrite(Slider1LED,LED_ON);
        pwmWrite(Slider2LED,LED_ON);
        pwmWrite(Slider3LED,LED_ON);
        pwmWrite(Slider4LED,LED_ON);
        pwmWrite(Slider5LED,LED_ON);
        pwmWrite(Slider6LED,LED_ON);
        break;
      default:
        break;
    }
}

void beep1(void)
{
  digitalWrite(beepout,LOW);
  delay(20);
  digitalWrite(beepout,HIGH);
}
void beep2(void)
{
  digitalWrite(beepout,LOW);
  delay(40);
  digitalWrite(beepout,HIGH);
}
void beep3(void)
{
  digitalWrite(beepout,LOW);
  delay(60);
  digitalWrite(beepout,HIGH);
}

void update4(unsigned long time_passed){
  static unsigned long time = 0;
  time += time_passed;
  if(time > pm25timeout)
  {
    Serial.println(time);
    pm25ledflashflag = 1;
    beep3();
    time = 0;
  }
}
 void updatepm25LED(unsigned long time_passed)
 {
    static unsigned long time = 0;
    static uint8 onoff_flag = 0 , onoff_count=0;
    if(pm25ledflashflag)
    {
      time += time_passed;
      if(time > 300)
      {
        if(onoff_count<3)
        {
          Serial.println(onoff_count);
          if(onoff_flag==0)
          {
            pwmWrite(PM25LED, LED_ON);
            //Serial.println("ON");
            onoff_flag = 1;
          }
          else
          {
            pwmWrite(PM25LED, LED_OFF);
            //Serial.println("OFF");
            onoff_flag = 1;
            onoff_count ++;
          }
          
        }
        else
        {
          onoff_count =0;
          pm25ledflashflag = 0;
        }
        time =0;
      }
      
    }
 }
