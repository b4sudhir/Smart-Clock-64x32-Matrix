#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>


#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
//Set Speaker Out

#include <ESP8266_Tones.h>
#define Speaker 3    //RX is connected to speaker pin GPIO3
ESP8266_Tones Mytone(Speaker);
bool hr_alm=0;
bool hr_alm_flag=0;
#define BLYNK_PRINT Serial
#include <Wire.h>
#include <TimeLib.h>
#include <DHT.h>
#include <Ticker.h>
#include <PxMatrix.h>
#include <Fonts/Org_01.h>
#include <WidgetRTC.h>

#include <Adafruit_GFX.h>
#include <FreeMonoBold12pt7b.h>

#include <kongtext4pt7b.h>
#include <TomThumb.h>
#include <Picopixel.h>
#include <Tiny3x3a2pt7b.h>
#include <FreeSansBold9pt7b.h>


#define USE_ICONS
#define USE_FIREWORKS
#define USE_WEATHER_ANI
// Local includes
#include "Digit.h"              // morphing digits

Ticker display_ticker;       // make Ticker instance for display updating

#define double_buffer
#define PI 3.1415926535897932384626433832795

// Pins for LED MATRIX..   (see schematic diagram for other connectiions on the matrix panel)
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_D 12
//#define P_E 0
#define P_OE 2

PxMATRIX display(64, 32, P_LAT, P_OE, P_A, P_B, P_C, P_D);   // make an instance of the Matrix display, E not required.

// RTC config
//#define  RTC_SCL             5                       // RTC - SCL chip select  -  D1 GPIO5
//#define  RTC_SDA             4                       // RTC - SDA clock        -  D2 GPIO4
//RTC_DS1307 RTC;                                    // make instance of the Real Time Clock

WidgetRTC rtc;

// DHT11 temp & humidity config
#define DHTTYPE DHT11                          // DHT uses DHT 11
#define DHTPIN 0                      // DHT uses pin GPIO0 D3     

DHT dht(DHTPIN, DHTTYPE);                  //make an instance of the DHT11

// brightness
int cin = 30;                                              // LED intensity as a percentage
const int LDR = A0; // Defining LDR PIN 
int input_val = 0;  // Varible to store Input values

// Morphing clock digits 
int bred=90;
int bblue=90;
int bgreen=90;

Digit  digit0(&display, 0, 63 - 1 - 9 * 1, 4, display.color565(bred, bgreen, bblue));
Digit  digit1(&display, 0, 63 - 1 - 9 * 2, 4, display.color565(bred, bgreen, bblue));
Digit  digit2(&display, 0, 63 - 4 - 9 * 3, 4, display.color565(bred, bgreen, bblue));
Digit  digit3(&display, 0, 63 - 4 - 9 * 4, 4, display.color565(bred, bgreen, bblue));
Digit  digit4(&display, 0, 63 - 7 - 9 * 5, 4, display.color565(bred, bgreen, bblue));
Digit  digit5(&display, 0, 63 - 7 - 9 * 6, 4, display.color565(bred, bgreen, bblue));

Digit digit00(&display, 0, 63 - 1 - 9 * 1, 8, display.color565(bred, bgreen, bblue));
Digit digit01(&display, 0, 63 - 1 - 9 * 2, 8, display.color565(bred, bgreen, bblue));
Digit digit02(&display, 0, 63 - 4 - 9 * 3, 8, display.color565(bred, bgreen, bblue));
Digit digit03(&display, 0, 63 - 4 - 9 * 4, 8, display.color565(bred, bgreen, bblue));
Digit digit04(&display, 0, 63 - 7 - 9 * 5, 8, display.color565(bred, bgreen, bblue));
Digit digit05(&display, 0, 63 - 7 - 9 * 6, 8, display.color565(bred, bgreen, bblue));

// Color defines RGB
int red = display.color565 (255, 0, 0);
int orange = display.color565 (255, 110, 0);
int yellow = display.color565 (255, 255, 0);
int green = display.color565 (0, 255, 0);
int blue = display.color565 (0, 0, 255);
int magenta = display.color565 (255, 0, 255);
int cyan = display.color565 (0, 250, 150);
int white = display.color565 (255, 255, 255);
int blck = display.color565 (0, 0, 0);

#include "TinyFont.h"
const byte row0 = 2+0*10;
const byte row1 = 2+1*10;
const byte row2 = 2+2*10;

//========================================
// Some standard colors
uint16_t myRED = display.color565(cin, 0, 0);
uint16_t myGREEN = display.color565(0, cin, 0);
uint16_t myBLUE = display.color565(0, 0, cin);
uint16_t myWHITE = display.color565(cin, cin, cin);
uint16_t myYELLOW = display.color565(cin, cin, 0);
uint16_t myCYAN = display.color565(0, cin, cin);
uint16_t myMAGENTA = display.color565(cin, 0, cin);
uint16_t myBLACK = display.color565(cin, cin, cin);

uint16_t myCOLORS[8]={myRED,myGREEN,myBLUE,myWHITE,myYELLOW,myCYAN,myMAGENTA,myBLACK};



//==================================================================//////////////////////////////////////////////////////////////////////////////

#define matrix_width 64
#define matrix_height 32

int dst = 0;
  float Humi;
  float Temp;
 static uint32_t lastTime = 0; // millis() memory
static bool flasher = false;  // seconds passing flasher
uint8_t frameDelay = 10;  // default frame delay value

int m,s,d;
uint8_t dow;
int aday;
uint8_t amonth;
String  ayear;
String adate;
String WeatherT;
String WeatherH;
String  text;
#define MAX_MESG  9
#define MAX_MES  20
#define MAX_ME  50
#define BUF_SIZE  612
char curMessage[BUF_SIZE] = { "Hello! Enter new message?" };
char newMessage[BUF_SIZE];
bool newMessageAvailable = false;
static bool Mode = true; 
uint8_t r=0, g=0, b=0;
unsigned int NewRTCh = 24;
unsigned int NewRTCm = 60;
unsigned int NewRTCs = 10;
char szTime[4];    // 00
char szMesg[10] = "";
  char  szBuf[10];



///////////////////////////////////////////////////////////////////////////////////////////////=====================================================


void getWeather();
void RefreshDigitalTime(); 
// Arrays for months and days
String Months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}; //char*
String Days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

 // saves for blanking
unsigned int prev_second, prev_Timer_second, prev_dayOfTheWeek, prev_day, prev_month, prev_year;   

//morphing clock variables
unsigned int s0, s1, m0, m1, h0, h1;        

float timeZone = 5.5;     //  ......QLD AU time is +10              ************ CHANGE TO SUIT TIME ZONE ***************

int scroll_val=5;

// weather  variables
String Wstr;
int Wstr_len=0;
int scroll_Y, old_scroll_Y;
//unsigned long last = 0;           // scroll timer variables
unsigned long cm;                  // scroll timer variables
int h, t, oldh, oldt, olds, oldMin;                    //indoor temp/humidity sensor
float temp = -10000;                  // openweather variables
int pressure = -10000;
float humidity = -10000;
int wind_speed = -10000;
int wind_deg = -10000;
String dir;                               // wind direction
unsigned long sun_rise, sun_set;

int tempMin = -10000;
int tempMax = -10000;
int tempM = -10000;
int presM = -10000;
int humiM = -10000;
int condM = -1;  //-1 - undefined, 0 - unk, 1 - sunny, 2 - cloudy, 3 - overcast, 4 - rainy, 5 - thunders, 6 - snow
String condS = "";

char military[3] = "Y";     // 24 hour mode? Y/N
char date_fmt[7] = "D.M.Y"; // date format: D.M.Y or M.D.Y or M.D or D.M or D/M/Y.. looking for trouble


int condition = 0;  // 0 - unk, 1 - sunny, 2 - cloudy, 3 - overcast, 4 - rainy, 5 - thunders, 6 - snow
String apiKey   = "dcfe2ba91766643eb4c8cd4611649739"; // e.g a hex string like "abcdef0123456789abcdef0123456789"   open weather map api key
String location = "Pune, IN"; //e.g. "Paris, FR"  the city you want the weather for ** SET THIS TO YOUR LOCAL WEATHER STATION *****************************************************
char server[]   = "api.openweathermap.org";

// timer variables
int timer_Seconds, timer_Minutes, timer_Hours;      

// date variables
String ddstr, mmstr, yystr, allstr;

String corona="";

int hh;
int mm;
int ss;
int ntpsync = 1;

int posx=0;
int posy=0;


int prev_time=0;
int prevhh;
int prevmm;
int prevss;
int prevday;


// Flags
bool autobright = false; // for auto bright
bool RunDigital_Flag = true;
bool RunFace1_Flag=false;
bool RunFace2_Flag=false;
bool RunFace3_Flag=false;
bool RunFace4_Flag=false;
bool RunAnalog_Flag = false;
bool RunWeather_Flag = false;
bool first_time = true;                     // first time to weather weather flag after is every 5minutes 30 secs
bool RunTimer_Flag = false;
bool dateprint_flag=false;     //for print date and time
bool loveprint_flag=false;     // for print love symbol
bool loveprt_1t_flag=false;     // for print love symbol only one time
bool fire_flag=false;     // for print love symbol
bool instant_fire=false;     // for print love symbol
bool prt=false;
bool inst_fire_ft=false;     //for print all mater after fire
bool inst_fire_bt=false;     //forsaving button state of fire
bool inout_prt = false;     //weather print status for inside or outside in Face 2 
bool w_face2 = false;     //weatherface2 only one time in  a second 
bool corona_update_flag=false;
bool exit_function=false;  // for exit from function return function;
bool weather_face3=false;  // for weather on feature in face 3 scrolling face;
bool pageload_f4 = false; // for pageload f4
bool pageload_f4_1t = false; // for pageload f4 loading only one time


int olddate=0;// olddate for checking new old date
int bright_val=35;



// variables for analog clock
int handY, handX, Analog_hour;     // point on circle for hands
int centerX =  15;      // center of clock
int centerY = 15;
int ssRad = 15;         // second hand radius
int mmRad = 15;       // minute hand radius
int hhRad = 10;           // hour hand radius
int hhMark = 15;        // hour markers radius
float handAngle;        // 0 deg points East, 90 deg point South, 180 deg point West, 270 deg point north

// Set password for network.
char ssid[] = "Sudkita";                                                // local wifi network SSID
char pass[] = "12345677";                                                      // local network password
char auth[] = "f-2189tghOXnLcuOK_4rBM3uxFUfPBFQ";           // Blynk authority token

 // weather client
WiFiClient client;                                                                    // make an instance of the weather client                   


WidgetTerminal terminal(V6);                                            //make instance of app 'terminal' widget, used for report NTP time setting & weather on app

//*******************************************************************************
// ISR for display refresh
void display_updater() {
  display.display(40);
}


//*******************************************************************************
// Blynk Terminal for message

 BLYNK_WRITE(V6)//v6 set to control Terminal Print.
  {
      terminal.clear();
      // if you type "IP" into Terminal Widget - it will respond: "IP ADDRESS:"
      if (String("ip") == param.asStr()||String("IP") == param.asStr()) 
      {
              terminal.println("IP Address : ") ;
              terminal.println(WiFi.localIP());
              corona_update_flag=false;
              weather_face3=false;

      } else if(String("Corona") == param.asStr()||String("corona") == param.asStr())
      {

        weather_face3=false;
        corona_update_flag=true;
        corona_update();
        memset(newMessage, 0, sizeof newMessage);

        for (int i = 0; i < corona.length(); i++) { 
            newMessage[i] = corona.charAt(i); 
        } 
         newMessageAvailable=true; 
        //strcpy(newMessage,corona.asStr());
        terminal.print("Message on print : ");
        terminal.println(newMessage);
        exit_function=true;

      }else if(String("Weather") == param.asStr()||String("weather") == param.asStr())
      {
        weather_update();
        corona_update_flag=false;
        weather_face3=true;
         newMessageAvailable=true;
        weather_print();

         memset(newMessage, 0, sizeof newMessage);
        
        for (int i = 0; i < Wstr.length(); i++) { 
            newMessage[i] = Wstr.charAt(i); 
        } 
        //strcpy(newMessage,corona.asStr());
        //terminal.clear();
        exit_function=true;
      }

      else 
      {
        strcpy(newMessage,param.asStr());
        weather_face3=false;
        corona_update_flag=false;
         newMessageAvailable=true; 
        terminal.print("Message on print : ");
        terminal.println(newMessage);
        exit_function=true;
      }
          // Ensure everything is sent
          terminal.flush();
  }
