/*  2 ds18b20
humidity am2305
barometer 
real time clock DS3231
writing to 2 SDcard
*/
/********************************************************************/
/*
 * TODO:
 * buron interrupt
 * monitor and watch setup menu and action
 * lorawan
 * sun battery
 */
/********************************************************************/
#include <OneWire.h> // для работы с датчиком температуры DS18B20
#include <DallasTemperature.h> // для работы с датчиком температуры DS18B20
#include <DHT.h> // библиотека для работы с датчиками DHT11, DHT21, DHT22
#include <DS3231.h> //часы
#include <Wire.h>
#include <SparkFun_LPS25HB_Arduino_Library.h>
#include <SD.h> //sd карта
#include <SPI.h> //sd карта
#include <GyverPower.h> //sleep and wake up
#include "buildTime.h" // для парсинга строки даты и времени, полученной при компиляции
#include <Nokia_LCD.h> //nokia 5110 display
#include "Init.h"
/********************************************************************/


DATE_MENU_SCREEN menuDate;
TIME_MENU_SCREEN menuTime;


bool alarmTime = false; 
byte buttonNum = 0;     // какая кнопка нажата
bool pressAnyButton = false; // была ли нажата кнопка любая
char screenValue[NUMBER_SHOW_PARAM][LCD_NUM_SYMBOL_IN_ROW];
uint8_t firstRowPos = 0;
uint8_t cursorPos = 0;
int timeDelay = 0;
int timeDelayOld = 0;


//#define DEBUG 1 //debug switch between printing and sd card
/********************************************************************/

const DeviceAddress t1_deviceAddress = { 0x28, 0xdb, 0x4, 0x75, 0xd0, 0x01, 0x3c, 0xd4 };
const DeviceAddress t2_deviceAddress = { 0x28, 0x4b,0xeb, 0x07, 0xb6, 0x01, 0x3c, 0x0b };
const DeviceAddress t3_deviceAddress = { 0x28, 0x48,0xdf, 0x07, 0xb6, 0x01, 0x3c, 0xc9 };

//button addresses analogread()
const int buttonDOWN = 372; //shitty 
const int buttonEnter = 420;   
const int buttonLeft = 520;    
const int buttonUP = 630;    //also shitty sometimes
const int buttonRight = 850;    

float pressurePascals = 0;// Создаём переменную для значения атмосферного давления в Паскалях
float humidity = 0;// переменная показания влажности
float t1 = 0; //температура первого датчика
float t2 = 0; //температура второго датчика
float t3 = 0; //температура третьего датчика
float Vbat = 0; // напряжение батареи
DateTime timeCurrent; // текущее время
DateTime timeOld; 

DS3231  rtc;
File myFile;
OneWire oneWire(ONE_WIRE_BUS); //вход датчика DS18B20
DallasTemperature sensors(&oneWire);
DHT humudity_sensor(PIN_AM2305, DHT22); //датчик влажности am2305
LPS25HB barometer;
Nokia_LCD lcd(PIN_CLK_LCD /* CLK */, PIN_CLK_DIN /* DIN */, PIN_CLK_DC /* DC */, PIN_CLK_CE /* CE */, PIN_CLK_RST /* RST */); //nokia lcd pins


     
void setup()
{
  //#ifdef DEBUG
    Serial.begin(9600); // открываем последовательный порт для мониторинга действий в программе
  //#endif  
    Wire.begin();// Start the I2C interface
    
    lcd.begin(); //initialize screen
    lcd.setBacklight(true);
    lcd.setContrast(60);
    lcd.clear(true);
    delay(2000);
    lcd.clear();
    lcd.setBacklight(false);
    lcd.setCursor(0,5);
    lcd.setInverted(true);
    lcd.print("Hello world!");
    lcd.setInverted(false);
    lcd.println("\nI am here.");
    
    pinMode(PIN_INT1, INPUT);// пин для внешнего прерывания от RTC
    power.setSleepMode(POWERDOWN_SLEEP);// настройка сна IDLE_SLEEP - 

    pinMode(PIN_INT2, INPUT);// пин для внешнего прерывания от button
    
    sensors.begin();
    
    barometer.begin(Wire, LPS25HB_I2C_ADDR_ALT);
    
    humudity_sensor.begin();
    
    rtc.setSecond(BUILD_SEC);    // устанавливаем секунд
    rtc.setMinute(BUILD_MIN);    // установка минут
    rtc.setHour(BUILD_HOUR);     //установка часов  
    rtc.setDate(BUILD_DAY);     // устанавливаем число
    rtc.setMonth(BUILD_MONTH);   // Устанавливаем месяц
    rtc.setYear(BUILD_YEAR);    // Устанавливаем год
    rtc.setClockMode(false);    // установка режима 12/24h. True is 12-h, false is 24-hour.
    rtc.setA1Time(0,0,0,10,0x0e, false, false, false);//setA1Time(byte A1Day, byte A1Hour, byte A1Minute, byte A1Second, byte AlarmBits, bool A1Dy, bool A1h12, bool A1PM)
    rtc.turnOnAlarm(1);

    attachInterrupt(0,buttonPressed,FALLING); 

  
    
}

void loop()
{
	if (true == pressAnyButton)
	{
		ReadSensors();
		LCDShow();
		pressAnyButton = false;
		timeOld = timeCurrent;
        timeDelay = millis();
        timeDelayOld = timeDelay;
	}
  
	if (true == alarmTime)
	{
		ReadSensors();
		write2sd();
        alarmTime = false;
	}
	
  
	if (0)//((timeCurrent.unixtime() - timeOld.unixtime())>5)
	{
		rtc.checkIfAlarm(ALARM_1);// сбрасываем флаг ALARM_1
		attachInterrupt(1, isr, FALLING);  // подключаем прерывание на пин D3 (Arduino NANO)
		lcd.clear();
		lcd.setCursor(0,2);
		lcd.print("Sleep");
		power.sleepDelay(0xffffffff);  // спим очень долго просыпаемся по прерыванию
		lcd.clear(); 
		lcd.print("Wakeup");
	}
	else
	{
        // читаем время RTC раз в секунду
		timeDelay = millis();
		if ((timeDelay - timeDelayOld)>1000)
        {
            timeCurrent = RTClib::now();  // чтение текущего времени
            timeDelayOld = timeDelay;
        }  
	}
    

}



/********************************обработчик аппаратного прерывания********************/ 
void isr() {
  // дёргаем за функцию "проснуться"
  // без неё проснёмся чуть позже (через 0-8 секунд)
  power.wakeUp();
  //Serial.println("\r\nISR");
  detachInterrupt(1);
  alarmTime = true;  
}






/********************************обработчик прерывания по кнопке********************/ 
void buttonPressed()          
{   
 power.wakeUp();
 ADCSRA |= (1 << ADEN);
 buttonNum = whbuttonPressed();
 if (0 != buttonNum)
 {
  pressAnyButton = true;
 }
 else
 {
  ;
 }
 
}
