 #include <LiquidCrystal_PCF8574.h>
//#include <Wire.h>        


#define upKey     PB12
#define downKey   PB13
#define leftKey   PB14
#define rightKey  PB15

/***Heater pins define******/
#define Heater1Out      PA6
#define Heater2Out      PA7
#define Heater3Out      PB0
#define Heater4Out      PB1

#define HeaterSelect1   PB10
#define HeaterSelect2   PB11

/* ADC Input For temp. sensor and pressure sensor*/
#define HeaterTemp1     PA0
#define HeaterTemp2     PA1
#define HeaterTemp3     PA2
#define HeaterTemp4     PA3

#define PressureSensor  PA4

#define PressureSelect1 PA15
//#define PressureSelect2 PB3

#define VibrationSelectLow PB3
#define VibrationSelectMiddle PB4
#define VibrationSelectHigh PB5

#define vibrationPWM    PA8
/*for read temperture 
 * 
 */
const int    SAMPLE_NUMBER      = 10;
const double BALANCE_RESISTOR   = 100000.0;
const double MAX_ADC            = 4097.0;
const double BETA               = 3950.0;
const double ROOM_TEMP          = 298.15;
const double RESISTOR_ROOM_TEMP = 100000.0;
double currentTemperature[4] = {0};
const int heaterPins[4] = {HeaterTemp1,HeaterTemp2,HeaterTemp3,HeaterTemp4};
const int heaterOutPins[4] = {Heater1Out,Heater2Out,Heater3Out,Heater4Out};
/*
 * for read pressure 
 */
const float pressuretable[5] = {
1, //off
1.92, //20N
2.063, //30N
2.150, //40N
2.199, //50N
};
int raw= 0;
float Vin= 3.3;
float Vout= 0;
long R1= 100000;
long R2= 0;
float buffer= 0;
int pressurecount=0;
#define pressuretime 5
uint8 pressuretimer=0; // count timer
uint8 pressurestate=0; //1:start 0;release
const uint16_t timertable[7]={0,5,10,15,20,25,30};
const uint16_t temptable[11]={0,25,30,35,40,45,50,55,60,65,70};
const uint16_t PWM_HIGH =65535;
const uint16_t PWM_MID =43690;
const uint16_t PWM_LOW =21845;
const uint16_t PWM_OFF =0;
/*
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
 typedef enum funcs{
    status,
    times,
    heater,
    vibration,
    pressure,
    funcs_max,
 };
 typedef enum valus{
  off,
  low,
  middle,
  high,
 };
 
 const int my_bitmap[][8] =
{
  // up arrow
  {0x04, 0x0E, 0x1F, 0x04, 0x04, 0x04, 0x00, 0x00},
  // down arrow
  {0x00, 0x00, 0x04, 0x04, 0x04, 0x1F, 0x0E, 0x04},
  // left arrow
  {0x00, 0x02, 0x06, 0x0E, 0x1E, 0x0E, 0x06, 0x02},
  // right arrow
  {0x00, 0x08, 0x0C, 0x0E, 0x0F, 0x0E, 0x0C, 0x08},
};    


int LCD1Address =0x27;
LiquidCrystal_PCF8574 lcd(LCD1Address/*0x3F*/);
//struct timer periodic_timer, arp_timer , led_timer;
void GPIO_INIT(void)
{
  /*key pad init.*/
  pinMode(upKey,INPUT_PULLUP);
  pinMode(downKey,INPUT_PULLUP);
  pinMode(leftKey,INPUT_PULLUP);
  pinMode(rightKey,INPUT_PULLUP);
  pinMode(Heater1Out,OUTPUT);
  pinMode(Heater2Out,OUTPUT);
  pinMode(Heater3Out,OUTPUT);
  pinMode(Heater4Out,OUTPUT);
  pinMode(HeaterSelect1,OUTPUT);
  pinMode(HeaterSelect2,OUTPUT);
  digitalWrite(Heater1Out,LOW);
  digitalWrite(Heater2Out,LOW);
  digitalWrite(Heater3Out,LOW);
  digitalWrite(Heater4Out,LOW);
  digitalWrite(HeaterSelect1,LOW);
  digitalWrite(HeaterSelect2,LOW);
  /* ADC Input For temp. sensor and pressure sensor*/
  pinMode(HeaterTemp1,INPUT_ANALOG);
  pinMode(HeaterTemp2,INPUT_ANALOG);
  pinMode(HeaterTemp3,INPUT_ANALOG);
  pinMode(HeaterTemp4,INPUT_ANALOG);
  pinMode(PressureSensor,INPUT_ANALOG);
  
  pinMode(PressureSelect1,OUTPUT);
  //pinMode(PressureSelect2,OUTPUT);
  digitalWrite(PressureSelect1,LOW);
  //digitalWrite(PressureSelect2,LOW);
  pinMode(vibrationPWM,PWM);
  pwmWrite(vibrationPWM,PWM_OFF);
  pinMode(VibrationSelectLow,OUTPUT);
  pinMode(VibrationSelectMiddle,OUTPUT);
  pinMode(VibrationSelectHigh,OUTPUT);
  digitalWrite(VibrationSelectLow,LOW);
  digitalWrite(VibrationSelectMiddle,LOW);
  digitalWrite(VibrationSelectHigh,LOW);
}