//*******************************************************************************
// Blynk run the digital clock button
BLYNK_WRITE(V0) {
  Prep (0, param.asInt());
  display.setFont();
  RefreshDigitalTime();
}

//*******************************************************************************
// Blynk run analog clock button
BLYNK_WRITE(V1) {
  Prep (1, param.asInt());
  display.setFont();
  DrawAnalogDate();
}

//*******************************************************************************
// 12 24 hrs
BLYNK_WRITE(V2) 
{ 
  if(param.asInt()) 
  {
    military[0] ='Y';
  }
  else
  {
    military[0] = 'N';
  }

   if(RunDigital_Flag) 
   { 
      Blynk.virtualWrite (V0, HIGH); 
      Blynk.virtualWrite(V3, LOW); 
      Blynk.virtualWrite(V16, LOW); 
      Blynk.virtualWrite(V17, LOW); 
      Blynk.virtualWrite(V19, LOW); 
      Prep (0, 1);
    }
  if(RunFace1_Flag)
  {  
    Blynk.virtualWrite(V3, HIGH);
    Blynk.virtualWrite(V0, LOW);
    Blynk.virtualWrite(V16, LOW); 
    Blynk.virtualWrite(V17, LOW); 
    Blynk.virtualWrite(V19, LOW); 
    Prep (3, 1);
    ntpsync=1;
    Face1();
   }
  if(RunFace2_Flag)
  {  
    Blynk.virtualWrite(V16, HIGH); 
    Blynk.virtualWrite(V3,  LOW);
    Blynk.virtualWrite(V0,  LOW);
    Blynk.virtualWrite(V17, LOW); 
    Blynk.virtualWrite(V19, LOW); 
    Prep (2, 1);
    ntpsync=1;
    Face2();
   }
    if(RunFace3_Flag)
  {  
    Blynk.virtualWrite(V17, HIGH); 
    Blynk.virtualWrite(V3,  LOW);
    Blynk.virtualWrite(V0,  LOW);
    Blynk.virtualWrite(V16, LOW); 
    Blynk.virtualWrite(V19, LOW); 
    Prep (5, 1);
    Mode = true;
    Face3();
   }
   if(RunFace4_Flag)
  {  
    Blynk.virtualWrite(V19, HIGH); 
    Blynk.virtualWrite(V17, LOW); 
    Blynk.virtualWrite(V3,  LOW);
    Blynk.virtualWrite(V0,  LOW);
    Blynk.virtualWrite(V16, LOW); 
    Prep (6, 1);
    pageload_f4=true;
    Face4();
   }

   
}

BLYNK_WRITE(V3) {
  Prep (3, param.asInt());
  ntpsync=1;
  Face1();}

//love Print button
BLYNK_WRITE(V10) {
  loveprint_flag=param.asInt();
}


//red slider
BLYNK_WRITE(V11) {
  bred=param.asInt();
  Adjust_Intensity();
}
//blue slider
BLYNK_WRITE(V12) {
  bblue=param.asInt();
  Adjust_Intensity();
}
//green slider
BLYNK_WRITE(V13) {
  bgreen=param.asInt();
  Adjust_Intensity();
}

//Hourly Alarm
BLYNK_WRITE(V14) 
{ 
  if(param.asInt()) 
  {
    hr_alm=1;
    fire_flag=1;
  }
  else
  {
    hr_alm=0;
    fire_flag=0;
  }
}


//Instatnt Fire On or off
BLYNK_WRITE(V15) 
{ 
  if(param.asInt())
  {
      instant_fire=true;
      inst_fire_bt=true;
  }
  else
  {
    instant_fire=false;
    inst_fire_bt=false;
    ntpsync=1;
    pageload_f4=true;
  }
}


BLYNK_WRITE(V16) {
  Prep (2, param.asInt());
  ntpsync=1;
  Face2();}


BLYNK_WRITE(V17) {
  NewRTCh = 24;NewRTCm = 60;NewRTCs = 10;
  Prep (5, param.asInt());
  olddate=0;
  corona_update_flag=true;
  Face3();}

  BLYNK_WRITE(V18) {
  autobright = param.asInt();
  if(!autobright)
    {
            cin = bright_val;
           Adjust_Intensity();
    }
  }


BLYNK_WRITE(V19) {
  Prep (6, param.asInt());
  pageload_f4=true;
  Face4();}



//*******************************************************************************
// Blynk set the clock NTP time button

//*******************************************************************************
// Blynk Timer button
BLYNK_WRITE(V4) {
  Prep (4, param.asInt());
  display.setFont();
  display.setTextColor (green);
  display.setCursor (17, 0);
  display.print ("TIMER");
  Blynk.syncVirtual(V7, V8, V9);            // get values from server
  
  digit0.Draw(timer_Seconds % 10);     //draw clock 
  digit1.Draw(timer_Seconds / 10);
  digit2.Draw(timer_Minutes % 10);
  digit3.Draw(timer_Minutes / 10);
  digit4.Draw(timer_Hours % 10);
  digit5.Draw(timer_Hours / 10);
  
  RunTimer_Flag = true;                         // start the timer
}

//********************************************************************************
// set panel brightness from Blynk slider
BLYNK_WRITE(V5) {
  bright_val=param.asInt();
  if(autobright==false)
  {
      cin = param.asInt();
      Adjust_Intensity();
  }
}

//********************************************************************************
// Blynk timer settings
BLYNK_WRITE(V7) {
  timer_Seconds = param.asInt();
}
BLYNK_WRITE(V8) {
  timer_Minutes = param.asInt();
}
BLYNK_WRITE(V9) {
  timer_Hours = param.asInt();
}

//********************************************************************************


//*****************************************************************************
// preperations when clocks, weather & timer buttons pressed
void Prep (int v, int par) {
  RunDigital_Flag = false;
  RunAnalog_Flag = false;
  RunFace1_Flag = false;
  RunTimer_Flag = false;
  RunFace2_Flag = false;
  RunFace3_Flag = false;
  RunFace4_Flag = false;

  Blynk.virtualWrite (V0, LOW);           //switch off 'digital'  button
  Blynk.virtualWrite (V1, LOW);           //switch off 'analog' button
  Blynk.virtualWrite (V3, LOW);           // switch off 'set time' button
  Blynk.virtualWrite (V4, LOW);           // switch off 'timer' button
  Blynk.virtualWrite (V16, LOW);           // switch off 'timer' button      
  Blynk.virtualWrite (V17, LOW);           // switch off 'timer' button   
  Blynk.virtualWrite (V19, LOW);           // switch off 'timer' button      
  switch (v) {
    case 0:
       RunDigital_Flag = true;
       if (par == 1) {
            Blynk.virtualWrite (V0, HIGH); 
            terminal.clear();
       }
       break;
    case 1:
       RunAnalog_Flag = true;
       if (par == 1) {
            Blynk.virtualWrite (V1, HIGH); 
            terminal.clear();
       }
       break;
    case 2:
       RunFace2_Flag = true;
       if (par == 1) {Blynk.virtualWrite (V16, HIGH);terminal.clear();} 
       break;          
   case 3:
       RunFace1_Flag = true;
       Blynk.virtualWrite (V3, HIGH);                         // NTP  flag done in button routine
       break;      
   case 4:
       if (par == 1) Blynk.virtualWrite (V4, HIGH);     // Timer flag done in button routine
       break;    
   case 5:
        RunFace3_Flag = true;
       if (par == 1) Blynk.virtualWrite (V17, HIGH);     // Timer flag done in button routine
       break;    
   case 6:
        RunFace4_Flag = true;
       if (par == 1) Blynk.virtualWrite (V19, HIGH);     // Timer flag done in button routine
       break; 
   }
  
  if (par == 0)  display_ticker.detach(); else  display_ticker.attach(0.002, display_updater);    //turn off display if button off, only blanks display if ALL buttons are off
  display.fillScreen (blck);      // blank the screen
  Adjust_Intensity();
}

//********************************************************************************
// display and run the timer
void Display_Timer() {
  if (timer_Seconds != 0 || timer_Minutes != 0 || timer_Hours != 0) {        // if not timed out
    if (prev_Timer_second != second()) {                                           // and it's a new second
    
      display.fillRect(2, 13, 59, 15, blck);        
      DrawColons();                                             
      timer_Seconds -- ;
      if (timer_Seconds < 0 && timer_Minutes > 0 || timer_Seconds < 0 && timer_Hours > 0) {
        timer_Seconds = 59;
        timer_Minutes -- ;
      }
      if (timer_Minutes < 0  && timer_Hours == 0) timer_Minutes = 0;
      if (timer_Minutes < 0  && timer_Hours > 0) {
        timer_Minutes = 59;
        timer_Hours -- ;
      }

      digit0.Draw(timer_Seconds % 10);                                                     //digit0.Draw(timer_Seconds % 10);    draw clock without morphing
      digit1.Draw(timer_Seconds / 10);
      digit2.Draw(timer_Minutes % 10);
      digit3.Draw(timer_Minutes / 10);
      digit4.Draw(timer_Hours % 10);
      digit5.Draw(timer_Hours / 10);
    }                                                                                                           // end  if (prev_Timer_second
    prev_Timer_second = second();
  }                                                                                                             // end if (timer_Seconds
  else 
  {
    display.fillRect(0, 0, 63, 8, blck);
    display.setTextColor (red);
    display.setCursor (2, 0);
    display.print ("TIMER DONE");
    Blynk.virtualWrite (V4, LOW);
                                                               // switch off 'timer' button
    
  }
}


void alarm()
{
  if(minute()==00)
  {
    if(hr_alm_flag==1)
    {
        int as=hour();
        if(as>22 || as < 8) return;
        if(as >12)as-=12;
        while(as>0)
         {
            Serial.println (as);
            Mytone.Bleep(Speaker); 
            delay(30);
            Mytone.Bleep(Speaker); 
            delay(5);
            Mytone.Bleep(Speaker); 
            delay(10);
            Mytone.Bleep(Speaker); 
            delay(500);
            as--;
         }
         hr_alm_flag=0;
    }
    
  }
  else hr_alm_flag=1;
}



//*******************************************************************************
// Run display sequence of date, analog clock, temp & humidity
void DisplayAnalog() {
 
  if (day() != prev_day) {                                                 // date
      DrawAnalogDate();
  }
  DrawHand(34, 1, 29,  90, orange);
  
  if (second() != prev_second) {                                     // time
      display.fillCircle(centerX, centerY, ssRad + 1, blck);      // blank the analog clock area HANDS only. Note trying to overwrite the hands in black didn't work, remnants were left behind.
      handAngle = (second() * 6) - 90;                              //draw the second hand. Note angle 0 degree faces East
      DrawHand(centerX, centerY, ssRad,  handAngle, red);
      //DrawHand(centerX, centerY+1, ssRad,  handAngle, red);
      handAngle = (minute() * 6) - 90;                               //draw the minute hand
      DrawHand(centerX, centerY, mmRad,  handAngle, green);
      DrawHand(centerX, centerY+1, mmRad,  handAngle, green);
      if (hour() > 11) Analog_hour = hour() - 12; else  Analog_hour = hour();         //set hour to 12 hour system, 0 - 11 for analog clock
      handAngle = (Analog_hour * 30) + ((minute() / 12) * 6) - 90;                                         //get to correct hand position, i.e.  2:24  is at 12 min mark to simulate real clock
      DrawHand(centerX, centerY, hhRad,  handAngle, blue);
      DrawHand(centerX, centerY+1, hhRad,  handAngle, blue);                                                      //draw the hour hand
    }
  
  prev_day = day();                                                            // save
  prev_second = second();
}

// ***********************************************************************
// Draw the analog hour marks
void drawHourMarks() {
  for ( int x = 0; x < 360; x += 30) {                                                //draw the hour markers every 30 degrees
    handY = round(centerY + hhMark * sin( x * (PI / 180) ) );
    handX = round(centerX + hhMark * cos( x * (PI / 180) ) );
    display.drawPixel (handX, handY, orange);
  }
}

