#define IntakeKey  PB12
#define CycleKey   PB13
#define AutoKey   PB14
#define Slider1Key  PB15
#define Slider2Key  PA8
#define Slider3Key  PA9
#define Slider4Key  PA10
#define Slider5Key  PA11
#define Slider6Key  PA15
#define IntakeLED  PA0
#define CycleLED  PA1
#define AutoLED   PA2
#define Slider1LED  PA3
#define Slider2LED  PA6
#define Slider3LED  PA7
#define Slider4LED  PA8
#define Slider5LED  PA9
#define Slider6LED  PA10
#define PM25LED  PB7
const uint16 LED_OFF =65535;
const uint16 LED_MID =32768;
const uint16 LED_ON =0;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
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
  pwmWrite(IntakeLED, LED_ON);   // turn the LED off (HIGH is the voltage level)
  pwmWrite(AutoLED, LED_ON);
  pwmWrite(CycleLED, LED_ON);
  pwmWrite(Slider1LED, LED_ON);
  pwmWrite(Slider2LED, LED_ON);
  pwmWrite(Slider3LED,LED_ON);
  pwmWrite(Slider4LED, LED_ON);
  pwmWrite(Slider5LED, LED_ON);
  pwmWrite(Slider6LED, LED_ON);
  pwmWrite(PM25LED, LED_ON);
  delay(3000);
  pwmWrite(IntakeLED, LED_MID);   // turn the LED off (HIGH is the voltage level)
  pwmWrite(AutoLED, LED_MID);
  pwmWrite(CycleLED, LED_MID);
  pwmWrite(Slider1LED, LED_MID);
  pwmWrite(Slider2LED, LED_MID);
  pwmWrite(Slider3LED,LED_MID);
  pwmWrite(Slider4LED, LED_MID);
  pwmWrite(Slider5LED, LED_MID);
  pwmWrite(Slider6LED, LED_MID);
  pwmWrite(PM25LED, LED_MID);
  delay(3000);
  pwmWrite(IntakeLED, LED_OFF);   // turn the LED off (HIGH is the voltage level)
  pwmWrite(AutoLED, LED_OFF);
  pwmWrite(CycleLED, LED_OFF);
  pwmWrite(Slider1LED, LED_OFF);
  pwmWrite(Slider2LED, LED_OFF);
  pwmWrite(Slider3LED,LED_OFF);
  pwmWrite(Slider4LED, LED_OFF);
  pwmWrite(Slider5LED, LED_OFF);
  pwmWrite(Slider6LED, LED_OFF);
  pwmWrite(PM25LED, LED_OFF);
}

void loop() {
  // put your main code here, to run repeatedly:

}