// the setup function runs once when you press reset or power the board
void setup() {
  int i;
  int bitmap_size = sizeof(my_bitmap) / sizeof(my_bitmap[0]);
  Serial.begin(9600);
  GPIO_INIT();
  lcd.begin(16, 2);
  // register the custom bitmaps
  for (i = 0; i < bitmap_size; i++)
  {
    lcd.createChar(i,(int *)my_bitmap[i]);
  }
  lcd.setBacklight(255);
  lcd.home();
  lcd.print("    WELLCOME    ");
  menu(status,0);
  // for (i = 0; i < 5; i++)
  //{
  //  lcd.scrollDisplayLeft();
  //  delay(500);
    
  //}
}
unsigned long previous_time;
uint8 keepkey=0xff;
uint8 keyeven=0;
uint8 keypressFlag =0;
uint8 menuindex = 0;
uint8 menukeep = 0;
uint16 timevalue=0;
uint8 heatervalue =0;
uint8 vibrationvalue=0;
uint8 pressurevalue =0;

long timecount=0;
// the loop function runs over and over again forever
void loop() {
  unsigned long current_time = millis();
  unsigned long time_passed;
  static uint ledlevel = 0;
  if(current_time > previous_time ){
    time_passed = current_time - previous_time;
  }else{
    time_passed = 0;//ULONG_MAX - previous_time + current_time;
  }

 if(time_passed >= 100){
    //beep3();
    update4(time_passed);
    if(keypressFlag && (0xff == KEY_Scan())){
      keypressFlag = 0;
    }
    previous_time = current_time;
    //Serial.println("current_time:0");
  }
  keepkey = KEY_Scan();
  if(keepkey !=0xff && keypressFlag ==0)
  {
    keyeven = keepkey;
    keypressFlag = 1;
    previous_time = current_time;
   // Serial.println("current_time:1");
    
  }
  if(keypressFlag && keyeven!=0xff ){
      if(keyeven == 0){
        if(menuindex >=1)
        menuindex = menuindex -1;
        else
        menuindex = 4;
      }else if(keyeven == 1){
        if(menuindex <4)
        menuindex = menuindex +1;
        else
        menuindex = 0;
      }
      
      if(keyeven!=0xff){
        menukeep = menuindex;
        menu(menuindex,keyeven);
      }
      keyeven = 0xff;
  } 
  /*****function action *******/
  /**timer action ******/
  if(timecount >0)
  {
    /**heater action *****/
    
    
    /**pressure action ***/
    if(pressurevalue>0)
    {
      if(pressurestate==0)
      {
        digitalWrite(PressureSelect1,HIGH);
        //Serial.println("PressureSelect1 2");
        //lcd.home();
        //lcd.setCursor(0, 1);
        //lcd.print("Pressure ON     ");
      }
      if(readpressure()>= pressuretable[pressurevalue] && pressurestate ==0)
      {
        pressuretimer = 5;
        pressurestate = 1;
      }
    }
    else
    {
      digitalWrite(PressureSelect1,LOW);
      //Serial.println("PressureSelect1 3");
      //lcd.home();
      //lcd.setCursor(0, 1);
      //lcd.print("Pressure OFF    ");
    }
  }
}