// ***********************************************************************
// Draw the analog clock hands
void DrawHand( int cx, int cy, int r, int a, int c) {
  int x = round( cx + r * cos( a * (PI / 180) ) );
  int y = round( cy + r * sin( a * (PI / 180) ) );
  display.drawLine (cx, cy, x, y, c);
}

// ***********************************************************************
// Draw the analog clock date, day & month
void DrawAnalogDate() {
      
      display.fillRect(38, 0, 18, 32, blck);                   // if the date changes blank out the date
      allstr = String(Days [weekday()-1]);                                            // print day
      display.setTextColor (blue);
      display.setCursor (39, 0);
      display.print (allstr);
      allstr = String(Months [month() - 1]);                                                    // print month
      display.setCursor (39, 24);
      display.print (allstr);
    
      Digit digit0(&display, 0, 48, 9, display.color565(0, 0, 255 * cin / 255));        //print date
      Digit digit1(&display, 0, 39, 9, display.color565(0, 0, 255 * cin / 255));
      digit0.Draw(day() % 10);                                                                      //draw date without morphing
      digit1.Draw(day() / 10);
}

//*******************************************************************************
// Refresh the digital time after blanking

//*******************************************************************************
// draw colons for digital clock
void DrawColons() {
  digit1.DrawColon(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
  digit3.DrawColon(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
}

//*****************************************************************************
// Adjust brightness/intensity of entire display
void Adjust_Intensity() {
  red = display.color565 (255 * cin / 255, 0, 0);
  orange = display.color565 (255 * cin / 255, 110 * cin / 255, 0);
  yellow = display.color565 (255 * cin / 255, 255 * cin / 255, 0);
  green = display.color565 (0, 255 * cin / 255, 0);
  blue = display.color565 (0, 0, 255 * cin / 255);
  magenta = display.color565 (255 * cin / 255, 0, 255 * cin / 255);
  cyan = display.color565 (0, 255 * cin / 255, 150 * cin / 255);
  white = display.color565 (255 * cin / 255, 255 * cin / 255, 255 * cin / 255);
  
if (RunDigital_Flag) {
      digit0.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit1.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit2.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit3.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit4.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit5.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      RefreshDigitalTime();
    }
  if (RunAnalog_Flag) {
    drawHourMarks();
    DrawAnalogDate();
  }

  if (RunFace1_Flag) {
      digit00.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit01.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit02.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit03.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit04.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit05.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      draw_date ();
      draw_weather ();
      ntpsync=1;
    }
      if (RunFace2_Flag) {
      digit00.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit01.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit02.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit03.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit04.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      digit05.SetColor(display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
      draw_date ();
      scroll_draw_weather ();
      ntpsync=1;
    }

      if (RunFace3_Flag) 
        {
                NewRTCh = 24;
                NewRTCm = 60;
                NewRTCs = 10;
                olddate = 0;
                Mode=true;
                exit_function=true;
        }
        if (RunFace4_Flag) 
        {
                pageload_f4=true;
        }
}

//*****************************************************************************
// for scrolling the weather text up the screen
void Scroll() {
    /*display.setTextColor (blck);
    display.setCursor (old_scroll_Y, 0);
    display.print (Wstr);                                      // blank out Wstr on previous line
    display.setTextColor (green);
    display.setCursor (scroll_Y, 0);
    display.print (Wstr);                                       // print Wstr on new line*/

    
}


BLYNK_CONNECTED() {
  // Synchronize time on connection
               rtc.begin();
                   Blynk.syncVirtual(V10);    //draw love
                   Blynk.syncVirtual(V2);    //12 24 hrs
                   //Blynk.syncVirtual(V0);       // digital on  
                   Blynk.syncVirtual(V5);       // Brightness Value CIN  
                   Blynk.syncVirtual(V11);   // color red 
                   Blynk.syncVirtual(V12);  // color green
                   Blynk.syncVirtual(V13);// color blue
                   Blynk.syncVirtual(V14);   //  Hr alarm on off
                   Blynk.syncVirtual(V15);   // Fire On Off on off
                    Blynk.syncVirtual(V18);   // Auto Brightness
              
}

          

// *********************************************************************************
// Setup
void setup() {
  Serial.begin(115200);
  display.begin(16);
  dht.begin();
  display_ticker.attach(0.002, display_updater);   // timer interrupt for the display refresh
                                                                              //** everything marked ** can be deleted or commented for a faster boot up**********************
  display.setFont(&Org_01);                                 //** set font to 5x6
  display.setTextColor (blue);                                //**
  display.setCursor (4, 18);                                   //**
  display.println ("Matrix Clock");                           //** splash
  display.drawRect(1, 11, 62, 11, red);                      //**
  delay(1000);                                                        //** delay to admire splash
  display.fillScreen (blck);                                      //** blank the screen
  display.setCursor (0, 4);                                     //**
  display.fillScreen (0);
  Serial.print ("display color range [");
  Serial.print (display.color565 (0, 0, 0));
  Serial.print (" .. ");
  Serial.print (display.color565 (255, 255, 255));
  Serial.println ("]");
  Blynk.begin(auth, ssid, pass, "blynkservergcp.ddns.net", 8080);   //RPi local server port 8080 ** CHANGE TO SUIT YOUR SERVER
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)
  RunDigital_Flag=true;
  Prep (0, 1);                                                         //prepare for digital clock
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));  // display Blynk library version on app
  terminal.println();
  terminal.flush();

  randomSeed(analogRead(5));                         // randomise for whatever purpose
  getWeather ();
}

// **************************************************************************************
// Loop
void loop() {
  int k = analogRead(LDR);
  if(analogRead(LDR)>80)
  input_val = map(analogRead(LDR), 0, 100, 23, 255);      // Reading Input
if(second()%10==0)
{
  Serial.print("autobright= "); 
  Serial.println(autobright);
  Serial.print("val= ");
  Serial.println(input_val);
  Serial.print("LDR= ");
  Serial.println(analogRead(LDR));
 } 
    if(autobright)
    {
      if(input_val!=cin)
      {
        if((input_val-cin)>50 || (input_val-cin) < -50 )
        {
           cin = input_val;
           Adjust_Intensity();
        }

      }
    }

  if (!Blynk.connected()) 
  {
      Serial.println("Blynk Disconnected");
      Blynk.connect();
      if (Blynk.connect()) {
          Serial.println("Reconnected");
      }
      else 
      {
         Serial.println("Not reconnected");
      }
  }
  
  Blynk.run();
  
  if (hr_alm) alarm();
  if (RunDigital_Flag) DisplayDigital();
  if (RunAnalog_Flag) DisplayAnalog();
  if (RunTimer_Flag) Display_Timer();
  if(RunFace1_Flag)  Face1(); 
  if(RunFace2_Flag)  Face2(); 
  if(RunFace3_Flag)  Face3(); 
  if(RunFace4_Flag)  Face4();
}


//*********************************************************************************
// convert wind direction to standard N, NNW,NW, W etc
String WindDirection(unsigned int wd) {
  if (wd >= 0 && wd <= 11) dir = "N";
  if (wd >= 12 && wd <= 34) dir = "NNE";
  if (wd >= 35 && wd <= 56) dir = "NE";
  if (wd >= 57 && wd <= 78) dir = "ENE";
  if (wd >= 79 && wd <= 101) dir = "E";
  if (wd >= 102 && wd <= 124) dir = "ESE";
  if (wd >= 125 && wd <= 146) dir = "SE";
  if (wd >= 147 && wd <= 168) dir = "SSE";
  if (wd >= 169 && wd <= 191) dir = "S";
  if (wd >= 192 && wd <= 204) dir = "SSW";
  if (wd >= 205 && wd <= 226) dir = "SW";
  if (wd >= 227 && wd <= 259) dir = "WSW";
  if (wd >= 260 && wd <= 281) dir = "W";
  if (wd >= 282 && wd <= 304) dir = "WNW";
  if (wd >= 305 && wd <= 326) dir = "NW";
  if (wd >= 327 && wd <= 348) dir = "NNW";
  if (wd >= 349 && wd <= 360) dir = "N";
  return dir;                                                           //return direction
}

//***********************************************************************
// get weather

void getWeather ()
{
  if (!apiKey.length ())
  {
    Serial.println ("w:missing API KEY for weather data, skipping"); 
    terminal.clear();
    terminal.println ("Missing API KEY for weather data, skipping"); 
    return;
  }
  Serial.print ("i:connecting to weather server.. "); 
  // if you get a connection, report back via serial: 
  if (client.connect (server, 80))
  { 
    Serial.println ("connected."); 
    // Make a HTTP request: 
    client.print ("GET /data/2.5/weather?"); 
    client.print ("q="+location); 
    client.print ("&appid="+apiKey); 
    client.print ("&cnt=1"); 
    client.println ("&units=metric");
    client.println ("Host: api.openweathermap.org"); 
    client.println ("Connection: close");
    client.println (); 
  } 
  else 
  { 
    Serial.println ("w:unable to connect");
    return;
  } 
  delay (1000);
  String sval = "";
  int bT, bT2;
  //do your best
  String line = client.readStringUntil ('\n');
  if (!line.length ())
    Serial.println ("w:unable to retrieve weather data");
  else
  {
    Serial.print ("weather:"); 
    Serial.println (line); 
    //weather conditions - "main":"Clear",
    bT = line.indexOf ("\"main\":\"");
    if (bT > 0)
    {
      bT2 = line.indexOf ("\",\"", bT + 8);
      sval = line.substring (bT + 8, bT2);
      Serial.print ("cond ");
      Serial.println (sval);
      //0 - unk, 1 - sunny, 2 - cloudy, 3 - overcast, 4 - rainy, 5 - thunders, 6 - snow
      condM = 0;
      if (sval.equals("Clear"))
        condM = 1;
      else if (sval.equals("Clouds"))
        condM = 2;
      else if (sval.equals("Overcast"))
        condM = 3;
      else if (sval.equals("Rain"))
        condM = 4;
      else if (sval.equals("Drizzle"))
        condM = 4;
      else if (sval.equals("Thunderstorm"))
        condM = 5;
      else if (sval.equals("Snow"))
        condM = 6;
      //
      condS = sval;
      Serial.print ("condM ");
      Serial.println (condM);
    }
    //tempM
    bT = line.indexOf ("\"temp\":");
    if (bT > 0)
    {
      bT2 = line.indexOf (",\"", bT + 7);
      sval = line.substring (bT + 7, bT2);
      Serial.print ("temp: ");
      Serial.println (sval);
      tempM = sval.toInt ();
    }
    else
      Serial.println ("temp NOT found!");
    //tempMin
    bT = line.indexOf ("\"temp_min\":");
    if (bT > 0)
    {
      bT2 = line.indexOf (",\"", bT + 11);
      sval = line.substring (bT + 11, bT2);
      Serial.print ("temp min: ");
      Serial.println (sval);
      tempMin = sval.toInt ();
    }
    else
      Serial.println ("temp_min NOT found!");
    //tempMax
    bT = line.indexOf ("\"temp_max\":");
    if (bT > 0)
    {
      bT2 = line.indexOf ("},", bT + 11);
      sval = line.substring (bT + 11, bT2);
      Serial.print ("temp max: ");
      Serial.println (sval);
      tempMax = sval.toInt ();
    }
    else
      Serial.println ("temp_max NOT found!");
    //pressM
    bT = line.indexOf ("\"pressure\":");
    if (bT > 0)
    {
      bT2 = line.indexOf (",\"", bT + 11);
      sval = line.substring (bT + 11, bT2);
      Serial.print ("press ");
      Serial.println (sval);
      presM = sval.toInt();
    }
    else
      Serial.println ("pressure NOT found!");
    //humiM
    bT = line.indexOf ("\"humidity\":");
    if (bT > 0)
    {
      bT2 = line.indexOf (",\"", bT + 11);
      sval = line.substring (bT + 11, bT2);
      Serial.print ("humi ");
      Serial.println (sval);
      humiM = sval.toInt();
    }
    else
      Serial.println ("humidity NOT found!");
  }//connected
}


#include "TinyIcons.h"
//icons 10x5: 10 cols, 5 rows
int moony_ico [50] = {
  //3 nuances: 0x18c3 < 0x3186 < 0x4a49
  0x0000, 0x4a49, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18c3,
  0x0000, 0x0000, 0x0000, 0x4a49, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4a49, 0x4a49, 0x3186, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4a49, 0x3186, 0x3186, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000,
};

#ifdef USE_WEATHER_ANI
int moony1_ico [50] = {
  //3 nuances: 0x18c3 < 0x3186 < 0x4a49
  0x0000, 0x18c3, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4a49,
  0x0000, 0x0000, 0x0000, 0x4a49, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4a49, 0x4a49, 0x3186, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4a49, 0x3186, 0x3186, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000,
};

int moony2_ico [50] = {
  //3 nuances: 0x18c3 < 0x3186 < 0x4a49
  0x0000, 0x3186, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3186,
  0x0000, 0x0000, 0x0000, 0x4a49, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4a49, 0x4a49, 0x3186, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4a49, 0x3186, 0x3186, 0x3186, 0x3186, 0x18c3, 0x0000, 0x0000,
};
#endif

int sunny_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0x0000,
  0x0000, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffe0, 0x0000,
  0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000, 0x0000,
  0xffe0, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0xffe0,
  0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000,
};

#ifdef USE_WEATHER_ANI
int sunny1_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0x0000,
  0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffe0, 0x0000,
  0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000, 0x0000,
  0xffe0, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0xffff,
  0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000,
};

int sunny2_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0x0000,
  0x0000, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000,
  0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000, 0x0000,
  0xffff, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0xffe0,
  0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0x0000, 0x0000,
};
#endif

int cloudy_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0x0000,
  0x0000, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0xc618, 0xffe0, 0x0000,
  0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0xffe0, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618,
  0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618,
};

#ifdef USE_WEATHER_ANI
int cloudy1_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0x0000,
  0x0000, 0xffe0, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0xc618, 0xffff, 0x0000,
  0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0xffff, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618,
  0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618,
};

int cloudy2_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xffe0, 0x0000, 0x0000, 0xffff, 0x0000, 0x0000, 0x0000,
  0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0xc618, 0xffe0, 0x0000,
  0x0000, 0x0000, 0x0000, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0xffe0, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618,
  0x0000, 0x0000, 0xffe0, 0xffe0, 0xffe0, 0xffe0, 0xc618, 0xc618, 0xc618, 0xc618,
};
#endif

int ovrcst_ico [50] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0xc618, 0x0000, 0x0000,
  0x0000, 0x0000, 0xc618, 0xc618, 0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0x0000, 0xc618, 0xffff, 0xffff, 0xc618, 0xffff, 0xffff, 0xffff, 0xc618, 0x0000,
  0x0000, 0xc618, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xc618, 0x0000,
  0x0000, 0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000, 0x0000,
};

#ifdef USE_WEATHER_ANI
int ovrcst1_ico [50] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0xc618, 0x0000, 0x0000,
  0x0000, 0x0000, 0xc618, 0xc618, 0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0xffff, 0xffff, 0xffff, 0xc618, 0x0000,
  0x0000, 0xc618, 0xc618, 0xc618, 0xffff, 0xffff, 0xffff, 0xffff, 0xc618, 0x0000,
  0x0000, 0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000, 0x0000,
};

int ovrcst2_ico [50] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0xc618, 0x0000, 0x0000,
  0x0000, 0x0000, 0xc618, 0xc618, 0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0x0000, 0xc618, 0xffff, 0xffff, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0x0000, 0xc618, 0xffff, 0xffff, 0xffff, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000,
  0x0000, 0x0000, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0x0000, 0x0000,
};
#endif

int thndr_ico [50] = {
  0x041f, 0xc618, 0x041f, 0xc618, 0xc618, 0xc618, 0x041f, 0xc618, 0xc618, 0x041f,
  0xc618, 0xc618, 0xc618, 0xc618, 0x041f, 0xc618, 0xc618, 0x041f, 0xc618, 0xc618,
  0xc618, 0x041f, 0xc618, 0xc618, 0xc618, 0x041f, 0xc618, 0xc618, 0xc618, 0xc618,
  0xc618, 0xc618, 0xc618, 0x041f, 0xc618, 0xc618, 0xc618, 0xc618, 0xc618, 0x041f,
  0xc618, 0x041f, 0xc618, 0xc618, 0xc618, 0xc618, 0x041f, 0xc618, 0x041f, 0xc618,
};

int rain_ico [50] = {
  0x041f, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f,
  0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x041f, 0x0000,
};

#ifdef USE_WEATHER_ANI
int rain1_ico [50] = {
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x041f, 0x0000,
  0x041f, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f,
  0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f,
};

int rain2_ico [50] = {
  0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x041f, 0x0000,
  0x041f, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f,
  0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000,
};

int rain3_ico [50] = {
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x041f, 0x0000,
  0x041f, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f,
  0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000,
};

int rain4_ico [50] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f,
  0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x041f, 0x0000,
  0x041f, 0x0000, 0x041f, 0x0000, 0x0000, 0x0000, 0x041f, 0x0000, 0x0000, 0x041f,
};
#endif

int snow_ico [50] = {
  0xc618, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618,
  0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0xc618, 0x0000,
};

#ifdef USE_WEATHER_ANI
int snow1_ico [50] = {
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0xc618, 0x0000,
  0xc618, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618,
  0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618,
};

int snow2_ico [50] = {
  0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0xc618, 0x0000,
  0xc618, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618,
  0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000,
};

int snow3_ico [50] = {
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0xc618, 0x0000,
  0xc618, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618,
  0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000,
};

int snow4_ico [50] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618,
  0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0xc618, 0x0000,
  0xc618, 0x0000, 0xc618, 0x0000, 0x0000, 0x0000, 0xc618, 0x0000, 0x0000, 0xc618,
};
#endif

#ifdef USE_WEATHER_ANI
int *suny_ani[] = {sunny_ico, sunny1_ico, sunny2_ico, sunny1_ico, sunny2_ico};
int *clod_ani[] = {cloudy_ico, cloudy1_ico, cloudy2_ico, cloudy1_ico, cloudy2_ico};
int *ovct_ani[] = {ovrcst_ico, ovrcst1_ico, ovrcst2_ico, ovrcst1_ico, ovrcst2_ico};
int *rain_ani[] = {rain_ico, rain1_ico, rain2_ico, rain3_ico, rain4_ico};
int *thun_ani[] = {thndr_ico, rain1_ico, rain2_ico, rain3_ico, rain4_ico};
int *snow_ani[] = {snow_ico, snow1_ico, snow2_ico, snow3_ico, snow4_ico};
int *mony_ani[] = {moony_ico, moony1_ico, moony2_ico, moony1_ico, moony2_ico};
#else
int *suny_ani[] = {sunny_ico, sunny_ico, sunny_ico, sunny_ico, sunny_ico};
int *clod_ani[] = {cloudy_ico, cloudy_ico, cloudy_ico, cloudy_ico, cloudy_ico};
int *ovct_ani[] = {ovrcst_ico, ovrcst_ico, ovrcst_ico, ovrcst_ico, ovrcst_ico};
int *rain_ani[] = {rain_ico, rain_ico, rain_ico, rain_ico, rain_ico};
int *thun_ani[] = {thndr_ico, rain_ico, rain_ico, rain_ico, rain_ico};
int *snow_ani[] = {snow_ico, snow_ico, snow_ico, snow_ico, snow_ico};
int *mony_ani[] = {moony_ico, moony_ico, moony_ico, moony_ico, moony_ico};
#endif