void update4(unsigned long time_passed){
  static unsigned long time = 0;
  static uint8 count=0;
  time += time_passed;
  if(time >= 1000)
  {
  
    
    
    //currentTemperature1 = readThermistor(HeaterTemp1);
    //Serial.print("Temp1:");
    //Serial.println(currentTemperature1);
    if(timecount >0){
      timecount = timecount -1;
        Serial.print("Time:");
        Serial.println(timecount);
        heatercheck();
        vibrationcheck();
        pressurecheck();
        if(menukeep == status)
        {
            lcd.setCursor(0, 1);
            if(count >=3){
             count = 0;
            }
            else
            { 
              count= count+1;
            }
            switch(count)
            {
              case 0:
                lcd.print("     RUN        ");
              break;
              case 1:
                lcd.print("     RUN.       ");
              break;
              case 2:
                lcd.print("     RUN..      ");
              break;
              case 3:
                lcd.print("     RUN...     ");
              break;
              
            }
        }
    }else
    {
       if(menukeep == status)
        {
           lcd.setCursor(0, 1);
           lcd.print("       OFF      ");
        }
        else if(menukeep == times)
        {
           lcd.setCursor(0, 1);
           lcd.print("       OFF      ");
        }
      offallfunction();
    }
    
    //if (count >=2)
    //count =0; 
    
    //Serial.println("timeout 1000");
    time = 0;
  }
}
void offallfunction(void)
{
  digitalWrite(PressureSelect1,LOW);
  digitalWrite(VibrationSelectLow,LOW);
  digitalWrite(VibrationSelectMiddle,LOW);
  digitalWrite(VibrationSelectHigh,LOW);
  for(int i=0;i<4;i++)
  {
    digitalWrite(heaterOutPins[i],LOW);
  }
}
void pressurecheck(void)
{
  if(pressurestate==1)
    {
      pressuretimer = pressuretimer -1;
      digitalWrite(PressureSelect1,HIGH);
        //lcd.home();
        //lcd.setCursor(0, 1);
        //lcd.print("pressure ON     ");
        //lcd.setCursor(12, 1);
        //lcd.print(pressuretimer);
      Serial.print("On time:");
      Serial.println(pressuretimer);
      if(pressuretimer==0)
      {
        digitalWrite(PressureSelect1,LOW);
        //Serial.println("PressureSelect1 1");
        pressurestate = 2;
        pressuretimer = 5; //reset timer to default time
      }
    }
    else if(pressurestate ==2)
    {
      pressuretimer = pressuretimer -1;
      digitalWrite(PressureSelect1,LOW);
      Serial.print("Off time:");
      Serial.println(pressuretimer);
        //lcd.home();
        //lcd.setCursor(0, 1);
        //lcd.print("pressure OFF     ");
        //lcd.setCursor(12, 1);
        //lcd.print(pressuretimer);
      if(pressuretimer==0)
      {
        digitalWrite(PressureSelect1,HIGH);
        //lcd.print(pressuretimer);
        pressurestate = 0; //to pressure start state //customer request 
        Serial.print("pressurestate:");
        Serial.println(pressurestate);
      }
      
      //lcd.home();
      //lcd.setCursor(0, 1);
      //lcd.print("Pressure OFF    ");
      //lcd.setCursor(12, 1);
      //lcd.print(pressuretimer);
    } 
}
void vibrationcheck(void)
{
  switch(vibrationvalue)
    {
      case off:
          digitalWrite(VibrationSelectLow,LOW);
          digitalWrite(VibrationSelectMiddle,LOW);
          digitalWrite(VibrationSelectHigh,LOW);
          pwmWrite(vibrationPWM,PWM_OFF);
         
      break;
      case low:
          digitalWrite(VibrationSelectLow,HIGH);
          digitalWrite(VibrationSelectMiddle,LOW);
          digitalWrite(VibrationSelectHigh,LOW);
          pwmWrite(vibrationPWM,PWM_LOW);
      break;
      case middle:
          digitalWrite(VibrationSelectLow,LOW);
          digitalWrite(VibrationSelectMiddle,HIGH);
          digitalWrite(VibrationSelectHigh,LOW);
          pwmWrite(vibrationPWM,PWM_MID);
      break;
      case high:
          digitalWrite(VibrationSelectLow,LOW);
          digitalWrite(VibrationSelectMiddle,LOW);
          digitalWrite(VibrationSelectHigh,HIGH);
          pwmWrite(vibrationPWM,PWM_HIGH);
      break;
      default:
      break;
    }
}
void heatercheck(void)
{
   /******* heater function action******/
  if(heatervalue!=0){
    for(int i= 0; i<4;i++){
      currentTemperature[i]= readThermistor(heaterPins[i]);
      //Serial.print("temp");
      //Serial.print(i);
      //Serial.print(":");
      //Serial.println(currentTemperature[i]);
      //Serial.print("heatervalue");
      //Serial.print(":");
      //Serial.println(heatervalue);
      if(currentTemperature[i]< temptable[heatervalue])
      {
        digitalWrite(heaterOutPins[i],HIGH);
        //Serial.println("HIGH");
      }
      else
      {
        digitalWrite(heaterOutPins[i],LOW);
        //Serial.println("LOW");
      }
    }
  }
}
uint8 KEY_Scan(void)
{
    if(!digitalRead(upKey))
    {
       delay(5);
       if(!digitalRead(upKey))
         { 
           return 0;
         }
     }
      if(!digitalRead(downKey))
      {
         delay(5);
         if(!digitalRead(downKey))
           { 
             return 1;
           }
       }
    if(!digitalRead(leftKey))
    {
       delay(5);
       if(!digitalRead(leftKey))
         { 
           return 2;
         }
     }
    if(!digitalRead(rightKey))
    {
       delay(5);
       if(!digitalRead(rightKey))
         { 
           return 3;
         }
     }
    return 255;
}
void menu(uint8 menu,uint keyeven)
{
  switch(menu)
  {
    case status:
      lcd.clear();
      lcd.home();
      lcd.print("     STATUS     ");
      lcd.setCursor(0, 1);
      
      lcd.print("                ");
      //lcd.print(char(0));
      //lcd.setCursor(1, 1);
      //lcd.print(char(1));
      //lcd.setCursor(2, 1);
     /// lcd.print(char(2));
      //lcd.setCursor(3, 1);
     // lcd.print(char(3));
    break;
    case times:
        Serial.print("menu:");
        Serial.println(menuindex);
        Serial.print("even:");
        Serial.println(keyeven);
      if(keyeven <=1)
      {
        
        lcd.clear();
        lcd.home();
        lcd.print("    TIME SET    ");
        
         if(timevalue==0){
          LCD_WORDSet(off);
        }
        else
        {
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(7, 1);
          lcd.print(timertable[timevalue]);
          lcd.setCursor(13, 1);
          lcd.print("Min");
        }
        
      }
      else if(keyeven >=2)
      {
        Serial.print("menu:");
        Serial.println(menuindex);
        Serial.print("even:");
        Serial.println(keyeven);
        if(keyeven==2)
        {
          if(timevalue!=0)
          {
            //timevalue=timevalue-5;
            timevalue=timevalue-1;
          }
          else
          {
            timevalue=6;
          }
        }
        else if(keyeven==3)
        {
          if(timevalue < 6)
          {
            timevalue=timevalue+1;
            //timevalue =timevalue+5;
          }
          else
          {
            timevalue=0;
          }
        }
        if(timevalue==0){
          LCD_WORDSet(off);
        }
        else
        {
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(7, 1);
          lcd.print(timertable[timevalue]);
          lcd.setCursor(13, 1);
          lcd.print("Min");
        }
         timecount = timertable[timevalue]* 60;
      }
     
    break;
    case heater:
      if(keyeven <=1)
      {
        lcd.clear();
        lcd.home();
        lcd.print("   HEATER SET  ");
        if(heatervalue==0)
        {
          LCD_WORDSet(off);
        }
        else
        {
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(7, 1);
          lcd.print(temptable[heatervalue]);
          lcd.setCursor(14, 1);
          lcd.print(char(223));
          lcd.setCursor(15, 1);
          lcd.print("C");
        }
      }
      else if(keyeven >=2)
      {
        Serial.print("menu:");
        Serial.println(menuindex);
        Serial.print("even:");
        Serial.println(keyeven);
        if(keyeven==2)
        {
          if(heatervalue!=0)
          {
            heatervalue=heatervalue-1;
          }
          else
          {
            heatervalue = 10;
          }
        }
        else if(keyeven==3)
        {
          if(heatervalue <10)
          {
            heatervalue = heatervalue + 1;
          }
          else
          {
            heatervalue = 0;
          }
        }
        if(heatervalue==0){
          LCD_WORDSet(off);
        }
        else
        {
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(7, 1);
          lcd.print(temptable[heatervalue]);
          lcd.setCursor(14, 1);
          lcd.print(char(223));
          lcd.setCursor(15, 1);
          lcd.print("C");
        }
      }
    break;
    case vibration:
    if(keyeven <=1)
      {
          lcd.clear();
          lcd.home();
          lcd.print(" Vibration Set  ");
          LCD_WORDSet(vibrationvalue);
      }
      else if(keyeven >=2)
      {
        Serial.print("menu:");
        Serial.println(menuindex);
        Serial.print("even:");
        Serial.println(keyeven);
        if(keyeven==2)
        {
          if(vibrationvalue!=0)
          {
            vibrationvalue=vibrationvalue-1;
          }
          else
          {
            vibrationvalue = 3;
          }
        }
        else if(keyeven==3)
        {
          if(vibrationvalue < high)
          vibrationvalue =vibrationvalue+1;
          else
          vibrationvalue = 0;
        }
        LCD_WORDSet(vibrationvalue);
      }
    break;
    case pressure:
    if(keyeven <=1)
      {
          lcd.clear();
          lcd.home();
          lcd.print("  Pressure Set  ");
          LCD_PressureSet(pressurevalue);
      }
      else if(keyeven >=2)
      {
        Serial.print("menu:");
        Serial.println(menuindex);
        Serial.print("even:");
        Serial.println(keyeven);
        if(keyeven==2)
        {
          if(pressurevalue!=0)
          {
            pressurevalue=pressurevalue-1;
          }
          else
          pressurevalue = 4;
        }
        else if(keyeven==3)
        {
          if(pressurevalue < 4)
          pressurevalue = pressurevalue + 1;
          else
          pressurevalue =0;
        }
        
       LCD_PressureSet(pressurevalue);
      }
    break;
    
  }
}