int xo = 1, yo = 26;
char use_ani = 0;
char daytime = 1;
void draw_weather_conditions ()
{
  //0 - unk, 1 - sunny, 2 - cloudy, 3 - overcast, 4 - rainy, 5 - thunders, 6 - snow
  Serial.print ("weather conditions ");
  Serial.println (condM);
  //cleanup previous cond
  xo = 3*TF_COLS; yo = 1;
#ifdef USE_ICONS
if (condM == 0 && daytime)
  {
    Serial.print ("!weather condition icon unknown, show: ");
    Serial.println (condS);
    int cc_dgr = display.color565 (30, 30, 30);
    //draw the first 5 letters from the unknown weather condition
    String lstr = condS.substring (0, (condS.length () > 5?5:condS.length ()));
    lstr.toUpperCase ();
    TFDrawText (&display, lstr, xo, yo, cc_dgr);
  }
  else
  {
    TFDrawText (&display, String("     "), xo, yo, 0);
  }
  //
  xo = 4*TF_COLS; yo = 1;
  switch (condM)
  {
    case 0://unk
      break;
    case 1://sunny
      if (!daytime)
        DrawIcon (&display, moony_ico, xo, yo, 10, 5);
      else
        DrawIcon (&display, sunny_ico, xo, yo, 10, 5);
      //DrawIcon (&display, cloudy_ico, xo, yo, 10, 5);
      //DrawIcon (&display, ovrcst_ico, xo, yo, 10, 5);
      //DrawIcon (&display, rain_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 2://cloudy
      DrawIcon (&display, cloudy_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 3://overcast
      DrawIcon (&display, ovrcst_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 4://rainy
      DrawIcon (&display, rain_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 5://thunders
      DrawIcon (&display, thndr_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
    case 6://snow
      DrawIcon (&display, snow_ico, xo, yo, 10, 5);
      use_ani = 1;
      break;
  }
#else
  xo = 3*TF_COLS; yo = 1;
  Serial.print ("!weather condition icon unknown, show: ");
  Serial.println (condS);
  int cc_dgr = display.color565 (30, 30, 30);
  //draw the first 5 letters from the unknown weather condition
  String lstr = condS.substring (0, (condS.length () > 5?5:condS.length ()));
  lstr.toUpperCase ();
  TFDrawText (&display, lstr, xo, yo, cc_dgr);
#endif
}

void draw_weather ()
{
  int cc_wht = display.color565 (cin, cin, cin);
  int cc_red = display.color565 (cin, 0, 0);
  int cc_grn = display.color565 (0, cin, 0);
  int cc_blu = display.color565 (0, 0, cin);
  int cc_dgr = display.color565 (30, 30, 30);

  //int cc_ylw = display.color565 (cin, cin, 0);
  //int cc_gry = display.color565 (128, 128, 128);
  terminal.clear();
  Serial.println ("showing the weather");
  terminal.println ("showing the weather");
  xo = 0; yo = 1;
  TFDrawText (&display, String("                "), xo, yo, cc_dgr);
  if (tempM == -10000 || humiM == -10000 || presM == -10000)
  {
    //TFDrawText (&display, String("NO WEATHER DATA"), xo, yo, cc_dgr);
      Serial.println ("!no weather data available");
    terminal.println ("!no weather data available");
  }
  else
  {
    //weather below the clock
    //-temperature
    int lcc = cc_grn;
      //C
      if (tempM < 35)
        lcc = cc_red;
      if (tempM < 25)
        lcc = cc_blu;
      if (tempM < 18)
        lcc = cc_wht;
    
    String lstr = String (tempM) + String("C");
    Serial.print ("temperature: ");
    Serial.println (lstr);
    terminal.print ("Temperature: ");
    terminal.println (lstr);
    
    TFDrawText (&display, lstr, xo, yo, lcc);
    //weather conditions
    //-humidity
    lcc = cc_red;
    if (humiM < 65)
      lcc = cc_grn;
    if (humiM < 35)
      lcc = cc_blu;
    if (humiM < 15)
      lcc = cc_wht;
    lstr = String (humiM) + "%";
    xo = 8*TF_COLS;
    TFDrawText (&display, lstr, xo, yo, lcc);
    
    terminal.print ("Humidity: ");
    terminal.println (lstr);
    
    //-pressure
    lstr = String (presM);
    xo = 12*TF_COLS;
    TFDrawText(&display, lstr, xo, yo, cc_blu);
    terminal.print ("Pressure:");
    terminal.println (lstr);
    terminal.print ("");
    terminal.flush();

    //weather conditions

    draw_weather_conditions ();
  }
}

void scroll_draw_weather ()
{
  int cc_wht = display.color565 (cin, cin, cin);
  int cc_red = display.color565 (cin, 0, 0);
  int cc_grn = display.color565 (0, cin, 0);
  int cc_blu = display.color565 (0, 0, cin);
  int cc_dgr = display.color565 (30, 30, 30);
    magenta = display.color565 (255 * cin / 255, 0, 255 * cin / 255);
       cyan = display.color565 (0, cin , cin);

  //int cc_ylw = display.color565 (cin, cin, 0);
  //int cc_gry = display.color565 (128, 128, 128);
  xo = 0; yo = 1;
  TFDrawText (&display, String("                "), xo, yo, cc_dgr);
  if (tempM == -10000 || humiM == -10000 || presM == -10000)
  {
    //TFDrawText (&display, String("NO WEATHER DATA"), xo, yo, cc_dgr);
      Serial.println ("!no weather data available");
    terminal.println ("!no weather data available");
  }
  else
  {
    //weather below the clock
    //-temperature
    if(inout_prt)
    {
        int h = dht.readHumidity();
      // Read temperature as Celsius (the default)
        int t = dht.readTemperature();
        if(h>100 || h<10)  // unable to retract value...use old value
        {
          h=0;
        }
        if(t>51 || t<0)
        {
          t=0;
        }
        
        int lcc = cc_grn;
          //C
          if (t < 35)
            lcc = cc_red;
          if (t < 25)
            lcc = cc_blu;
          if (t < 18)
            lcc = cc_wht;
        TFDrawText (&display, "INSIDE", xo, yo, cyan);        
        String lstr = String (t) + String("C");  
        xo= xo = 8*TF_COLS;  
        TFDrawText (&display, lstr, xo, yo, lcc);

        //weather conditions
        //-humidity
        lcc = cc_red;
        if (h < 65)
          lcc = cc_grn;
        if (h < 35)
          lcc = cc_blu;
        if (h < 15)
          lcc = cc_wht;
        lstr = String (h) + "%";
        xo = 13*TF_COLS;
        TFDrawText (&display, lstr, xo, yo, lcc);
        inout_prt=!inout_prt;
    }
    else
    {

        int lcc = cc_grn;
          //C
          if (tempM < 35)
            lcc = cc_red;
          if (tempM < 25)
            lcc = cc_blu;
          if (tempM < 18)
            lcc = cc_wht;
        
        TFDrawText (&display, "OUTSIDE", xo, yo, magenta);
        String lstr = String (tempM) + String("C");  
        xo= xo = 8*TF_COLS;  
        TFDrawText (&display, lstr, xo, yo, lcc);

        //weather conditions
        //-humidity
        lcc = cc_red;
        if (humiM < 65)
          lcc = cc_grn;
        if (humiM < 35)
          lcc = cc_blu;
        if (humiM < 15)
          lcc = cc_wht;
        lstr = String (humiM) + "%";
        xo = 13*TF_COLS;
        TFDrawText (&display, lstr, xo, yo, lcc);
        inout_prt=!inout_prt;

    }
  }
}


void draw_love ()
{
  if(loveprint_flag==true && loveprt_1t_flag==true)
  {
    Serial.println(loveprt_1t_flag );
            display.fillRect(0, 0, 64, 8, blck);                        // blank out the area
            loveprt_1t_flag = false;
            Serial.println ("love");
          //love*you,boo

          yo = 1;
          
          xo  = 0; TFDrawChar (&display, 'S', xo, yo, display.color565 (cin, random (0, cin), random (0, cin)));
          xo += 4; TFDrawChar (&display, 'U', xo, yo, display.color565 (random (0, cin), cin , random (0, cin)));
          xo += 4; TFDrawChar (&display, 'D', xo, yo, display.color565 (cin, random (0, cin), random (0, cin)));
          xo += 4; TFDrawChar (&display, 'H', xo, yo, display.color565 (random (0, cin), random (0, cin), cin));
          xo += 4; TFDrawChar (&display, 'I', xo, yo, display.color565 (random (0, cin), cin, random (0, cin)));
          xo += 4; TFDrawChar (&display, 'R', xo, yo, display.color565 (cin, random (0, cin), random (0, cin)));
          xo += 4; TFDrawChar (&display, ' ', xo, yo, display.color565 (random (0, cin), random (0, cin), cin));
          xo += 4; TFDrawChar (&display, 'h', xo, yo, display.color565 (cin, 0, 0));
          xo += 4; TFDrawChar (&display, 'i', xo, yo, display.color565 (cin, 0, 0));
          xo += 4; TFDrawChar (&display, ' ', xo, yo, display.color565 (cin, random (0, cin), random (0, cin)));
          xo += 4; TFDrawChar (&display, 'A', xo, yo, display.color565 (random (0, cin), random (0, cin), cin));
          xo += 4; TFDrawChar (&display, 'N', xo, yo, display.color565 (cin, random (0, cin), random (0, cin)));
          xo += 4; TFDrawChar (&display, 'K', xo, yo, display.color565 (random (0, cin), cin, random (0, cin)));
          xo += 4; TFDrawChar (&display, 'I', xo, yo, display.color565 (cin, random (0, cin), random (0, cin)));
          xo += 4; TFDrawChar (&display, 'T', xo, yo, display.color565 (random (0, cin), random (0, cin), cin));
          xo += 4; TFDrawChar (&display, 'A', xo, yo, display.color565 (random (0, cin), cin, random (0, cin)));

  }
}
//

void draw_date ()
{
  int cc_grn = display.color565 (255* cin / 255, 110* cin / 255, 0);
  Serial.println ("showing the date");
  //for (int i = 0 ; i < 12; i++)
    //TFDrawChar (&display, '0' + i%10, xo + i * 5, yo, display.color565 (0, 255, 0));
  //date below the clock
  String lstr=String(Days [weekday()-1]);
  Serial.print("Days: ");
  Serial.println(lstr);
  if (lstr.length())
  {
    //
    xo=1; yo = 30;
    display.setFont(&Org_01);
    int rr = random (0, cin);
    int bb = random (0, cin);
    display.setCursor (xo, yo);
    display.setTextColor (display.color565 (cin, random (0, cin), random (0, cin)));
    display.print (lstr);
  }


         lstr = "";
         lstr += (day() < 10 ? "0" + String(day()) : String(day()));
         lstr += "-";
         lstr += (month() < 10 ? "0" + String(month()) : String(month()));
         lstr += "-";
         lstr += String(year());
  //
  if (lstr.length())
  {
    //
    int a=(lstr.length()*4)/2;
    xo=44-a;
    yo = 26;
    display.setFont(&Org_01);
    TFDrawText (&display, lstr, xo, yo, cc_grn);
  }
}

void draw_animations (int stp)
{
#ifdef USE_ICONS
  //weather icon animation
  int xo = 4*TF_COLS; 
  int yo = 1;
  //0 - unk, 1 - sunny, 2 - cloudy, 3 - overcast, 4 - rainy, 5 - thunders, 6 - snow
  if (use_ani)
  {
    int *af = NULL;
    //weather/night icon
    if (!daytime)
      af = mony_ani[stp%5];
    else
    {
      switch (condM)
      {
        case 1:
            af = suny_ani[stp%5];
          break;
        case 2:
            af = clod_ani[stp%5];
          break;
        case 3:
            af = ovct_ani[stp%5];
          break;
        case 4:
            af = rain_ani[stp%5];
          break;
        case 5:
            af = thun_ani[stp%5];
          break;
        case 6:
            af = snow_ani[stp%5];
          break;
      }
    }
    //draw animation
    if (af)
      DrawIcon (&display, af, xo, yo, 10, 5);
  }
#endif
}

#ifdef USE_FIREWORKS
//fireworks
// adapted to Arduino pxMatrix
// from https://r3dux.org/2010/10/how-to-create-a-simple-fireworks-effect-in-opengl-and-sdl/
// Define our initial screen width, height, and colour depth
int SCREEN_WIDTH  = 64;
int SCREEN_HEIGHT = 32;

const int FIREWORKS = 6;           // Number of fireworks
const int FIREWORK_PARTICLES = 8;  // Number of particles per firework

class Firework
{
  public:
    float x[FIREWORK_PARTICLES];
    float y[FIREWORK_PARTICLES];
    char lx[FIREWORK_PARTICLES], ly[FIREWORK_PARTICLES];
    float xSpeed[FIREWORK_PARTICLES];
    float ySpeed[FIREWORK_PARTICLES];

    char red;
    char blue;
    char green;
    char alpha;

    int framesUntilLaunch;

    char particleSize;
    boolean hasExploded;

    Firework(); // Constructor declaration
    void initialise();
    void move();
    void explode();
};

const float GRAVITY = 0.05f;
const float baselineSpeed = -1.0f;
const float maxSpeed = -2.0f;

// Constructor implementation
Firework::Firework()
{
  initialise();
  for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
  {
    lx[loop] = 0;
    ly[loop] = SCREEN_HEIGHT + 1; // Push the particle location down off the bottom of the screen
  }
}

void Firework::initialise()
{
    // Pick an initial x location and  random x/y speeds
    float xLoc = (rand() % SCREEN_WIDTH);
    float xSpeedVal = baselineSpeed + (rand() % (int)maxSpeed);
    float ySpeedVal = baselineSpeed + (rand() % (int)maxSpeed);

    // Set initial x/y location and speeds
    for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
    {
        x[loop] = xLoc;
        y[loop] = SCREEN_HEIGHT + 1; // Push the particle location down off the bottom of the screen
        xSpeed[loop] = xSpeedVal;
        ySpeed[loop] = ySpeedVal;
        //don't reset these otherwise particles won't be removed
        //lx[loop] = 0;
        //ly[loop] = SCREEN_HEIGHT + 1; // Push the particle location down off the bottom of the screen
    }

    // Assign a random colour and full alpha (i.e. particle is completely opaque)
    red   = (rand() % 255);/// (float)RAND_MAX);
    green = (rand() % 255); /// (float)RAND_MAX);
    blue  = (rand() % 255); /// (float)RAND_MAX);
    alpha = 50;//max particle frames

    // Firework will launch after a random amount of frames between 0 and 400
    framesUntilLaunch = ((int)rand() % (SCREEN_HEIGHT/2));

    // Size of the particle (as thrown to glPointSize) - range is 1.0f to 4.0f
    particleSize = 1.0f + ((float)rand() / (float)RAND_MAX) * 3.0f;

    // Flag to keep trackof whether the firework has exploded or not
    hasExploded = false;

    //cout << "Initialised a firework." << endl;
}

void Firework::move()
{
    for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
    {
        // Once the firework is ready to launch start moving the particles
        if (framesUntilLaunch <= 0)
        {
            //draw black on last known position
            //display.drawPixel (x[loop], y[loop], cc_blk);
            lx[loop] = x[loop];
            ly[loop] = y[loop];
            //
            x[loop] += xSpeed[loop];

            y[loop] += ySpeed[loop];

            ySpeed[loop] += GRAVITY;
        }
    }
    framesUntilLaunch--;

    // Once a fireworks speed turns positive (i.e. at top of arc) - blow it up!
    if (ySpeed[0] > 0.0f)
    {
        for (int loop2 = 0; loop2 < FIREWORK_PARTICLES; loop2++)
        {
            // Set a random x and y speed beteen -4 and + 4
            xSpeed[loop2] = -4 + (rand() / (float)RAND_MAX) * 8;
            ySpeed[loop2] = -4 + (rand() / (float)RAND_MAX) * 8;
        }

        //cout << "Boom!" << endl;
        hasExploded = true;
    }
}

void Firework::explode()
{
    for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
    {
        // Dampen the horizontal speed by 1% per frame
        xSpeed[loop] *= 0.99;

        //draw black on last known position
        //display.drawPixel (x[loop], y[loop], cc_blk);
        lx[loop] = x[loop];
        ly[loop] = y[loop];
        //
        // Move the particle
        x[loop] += xSpeed[loop];
        y[loop] += ySpeed[loop];

        // Apply gravity to the particle's speed
        ySpeed[loop] += GRAVITY;
    }

    // Fade out the particles (alpha is stored per firework, not per particle)
    if (alpha > 0)
    {
        alpha -= 1;
    }
    else // Once the alpha hits zero reset the firework
    {
        initialise();
    }
}

// Create our array of fireworks
Firework fw[FIREWORKS];

void fireworks_loop (int frm)
{
  int cc_frw;
  //display.fillScreen (0);
  // Draw fireworks
  //cout << "Firework count is: " << Firework::fireworkCount << endl;
  for (int loop = 0; loop < FIREWORKS; loop++)
  {
      for (int particleLoop = 0; particleLoop < FIREWORK_PARTICLES; particleLoop++)
      {
  
          // Set colour to yellow on way up, then whatever colour firework should be when exploded
          if (fw[loop].hasExploded == false)
          {
              //glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
              cc_frw = display.color565 (255, 255, 0);
          }
          else
          {
              //glColor4f(fw[loop].red, fw[loop].green, fw[loop].blue, fw[loop].alpha);
              //glVertex2f(fw[loop].x[particleLoop], fw[loop].y[particleLoop]);
              cc_frw = display.color565 (fw[loop].red, fw[loop].green, fw[loop].blue);
          }

          // Draw the point
          //glVertex2f(fw[loop].x[particleLoop], fw[loop].y[particleLoop]);
          display.drawPixel (fw[loop].x[particleLoop], fw[loop].y[particleLoop], cc_frw);
          display.drawPixel (fw[loop].lx[particleLoop], fw[loop].ly[particleLoop], 0);
      }
      // Move the firework appropriately depending on its explosion state
      if (fw[loop].hasExploded == false)
      {
          fw[loop].move();
      }
      else
      {
          fw[loop].explode();
      }
      //
      //delay (10);
  }
}
//-
#endif //define USE_FIREWORKS


void fire()
{
  if(fire_flag)
  {
      static int i = 0;
      static int last = 0;
      static int cm;
      cm = millis ();
      if ((cm - last) > 50)
      {
                //Serial.println(millis() - last);
          last = cm;
          i++;
          fireworks_loop (i);
      }
  }
}



//*******************************************************************************
// Run display sequence of date, digital clock, temp & humidity

void DisplayDigital() {


//******Display Love*******

 if (second()>50 && second()<57){draw_love();prt=true;}
 if(second()==58) {prev_day=100; loveprt_1t_flag= true;}
 
//******Display Fire*******
 if((minute() % 15 == 00) && second() <  21) {instant_fire=1;}
 if((minute() % 15 == 00) && second() == 21) {if(!inst_fire_bt){instant_fire=0;} RefreshDigitalTime();}


  // ************ DATE ***************
if(prt)
{
  if (day() != prev_day) {                                  // if the date changes 

      display.fillRect(0, 0, 64, 9, blck);                        // blank out the date
      allstr = String(day()) + " " +  String(Months [month() - 1]);    //compose the date
      display.setTextColor (display.color565 (cin, 0, 0));
      display.setCursor (2, 1);
      display.print (String(Days [weekday()-1])); // print day
      display.setTextColor (display.color565 (cin, cin, cin));
      display.setCursor (27, 1);
      display.print (allstr);                                              // print the date
      prt=false;
  }
 } 

  // ******* TIME**************************

  s0 = second() % 10;
  s1 = second() / 10;
  m0 = minute() % 10;
  m1 = minute() / 10;
  int th;  ///temproray hour for 24th 12;
  th=hour();
  if (th > 12 && military[0] == 'N')
      th -= 12;

  h0 = th % 10;
  h1 = th / 10;

 if(instant_fire && fire_flag){
        fire();
        delay(25);
        inst_fire_ft=true;}
  else
  {
        if(inst_fire_ft){RefreshDigitalTime();inst_fire_ft=false;}
        if (s0 != digit0.Value()) digit0.Morph(s0);                //morph the time
        if (s1 != digit1.Value()) digit1.Morph(s1);
        if (m0 != digit2.Value()) digit2.Morph(m0);
        if (m1 != digit3.Value()) digit3.Morph(m1);
        if (h0 != digit4.Value()) digit4.Morph(h0);
        if (h1 != digit5.Value()) digit5.Morph(h1);
   }
  //************************ SAVE ***************************************
  prev_dayOfTheWeek = weekday();     // save for next time
  prev_day = day();
  prev_month = month();
}


void RefreshDigitalTime() 
{

         display.fillScreen (blck);                      // blank the screen
          allstr = String(day()) + " " +  String(Months [month() - 1]);    //compose the date
          display.setTextColor (display.color565 (cin, 0, 0));
          display.setCursor (2, 1);
          display.print (String(Days [weekday()-1])); // print day
          display.setTextColor (display.color565 (0, 0, cin));
          display.setCursor (27, 1);
          display.print (allstr);                                               // print the date
          DrawColons();

        int th;  ///temproray hour for 24th 12;
        th=hour();
        if (th > 12 && military[0] == 'N')
            th -= 12;

        digit0.Draw(second() % 10);       //draw clock without morphing
        digit1.Draw(second() / 10);
        digit2.Draw(minute() % 10);
        digit3.Draw(minute() / 10);
        digit4.Draw(th % 10);
        digit5.Draw(th / 10);

}



void Face1()
{
  //
  hh = hour();   //NTP.getHour ();
  mm = minute(); //NTP.getMinute ();
  ss = second(); //NTP.getSecond ();
 
 
  static int i = 0;
  static int last = 0;
  static int cm;
   cm = millis ();
//******Display Love*******

 if (second()>50 && second()<57){draw_love();}
 if(second()==58) {draw_weather ();  loveprt_1t_flag= true;}
 
//******Display Fire*******
 if((minute() % 15 == 00) && second() <  21) {instant_fire=1;}
 if((minute() % 15 == 00) && second() == 21) {if(!inst_fire_bt){instant_fire=0;} ntpsync=1;}

          //weather animations
           if(second()<51 || second()>57)
           {
              if ((cm - last) > 150)
              {
                last = cm;
                i++;
                draw_animations (i);
              }
          }


  
   if (ntpsync==1)
   {
       ntpsync = 0;
       prevss = ss;
       prevmm = mm;
       prevhh = hh;
    
    display.fillScreen (0);
    draw_date ();
    draw_weather ();

    digit01.DrawColon (display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
    digit03.DrawColon (display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
    //military time?
    
    if (hh > 12 && military[0] == 'N')
      hh -= 12;
    //
  

    digit00.Draw (ss % 10);
    digit01.Draw (ss / 10);
    digit02.Draw (mm % 10);
    digit03.Draw (mm / 10);
    digit04.Draw (hh % 10);
    digit05.Draw (hh / 10);
    
  }
  else
  {

    if(instant_fire){
        fire();
        delay(25);
      }
        else{
              //seconds
              if (ss != prevss) 
              {
                int s0 = ss % 10;
                int s1 = ss / 10;
                if (s0 != digit00.Value ()) digit00.Morph (s0);
                if (s1 != digit01.Value ()) digit01.Morph (s1);
                //ntpClient.PrintTime();
                prevss = ss;
                //refresh weather every 5mins at 30sec in the minute
                if (ss == 30 && ((mm % 15) == 0))
                  getWeather ();
              }
              //minutes
              if (mm != prevmm)
              {
                int m0 = mm % 10;
                int m1 = mm / 10;
                if (m0 != digit02.Value ()) digit02.Morph (m0);
                if (m1 != digit03.Value ()) digit03.Morph (m1);
                prevmm = mm;   
              }
              //hours
              if (hh != prevhh) 
              {
                prevhh = hh;
                //
                  draw_date ();
                //brightness control: dimmed during the night(25), bright during the day(150)
                if (hh == 20 || hh == 8)
                {
                  ntpsync = 1;
                  //bri change is taken care of due to the sync
                }
                //military time?
                if (hh > 12 && military[0] == 'N')
                    hh -= 12;
                //
                int h0 = hh % 10;
                int h1 = hh / 10;
                if (h0 != digit04.Value ()) digit04.Morph (h0);
                if (h1 != digit05.Value ()) digit05.Morph (h1);
              }//hh changed
        }
  }
}


void Face2()
{
  //
  hh = hour();   //NTP.getHour ();
  mm = minute(); //NTP.getMinute ();
  ss = second(); //NTP.getSecond ();
 
 
  static int i = 0;
  static int last = 0;
  static int cm;
   cm = millis ();
//******Display Love*******

 if (second()>53 && second()<60){draw_love();}
if(second()==55) {w_face2= false;} 
 if(second()==00) {loveprt_1t_flag= true;}

 
//******Display Fire*******
 if((minute() % 15 == 00) && second() <  21) {instant_fire=1;}
 if((minute() % 15 == 00) && second() == 21) {if(!inst_fire_bt){instant_fire=0;} ntpsync=1;}

          //weather animations
           


  if(second() % 5==0)
  {
    if(w_face2)
    {
        w_face2=false;
        scroll_draw_weather ();
    }
  }
  else
  {
    w_face2=true;
  }
  
   if (ntpsync==1)
   {
       ntpsync = 0;
       prevss = ss;
       prevmm = mm;
       prevhh = hh;
    
    display.fillScreen (0);
    draw_date ();
    scroll_draw_weather ();
    digit01.DrawColon (display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
    digit03.DrawColon (display.color565(bred * cin / 255, bgreen * cin / 255, bblue * cin / 255));
    
    if (hh > 12 && military[0] == 'N')
      hh -= 12;
    //

    digit00.Draw (ss % 10);
    digit01.Draw (ss / 10);
    digit02.Draw (mm % 10);
    digit03.Draw (mm / 10);
    digit04.Draw (hh % 10);
    digit05.Draw (hh / 10);
    
  }
  else
  {

    if(instant_fire){
        fire();
        delay(25);
      }
        else{
              //seconds
              if (ss != prevss) 
              {
                int s0 = ss % 10;
                int s1 = ss / 10;
                if (s0 != digit00.Value ()) digit00.Morph (s0);
                if (s1 != digit01.Value ()) digit01.Morph (s1);
                //ntpClient.PrintTime();
                prevss = ss;
                //refresh weather every 5mins at 30sec in the minute
                if (ss == 30 && ((mm % 15) == 0))
                  getWeather ();
              }
              //minutes
              if (mm != prevmm)
              {
                int m0 = mm % 10;
                int m1 = mm / 10;
                if (m0 != digit02.Value ()) digit02.Morph (m0);
                if (m1 != digit03.Value ()) digit03.Morph (m1);
                prevmm = mm;   
              }
              //hours
              if (hh != prevhh) 
              {
                prevhh = hh;
                //
                  draw_date ();
                //brightness control: dimmed during the night(25), bright during the day(150)
                if (hh == 20 || hh == 8)
                {
                  ntpsync = 1;
                  //bri change is taken care of due to the sync
                }
                //military time?
                if (hh > 12 && military[0] == 'N')
                    hh -= 12;
                //
                int h0 = hh % 10;
                int h1 = hh / 10;
                if (h0 != digit04.Value ()) digit04.Morph (h0);
                if (h1 != digit05.Value ()) digit05.Morph (h1);
              }//hh changed
        }
  }
}


//===========================================================================FACE 3




void Face3()
{
      if (newMessageAvailable)
    {
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
    }
/*
    if(second()%10==0)
    {
        getDate();
    }
    else
    {
        getDowe();
    }*/
    if(olddate!=day())
    {
        getdayd();
        olddate=day(); 
    }

    if(!corona_update_flag)
    {
        if (Mode)
        {
          if (second()>50 && second()<57){loveprt_1t_flag= true;}
          //getDate();
          getWeather_f3();
          scroll_text(23,12,WeatherT);
          scroll_text(23,12,WeatherH);
          Mode = false;
        }
    }


    if(weather_face3)
    {
      if(minute()%15==0 && second()<5 )
      {
        getWeather();
      }
      weather_print();
      for (int i = 0; i < Wstr.length(); i++) { 
            newMessage[i] = Wstr.charAt(i); 
        } 
        //strcpy(newMessage,corona.asStr());
        //terminal.clear();
        terminal.print("Message on print : ");
        terminal.println(newMessage);

    }
    
 
      scroll_text(23,5,curMessage);
}


char *mon2str(uint8_t mon, char *psz, uint8_t len)
{
  static const char str[][4] PROGMEM =
  {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };

  *psz = '\0';
  mon--;
  if (mon < 12)
  {
    strncpy_P(psz, str[mon], len);
    psz[len] = '\0';
  }

  return(psz);
}
char *dow2str(uint8_t code, char *psz, uint8_t len)
{
  static const char str[][10] PROGMEM =
  {
    "Sunday", "Monday", "Tuesday", "Wednesday",
    "Thursday", "Friday", "Saturday"
  };

  *psz = '\0';
  code--;
  if (code < 7)
  {
    strncpy_P(psz, str[code], len);
    psz[len] = '\0';
  }

  return(psz);
}

void getWeather_f3()
{
  dht.begin();
  Temp = dht.readTemperature();
  Humi = dht.readHumidity();
  if (isnan(Humi) || isnan(Temp)) {
      delay(100);
    return;
  }
  WeatherT = "Temp: ";
  WeatherT += Temp;
  WeatherT += "$";
  WeatherH = "Humid: ";
  WeatherH += Humi;
  WeatherH += " %";
}

void getDate()
// Code for reading date
{      
  text = mon2str(month(), szBuf, sizeof(szBuf)-1);
        display.setCursor(0,0);
  display.fillRect(0, 0, 64, 8, display.color565(0, 0, 0));
  display.setFont(&kongtext4pt7b);
  display.setTextColor(myRED);
  if (day() < 10) { display.print("0");  }
  display.print(day());
  display.setTextColor(myGREEN);
  display.print(text);
  display.setTextColor(myRED);
  display.print(year());
  display.setFont();
  //Serial.println("Printing date");
}


void getDowe()
// Code for reading date
{
  text = dow2str(dow, szBuf, sizeof(szBuf)-1);
  uint16_t text_length = text.length();
  int xpos = (matrix_width - text_length*7)/2;
  display.setCursor(xpos,0);
  display.fillRect(0, 0, 64, 8, display.color565(0, 0, 0));
  display.setFont(&kongtext4pt7b);
  uint8_t y = 0;
  for (y=0; y<10; y++) {
    display.setTextColor(Whel(y));
    display.print(text[y]);
    }
  display.setFont();
  //Serial.println("Printing dAY");
}  

void getdayd()
// Code for reading date
{
   display.fillRect(0, 0, 64, 8, display.color565(0, 0, 0));
    // display.fillRect(0, 0, 23, 8, display.color565(100, 100, 100));
    //display.fillRect(1, 1, 21, 6, display.color565(0, 0, 0));
   DrawHand(0, 0, 23,  0, display.color565(cin, cin, cin));
   DrawHand(0, 0, 7,  90, display.color565(cin, cin, cin));
   DrawHand(0, 7, 23,  0, display.color565(cin, cin, cin));
   DrawHand(23, 0, 7,  90, display.color565(cin, cin, cin));
   
   //display.drawLine(0, 23, 8, 90, display.color565(100, 100, 100));
   //display.drawLine(8, 0, 23, 0, display.color565(100, 100, 100));
  
   text = dow2str(weekday(), szBuf, sizeof(szBuf)-1);
   uint16_t text_length = text.length();
   int xpos = 1;
   display.setCursor(xpos,0);
   display.setFont(&kongtext4pt7b);
   uint8_t y = 0;
   for (y=0; y<3; y++) 
   {
     display.setTextColor(Whel(y));
     display.print(text[y]);
   }  

  display.setFont();
  display.setFont(&kongtext4pt7b);
  display.setFont();  
  //Serial.println("Printing dAY");
  text = mon2str(month(), szBuf, sizeof(szBuf)-1);
  display.setCursor(26,0);
  display.fillRect(26, 0, 64, 8, display.color565(0, 0, 0));
  display.setFont(&kongtext4pt7b);
  display.setTextColor(Whel(random(0,9)));
  if (day() < 10) { display.print("0");  }
  display.print(day());
  display.setFont();
  display.setCursor(41,0);
  display.setTextColor(Whel(random(0,9)));
  display.setFont(&kongtext4pt7b);
  display.print(text);
  display.setFont();  

  //Serial.println();

}  



void getRTCh(char *psz)
// Code for reading clock time
{
  int hh=hour();
  if (hh > 12 && military[0] == 'N')
                    hh -= 12;
  sprintf(psz, "%02d", hh);
  display.setCursor(0, 16); 
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(display.color565(cin, 0, cin));
  display.fillRect(0, 8, 24, 15, display.color565(0, 0, 0));
  display.print(szTime);
  display.setFont();
  NewRTCh=h;
}

void getRTCm(char *psz)
// Code for reading clock time
{
  sprintf(psz, "%02d", m);
  display.setCursor(26, 16); 
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(display.color565(cin, 0, cin));
  display.fillRect(26, 8, 25, 15, display.color565(0, 0, 0));
  display.print(szTime);
  display.setFont();
  NewRTCm=m;
}


void getTim(char *psz, bool f = true)
// Code for reading clock time
{
  if (NewRTCs != s/10) 
  {
  display.setCursor(20, 8); 
  display.setTextSize(2);
  display.setTextColor(Whel(random(0,9)));
  display.fillRect(24, 12, 2, 6, display.color565(0, 0, 0));
    display.print(f ? ':' : ' ');
  display.setCursor(54, 10); 
  display.setTextSize(1);
  display.fillRect(54, 10, 10, 6, display.color565(0, 0, 0));
  display.setFont(&kongtext4pt7b);
  display.setTextColor(Whel(random(0,9)));
    display.print(f ? ' ' : '*');
  display.setFont();
  display.setCursor(51, 16); 
  display.setTextSize(1);
  display.setTextColor(Whel(random(0,9)));
  sprintf(psz, "%02d", s);
  display.fillRect(51, 17, 13, 6, display.color565(0, 0, 0));
  display.setFont(&kongtext4pt7b);
    display.print(szTime);
  display.setFont();
      NewRTCs=s/10;
    }
    else
  {
  display.setCursor(20, 8); 
  display.setTextSize(2);
  display.setTextColor(Whel(random(0,9)));
  display.fillRect(24, 12, 2, 6, display.color565(0, 0, 0));
    display.print(f ? ':' : ' ');
  display.setCursor(54, 10); 
  display.setTextSize(1);
  display.fillRect(54, 10, 10, 6, display.color565(0, 0, 0));
  display.setFont(&kongtext4pt7b);
  display.setTextColor(Whel(random(0,9)));
    display.print(f ? ' ' : '*');
  display.setFont();
  display.setCursor(51, 16); 
  display.setTextColor(Whel(random(0,9)));
  sprintf(psz, "%02d", s);
  display.fillRect(58, 17, 6, 6, display.color565(0, 0, 0));
  display.setFont(&kongtext4pt7b);
    display.print(szTime);
  display.setFont();
    }
}


void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text)
  {  
    uint16_t text_length = text.length();
    // Asuming 5 pixel average character width
    for (int xpos=matrix_width; xpos>-(matrix_width+text_length*5); xpos--)
    {
      if(exit_function)
      {
       exit_function=false;;
       return;
      }
      if(minute()%10==0 && second()==0)
      {
        corona_update();
      }


      if(second()==30||second()==0)
      {
        Mode = true;
      }

      Blynk.run();
      if(!RunFace3_Flag)
      {
        return;
      }

      if (second()>50 && second()<57){draw_love();}
      if(second()==58) { getdayd(); olddate=0; loveprt_1t_flag= true;}
 
      //******Display Fire*******
      if(minute() % 15 == 00 && second() <  21) {instant_fire=1;}
      if((minute() % 15 == 00) && (second() > 20 ||second() < 23)) {if(!inst_fire_bt){instant_fire=0;} olddate=0; Mode = true; return;}


            if(instant_fire)
            {
               fire();
                delay(25);
                NewRTCh = 24;
                NewRTCm = 60;
                NewRTCs = 10;
                olddate=0;
            }
        else{
                display.setCursor(xpos,ypos);
                display.fillRect(0, 23, 64, 9, display.color565(0, 0, 0));
                display.setFont(&kongtext4pt7b);
                display.print(text);
                display.setFont();
                delay(scroll_delay);
                yield();
                if (millis() - lastTime >= 1000)
                {
                  lastTime = millis();
                  updateTime();
                  getTim(szTime, flasher);
                  flasher = !flasher;
                  if (NewRTCh != h) 
                  {
                      getRTCh(szTime);
                  }
                  if (NewRTCm != m) 
                  {
                      getRTCm(szTime);
                      getWeather_f3();
                  }
                }
           }
  }
    r++;
    if(r == 8)
    {
        r = 0;
        g++;
        if(g == 8) 
        {
          g = 0;
          b++;
          if(b == 8) 
          {
            b = 0;
          }
        }
    }
}





//====================================================================
// Input a value 0 to 24 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Wheel(byte WheelPos) {
  if(WheelPos < 2) {
   return display.color565(cin, 0, 0);
  } else if(WheelPos < 5) {
   WheelPos -= 2;
   return display.color565(0, 0, cin);
  } else {
   WheelPos -= 5;
   return display.color565(cin, 0, 0);
  }
}
// Input a value 0 to 24 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Whel(byte WheelPos) {
  if(WheelPos < 1) {
   return display.color565(cin, 0, 0);
  } else if(WheelPos < 2) {
   WheelPos -= 1;
   return display.color565(0, cin, 0);
  } else if(WheelPos < 3) {
   WheelPos -= 2;
   return display.color565(cin, cin, 0);
  } else if(WheelPos < 4) {
   WheelPos -= 3;
   return display.color565(cin, 0, cin);
  } else if(WheelPos < 5) {
   WheelPos -= 4;
   return display.color565(0, 0, cin);
  } else if(WheelPos < 6) {
   WheelPos -= 5;
   return display.color565(0, cin, cin);
  } else if(WheelPos < 7) {
   WheelPos -= 6;
   return display.color565(cin, 0, cin);
  } else if(WheelPos < 8) {
   WheelPos -= 7;
   return display.color565(cin, cin, 0);
  } else if(WheelPos < 9) {
   WheelPos -= 8;
   return display.color565(cin, 0, 0);
  } else {
   WheelPos -= 9;
   return display.color565(cin, cin, cin);
  }
}


void updateTime()
{
      dow = weekday();
      aday = day();
      amonth = month();
      ayear = year();
      int hh =hour();
      if (hh > 12 && military[0] == 'N')
                    hh -= 12;
      h = hh;
      m = minute();
      s = second();
}

void corona_update()
{
 Serial.print ("i:connecting to weather server.. "); 

 HTTPClient http; //Object of class HTTPClient
    http.begin("http://coronavirus-19-api.herokuapp.com/countries/india");
    int httpCode = http.GET();
    Serial.print (httpCode);
    if (httpCode > 0) 
    {
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
 
      int cases = root["cases"]; 
      int todayCases = root["todayCases"]; 
      int deaths = root["deaths"]; 
      int todayDeaths = root["todayDeaths"]; 
      int recovered = root["recovered"]; 
      int active = root["active"]; 

      Serial.print("cases:");
      Serial.println(cases);
      Serial.print("todayCases:");
      Serial.println(todayCases);
      Serial.print("deaths:");
      Serial.println(deaths);
      Serial.print("todayDeaths:");
      Serial.println(todayDeaths);
      Serial.print("recovered:");
      Serial.println(recovered);
      Serial.print("active:");
      Serial.println(active);
    corona = "";
    corona += "T-cases:";  
    corona += cases;
    corona +=  " Today:"; 
    corona +=  todayCases ;
    corona += " T-Deaths:" ;
    corona += deaths ;
    corona += " Deaths:";
     corona += todayDeaths ;
     corona += " Recovered:" ;
     corona += recovered; 
     corona += " T-Active:" ;
     corona += active;

    corona_update_flag=true;
    newMessageAvailable = true;
    }
    http.end(); //Close connection
 
}
void weather_update()
{
 
  if (!apiKey.length ()) 
  {
    terminal.println("No Key");
    return;
  }
  terminal.println("connecting");
  if (client.connect (server, 80)) {
   Serial.println ("connected."); 
    // Make a HTTP request: 
    client.print ("GET /data/2.5/weather?"); 
    client.print ("q="+location); 
    client.print ("&appid="+apiKey); 
    client.print ("&cnt=1"); 
    client.println ("&units=metric");
    client.println ("Host: api.openweathermap.org"); 
    client.println ("Connection: close");
    client.println (); 
  }
  else
  {
    terminal.println ("cant connect");
    return;
  }
  delay (1000);
  String sval = "";
  int bT, bT2;
  String line = client.readStringUntil ('\n');
    if (!line.length ()) terminal.println ("can't retrieve weather data");
    else
    {
      Serial.println(line);
      terminal.println ("weather");
      terminal.println (line);
      bT = line.indexOf ("\"main\":\"");                        //weather conditions - find "main":"?????",
      if (bT > 0) 
      {
        bT2  = line.indexOf ("\",\"", bT + 8);
        sval = line.substring (bT + 8, bT2);
        terminal.println ("condition " + String(sval));
        if (sval.equals("Clear")) condition = 1;            //0 - unk, 1 - sunny, 2 - cloudy, 3 - overcast, 4 - rainy, 5 - thunders, 6 - snow
        else if (sval.equals("Clouds")) condition = 2;
        else if (sval.equals("Overcast")) condition = 3;
        else if (sval.equals("Rain")) condition = 4;
        else if (sval.equals("Thunderstorm")) condition = 5;
        else if (sval.equals("Snow")) condition = 6;
      }

      bT = line.indexOf ("\"temp\":");                        // find temperature
      if (bT > 0)
      {
        bT2 = line.indexOf (",\"", bT + 7);
        sval = line.substring (bT + 7, bT2);
        terminal.println ("Temperature " + String(sval));
        temp = sval.toInt ();
      }
      else terminal.println ("no temperature");

      bT = line.indexOf ("\"pressure\":");                    // find pressure
      if (bT > 0) 
      {
        bT2 = line.indexOf (",\"", bT + 11);
        sval = line.substring (bT + 11, bT2);
        terminal.println ("pressure " + String(sval));
        pressure = sval.toInt();
      }
      else terminal.println ("No Pressure");

      bT = line.indexOf ("\"humidity\":");                    // find humidity
      if (bT > 0) 
      {
        bT2 = line.indexOf (",\"", bT + 11);
        sval = line.substring (bT + 11, bT2);
        terminal.println ("humidity " + String(sval));
        humidity = sval.toInt();
      }
      else terminal.println ("No Humidity");

      bT = line.indexOf ("\"speed\":");                    //find wind speed
      if (bT > 0) 
      {
        bT2 = line.indexOf (",\"", bT + 8);
        sval = line.substring (bT + 8, bT2);
        terminal.println ("wind speed " + String(sval));
        wind_speed = sval.toInt() * 3.6;                 // convert mtrs/second to kph
      }
      else terminal.println ("No speed");

      bT = line.indexOf ("\"deg\":");                       // find wind direction
      if (bT > 0) 
      {
        bT2 = line.indexOf (",\"", bT + 6);
        sval = line.substring (bT + 6, bT2 - 1);
        terminal.println ("wind direction " + String(sval));
        wind_deg = sval.toInt();
      }
      else terminal.println ("No wind direction");

      bT = line.indexOf ("\"sunrise\":");                    // find sunrise
      if (bT > 0) 
      {
        bT2 = line.indexOf (",\"", bT + 10);
        sval = line.substring (bT + 10, bT2);
        sun_rise = sval.toInt();                                 // UNIX time UDT
        sun_rise += (timeZone * 3600);                  // add our time zone as seconds
        if(minute(sun_rise) < 10)
        {                          // using timelib function to convert UNIX time to hours and minutes
            terminal.println("Sunrise " + String(hour(sun_rise)) + ":0" + String(minute(sun_rise))); 
        } 
        else 
        {
            terminal.println("Sunrise " + String(hour(sun_rise)) + ":" + String(minute(sun_rise))); 
        }
      }
      else terminal.println ("No sunrise");

      bT = line.indexOf ("\"sunset\":");                    // sunset
      if (bT > 0) 
      {
        bT2 = line.indexOf (",\"", bT + 9);
        sval = line.substring (bT + 9, bT2);
        sun_set = sval.toInt();                                  // UNIX time UDT
        sun_set += (timeZone * 3600);                   // add our time zone as seconds
        if(minute(sun_set) <10) 
        {
             terminal.println("Sunset " + String(hour(sun_set)) + ":0" + String(minute(sun_set)));  // display as hours:minutes
        } 
        else 
        {
             terminal.println("Sunset " + String(hour(sun_set)) + ":" + String(minute(sun_set)));
        }
      }
      else terminal.println ("No sunset");

      terminal.println();
      terminal.println();
      terminal.flush();
    }                                                                   // if line length
  }  
////COPY TEXT

void weather_print()
{
    h = dht.readHumidity();
    t = dht.readTemperature();
    Wstr = "Inside Temp:";                                                      // or dht.readTemperature(true) for Fahrenheit  
    Wstr += String(t) + "'C ";                                        // indoor temp/humidity  line 1
    Wstr += "Humid:"; 
    Wstr += String(h) + "% ";                                  // indoor humidity  line 3
    Wstr += "Outside Temp:";                                                      // or dht.readTemperature(true) for Fahrenheit  
    Wstr += String(tempM) + "'C ";                           // outdoor temperature  line 2
    Wstr += "Humid:";                                                      // or dht.readTemperature(true) for Fahrenheit 
    Wstr += String (humidity) + "%";                // outdoor humidity  line 4
    Wstr += "Pressure:";                                                      // or dht.readTemperature(true) for Fahrenheit 
    Wstr += String (pressure)  + "mb ";                  // pressure line 5 
    Wstr += "Wind Dir:";                                                      // or dht.readTemperature(true) for Fahrenheit 
    WindDirection (wind_deg);                                                     // convert degrees to Wind Direction
    Wstr += dir + " at " + String (wind_speed) + "kph";     // direction & wind speed lline 6
    Wstr += "Weather is :";                                                      // or dht.readTemperature(true) for Fahrenheit 
    switch (condition) {                                                                  // get conditions line 7
      case 1: Wstr += String("Clear"); break;
      case 2: Wstr += String("Cloudy"); break;
      case 3: Wstr += String("Overcast"); break;
      case 4: Wstr += String("Rainy"); break;
      case 5: Wstr += String("Stormy"); break;
      case 6: Wstr += String("Snowy"); break;
    }

     if(minute(sun_rise) < 10) {
             Wstr += String(hour(sun_rise)) + ":0" + String(minute(sun_rise)) + "SUNRISE";      //sunrise line 8
        } else {
            Wstr += String(hour(sun_rise)) + ":" + String(minute(sun_rise)) + " SUNRISE";         //sunrise line 8
        }

    if(minute(sun_set) < 10) {
            Wstr += String(hour(sun_set)) + ":0" + String(minute(sun_set)) + " SUNSET";         //sunset line 9
       } else {
            Wstr += String(hour(sun_set)) + ":" + String(minute(sun_set)) + " SUNSET";           //sunset line 9
       }
}



void Face4()
{
 //******Display Fire*******
      if(minute() % 15 == 00 && second() <  21) 
        {
          instant_fire=1;
          pageload_f4 = false;
          pageload_f4_1t=true;
        }

      if(minute() % 15 == 00 && (second() > 20 && second() < 22)) 
        {
          if(!inst_fire_bt)
            {
              instant_fire=0;
              if(pageload_f4_1t)
              {
                pageload_f4 = true;
                pageload_f4_1t=false;
              }
            } 
        }


  
  
  if (pageload_f4)
  {
    Serial.println("Face4 Started");
        display.fillScreen(0);
        display.fillRect(0, 0, 64, 32, display.color565 (cin, cin/2, 0));                        // blank out the area     
        display.fillRect(1, 1, 62, 30, blck);
        display.setTextColor(display.color565(cin, cin/3, cin/1.5)); // R,G,B
        display.setFont(&FreeSansBold9pt7b);
        display.setCursor(10, 14);
        int hh=hour();
        if (hh > 12 && military[0] == 'N')
                    hh -= 12;
        display.printf("%02d", hh);
        display.setCursor(31, 14);
        display.printf(":");
        display.setCursor(36, 14);
        display.printf("%02d", minute());
        pageload_f4=false;
        display.setFont(&kongtext4pt7b);
        String t =dow2str(weekday(), szBuf, sizeof(szBuf)-1);
        int x=  t.length();
        Serial.println(x);
        x=x*7;
        x=63-x;
        x=x/2;     
        display.setCursor(x, 22);
        display.setTextColor(display.color565(0, cin/1.33, cin)); // R,G,B
        display.printf("%s",  dow2str(weekday(), szBuf, sizeof(szBuf)-1));
        int d = 2;
        if(day()>9 || day()<20)
        {
           d= 5; 
        }
        display.setCursor(3, 29);
        display.setFont(&Org_01);      
        display.setTextColor(magenta);
        display.printf("%02d/%02d/%02d\n", day(), month(), year());  
        pageload_f4=false;
        olds = second();
        oldh=hour();
        oldMin=minute();  
  }
  else
  {
        if(instant_fire)
            {
               fire();
                delay(25);
            }
        else{            

                    //minute change
                    if(minute()!=oldMin)
                    {
                        display.fillRect(36, 2, 20, 13, blck);
                        display.setTextColor(display.color565(cin, cin/3, cin/1.5)); // R,G,B
                        display.setFont(&FreeSansBold9pt7b);
                        display.setCursor(36, 14);
                        display.printf("%02d", minute());
                        oldMin=minute();  
                    }
                    //hour change
                    if(hour()!=oldh)
                    {
                        display.fillRect(10, 2, 20, 13, blck);
                        display.setTextColor(display.color565(cin, cin/3, cin/1.5)); // R,G,B
                        display.setFont(&FreeSansBold9pt7b);
                        display.setCursor(10, 14);
                        int hh=hour();
                        if (hh > 12 && military[0] == 'N')
                                    hh -= 12;
                        display.printf("%02d", hh);
                        oldh=hour();
                    }

                    if(hour()==0 && minute()==0 && second()==0)
                    {
                        display.fillRect(1, 17, 62, 14, blck);
                        display.setFont(&kongtext4pt7b);
                        String t =dow2str(weekday(), szBuf, sizeof(szBuf)-1);
                        int x=  t.length();
                        Serial.println(x);
                        x=x*7;
                        x=63-x;
                        x=x/2;     
                        display.setCursor(x, 22);
                        display.setTextColor(bgreen);
                        display.printf("%s",  dow2str(weekday(), szBuf, sizeof(szBuf)-1));
                        int d = 2;
                        if(day()>9 || day()<20)
                        {
                           d= 5; 
                        }
                        display.setCursor(3, 29);
                        display.setFont(&Org_01);      
                        display.setTextColor(magenta);
                        display.printf("%02d/%02d/%02d\n", day(), month(), year());  
                    }

                    
                    //second run
                    if(second()%2==0)
                    {
                        display.setTextColor(blck); // R,G,B
                        display.setFont(&FreeSansBold9pt7b);
                        display.setCursor(31, 14);
                        display.print(":");
                        delay(500);
                    }
                    else
                    {
                        display.setTextColor(display.color565(cin, cin/1.5, cin/3)); // R,G,B
                        display.setFont(&FreeSansBold9pt7b);
                        display.setCursor(31, 14);
                        display.print(":");
                        delay(500);
                    }
                      if(olds!=second())
                      { display.fillRect(0, 1, 2, 32, blck);
                        display.fillRect(62, 1, 2, 32, blck);

                        if(second()<31)
                        {
                          display.fillRect(0, 1, 1, second(), display.color565(cin, cin, cin));
                          display.fillRect(0, second()+1, 2, 2, display.color565(cin, 0, 0)); 

                          display.fillRect(63, 1, 1, second(), display.color565(cin, cin, cin));
                          display.fillRect(62, second()+1, 2, 2, display.color565(cin, 0, 0)); 

                        }
                        else
                        {
                          display.fillRect(0, 1, 1, 60-second(), display.color565(cin, cin, cin));
                          display.fillRect(0, 60-second(), 2, 2, display.color565(cin, 0, 0)); 

                          display.fillRect(63, 1, 1, 60-second(), display.color565(cin, cin, cin));
                          display.fillRect(62, 60-second(), 2, 2, display.color565(cin, 0, 0)); 
                        }
                        olds=second();
                      }
                      
                        
                      //display.fillRect(49, 6, 14, 8, blck);   
                      //display.setFont(&kongtext4pt7b);
                      //display.setCursor(49, 13);
                      //display.printf("%02d", second());
             }       
  }   
} 