void LCD_WORDSet(uint8 func)
{
  switch(func)
  {
     case off:
      lcd.setCursor(0, 1);
      lcd.print("       OFF      ");
     break;
     case low:
      lcd.setCursor(0, 1);
      lcd.print("       LOW      ");
     break;
     case middle:
      lcd.setCursor(0, 1);
      lcd.print("      MIDDLE    ");
     break;
     case high:
      lcd.setCursor(0, 1);
      lcd.print("      HIGH      ");
     break;
  }
}

void LCD_PressureSet(uint8 func)
{
  switch(func)
  {
     case 0:
      lcd.setCursor(0, 1);
      lcd.print("       OFF      ");
      pressurevalue = 0;
     break;
     case 1:
      lcd.setCursor(0, 1);
      lcd.print("       20N      ");
      pressurevalue = 1;
     break;
     case 2:
      lcd.setCursor(0, 1);
      lcd.print("       30N      ");
      pressurevalue = 2;
     break;
     case 3:
      lcd.setCursor(0, 1);
      lcd.print("       40N      ");
      pressurevalue = 3;
     break;
     case 4:
      lcd.setCursor(0, 1);
      lcd.print("       50N      ");
      pressurevalue = 4;
     break;
  }
}
double readThermistor(int pin) 
{
 // variables that live in this function
 double rThermistor = 0;            // Holds thermistor resistance value
 double tKelvin     = 0;            // Holds calculated temperature
 double tCelsius    = 0;            // Hold temperature in celsius
 double adcAverage  = 0;            // Holds the average voltage measurement
 int    adcSamples[SAMPLE_NUMBER]={0};  // Array to hold each voltage measurement

 /* Calculate thermistor's average resistance:
    As mentioned in the top of the code, we will sample the ADC pin a few times
    to get a bunch of samples. A slight delay is added to properly have the
    analogRead function sample properly */
 //Serial.print("pin:");
 //Serial.println(pin);
 for (int i = 0; i < SAMPLE_NUMBER; i++) 
 {
   adcSamples[i] = analogRead(pin);  // read from pin and store
   delay(10);        // wait 10 milliseconds
 }

 /* Then, we will simply average all of those samples up for a "stiffer"
    measurement. */
 for (int i = 0; i < SAMPLE_NUMBER; i++) 
 {
   adcAverage += adcSamples[i];      // add all samples up . . .
 }
 adcAverage /= SAMPLE_NUMBER;        // . . . average it w/ divide
 //Serial.print("adcAverage:");
 //Serial.println(adcAverage);
 /* Here we calculate the thermistor’s resistance using the equation 
    discussed in the article. */
 rThermistor = BALANCE_RESISTOR * ( (MAX_ADC / adcAverage) - 1);

 /* Here is where the Beta equation is used, but it is different
    from what the article describes. Don't worry! It has been rearranged
    algebraically to give a "better" looking formula. I encourage you
    to try to manipulate the equation from the article yourself to get
    better at algebra. And if not, just use what is shown here and take it
    for granted or input the formula directly from the article, exactly
    as it is shown. Either way will work! */
 tKelvin = (BETA * ROOM_TEMP) / 
           (BETA + (ROOM_TEMP * log(rThermistor / RESISTOR_ROOM_TEMP)));

 /* I will use the units of Celsius to indicate temperature. I did this
    just so I can see the typical room temperature, which is 25 degrees
    Celsius, when I first try the program out. I prefer Fahrenheit, but
    I leave it up to you to either change this function, or create
    another function which converts between the two units. */
 tCelsius = tKelvin - 273.15;  // convert kelvin to celsius 
 Serial.print(pin);
 Serial.print(" temp:");
 Serial.println(tCelsius);
 return tCelsius;    // Return the temperature in Celsius
}
/*
const floatt pressuretable[5] = {
72300, //20N 1
59900, //30N 2
53500, //40N 3
50100, //50N 4
*/
float readpressure(void)
{
 double adcAverage  = 0;            // Holds the average voltage measurement
 int    adcSamples[SAMPLE_NUMBER]={0};
 for (int i = 0; i < SAMPLE_NUMBER; i++) 
 {
   adcSamples[i] = analogRead(PressureSensor);  // read from pin and store
   delay(10);        // wait 10 milliseconds
 }

 /* Then, we will simply average all of those samples up for a "stiffer"
    measurement. */
 for (int i = 0; i < SAMPLE_NUMBER; i++) 
 {
   adcAverage += adcSamples[i];      // add all samples up . . .
 }
 adcAverage /= SAMPLE_NUMBER;        // . . . average it w/ divide
  //raw= analogRead(PressureSensor);
  if(adcAverage) 
  {
    buffer= adcAverage * Vin;
    Vout= (buffer)/4096.0;
    /*if(Vout < 0.5)
    return 300000;
    buffer= Vout/(Vin-Vout);
    R2 = R1 *buffer;
    //R2 = R1 * ( (MAX_ADC / adcAverage) - 1);
    //buffer= adcAverage * Vin;
    //Vout= (buffer)/4096.0;
    //buffer= (Vin/Vout) -1;
    //R2= R1 * buffer;
    //Serial.print("Vout: ");
    //Serial.println(Vout);
    Serial.print("R2: ");
    Serial.println(R2);
    
    return R2;*/
    //Serial.print("Vout: ");
    //Serial.println(Vout);
    return Vout;
  }
  //Serial.println("Vout = 0");
  return 0;
}
