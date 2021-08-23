#include <Arduino.h>

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


//**************************************************************************************************
// @Function      anybuttonPressed()
//--------------------------------------------------------------------------------------------------
// @Description   Has any button pressed?
//--------------------------------------------------------------------------------------------------
// @Notes         None.  
//--------------------------------------------------------------------------------------------------
// @ReturnValue   true  - button was pressed
//                false - button wasn't pressed
//--------------------------------------------------------------------------------------------------
// @Parameters    None.
//**************************************************************************************************
bool anybuttonPressed(void)
{
    int reading = analogRead(BUTTON_PIN);
    return (reading > 300) && (reading < 900);
}// end of anybuttonPressed()



//**************************************************************************************************
// @Function      whbuttonPressed()
//--------------------------------------------------------------------------------------------------
// @Description   What button was pressed?
//--------------------------------------------------------------------------------------------------
// @Notes         None.  
//--------------------------------------------------------------------------------------------------
// @ReturnValue   Number button
//--------------------------------------------------------------------------------------------------
// @Parameters    None.
//**************************************************************************************************
int whbuttonPressed(void)
{
    int reading = analogRead(BUTTON_PIN);
    if (reading > 400 && reading < 500) return 1;
     else {if (reading>500 && reading<600) return 5;
            else {if (reading > 600 && reading < 700) return 2;
                    else {if (reading > 800 && reading < 900) return 3;
                          else {if (reading >300 && reading < 400) return 4;
                                  else return 0;
                                  }
                          }
                  }
        }
}// end of whbuttonPressed()



//**************************************************************************************************
// @Function      write2sd()
//--------------------------------------------------------------------------------------------------
// @Description   Write data in two SD card
//--------------------------------------------------------------------------------------------------
// @Notes         None.  
//--------------------------------------------------------------------------------------------------
// @ReturnValue   None.
//--------------------------------------------------------------------------------------------------
// @Parameters    None.
//**************************************************************************************************
void write2sd(void)
{
      
    #ifdef DEBUG
    Serial.print("Initializing SD card #1...");
    #endif
    if (SD.begin(PIN_CS_SD_CARD_1)) 
      {
        #ifdef DEBUG
          Serial.println("initialization done.");
        #endif
      
        myFile = SD.open("temp4.txt", FILE_WRITE);
        if (myFile)
        {    
          myFile.print(t1);
          myFile.print(" | ");
          myFile.print(t2);
          myFile.print(" | ");
          myFile.print(t3);
          myFile.print(" | ");     
          myFile.print(pressurePascals);
          myFile.print(" | ");
          myFile.print(humidity);
          myFile.print(" | ");
          myFile.print(timeCurrent.day());
          myFile.print(".");
          myFile.print(timeCurrent.month());
          myFile.print(".");
          myFile.print(timeCurrent.year());
          myFile.print("  ");
          myFile.print(timeCurrent.hour());
          myFile.print(":");
          myFile.print(timeCurrent.minute());
          myFile.print(":");
          myFile.println(timeCurrent.second());
          myFile.close();
        } 
        else 
        {
          #ifdef DEBUG
          Serial.println("Could not open file (writing).");
          #endif
        }
    }
    else
    {
      #ifdef DEBUG
      Serial.println("initialization failed SD card #1!");
      Serial.print("error code = ");
      Serial.println(SD.card.errorCode());
      #endif
    }
    SD.end();

    #ifdef DEBUG
    Serial.print("Initializing SD card #2...");
    #endif
    if (SD.begin(PIN_CS_SD_CARD_2)) 
      {
        #ifdef DEBUG
          Serial.println("initialization done.");
        #endif
      
        myFile = SD.open("temp4.txt", FILE_WRITE);
        if (myFile)
        {  
          myFile.print(t1);
          myFile.print(" | ");
          myFile.print(t2);
          myFile.print(" | ");
          myFile.print(t3);
          myFile.print(" | ");     
          myFile.print(pressurePascals);
          myFile.print(" | ");
          myFile.print(humidity);
          myFile.print(" | ");
          myFile.print(timeCurrent.day());
          myFile.print(".");
          myFile.print(timeCurrent.month());
          myFile.print(".");
          myFile.print(timeCurrent.year());
          myFile.print("  ");
          myFile.print(timeCurrent.hour());
          myFile.print(":");
          myFile.print(timeCurrent.minute());
          myFile.print(":");
          myFile.println(timeCurrent.second());
          myFile.close(); 
        } 
        else 
        {
          #ifdef DEBUG
          Serial.println("Could not open file (writing).");
          #endif
        }
    }
    else
    {
      #ifdef DEBUG
      Serial.println("initialization failed SD card #2!");
      Serial.print("error code = ");
      Serial.println(SD.card.errorCode());
      #endif
    }
    SD.end();  
}// end of write2sd()



//**************************************************************************************************
// @Function      LCDShow()
//--------------------------------------------------------------------------------------------------
// @Description   Show menu on LCD
//--------------------------------------------------------------------------------------------------
// @Notes         None.  
//--------------------------------------------------------------------------------------------------
// @ReturnValue   None.
//--------------------------------------------------------------------------------------------------
// @Parameters    None.
//**************************************************************************************************
 void LCDShow(void)
{
  static MENU_SCREEN menuLCD = MAIN_MENU;
  
/*------------------------------ MAIN menu ----------------------------*/  
    if (MAIN_MENU == menuLCD)
    {
        if (BUTTON_DOWN == buttonNum)// down
        {
            if ((NUMBER_SHOW_PARAM-1) != cursorPos)
            {
                if ((firstRowPos+(NUMBER_ROWS_SCREEN-1)) == cursorPos)
                {
                    firstRowPos +=1; 
                }
                cursorPos +=1;
            }
                
            printCurrentMenuOnLCD(menuLCD);
        }
        else if (BUTTON_UP == buttonNum)// up
           {
                if (0 != cursorPos)
                {
                    if(firstRowPos == cursorPos)
                    {
                        firstRowPos -=1;
                    }
                    cursorPos -=1;
                }
                    
                printCurrentMenuOnLCD(menuLCD);
            }
        else if (BUTTON_SELECT == buttonNum)// select
            {
                if (SCREEN_DATE_POS == cursorPos) 
                {
                    menuLCD = DATE_MENU;
                    menuDate.state = DAY;
                    printCurrentMenuOnLCD(menuLCD);
                }
                else if (SCREEN_TIME_POS == cursorPos) 
                {
                    menuLCD = TIME_MENU;
                    ;                
                }
                else if (SCREEN_ALARM_POS == cursorPos)
                {
                    menuLCD = ALARM_MENU;
                    ;        
                }
            }
    }
/*------------------------------ DATE menu ----------------------------*/
    else if (DATE_MENU == menuLCD)
    {
        if (BUTTON_DOWN == buttonNum)// down
        {
            if (DAY == menuDate.state)
            {
                if (menuDate.date.day != 1)
                {
                    menuDate.date.day--;
                }            
            }
            else if (MONTH == menuDate.state)
            {
                if (menuDate.date.month != 1)
                {
                    menuDate.date.month--;
                }
            }
            else if (YEAR == menuDate.state)
            {
                if (menuDate.date.year != 0)
                {
                    menuDate.date.year--;
                }
            }
        }
        else if (BUTTON_UP == buttonNum)// up
        {
            if (DAY == menuDate.state)
            {
                if((menuDate.date.month == 1) || (menuDate.date.month == 3) || (menuDate.date.month == 5) || (menuDate.date.month == 7)\
                    || (menuDate.date.month == 8) || (menuDate.date.month == 10) || (menuDate.date.month == 12) || (menuDate.date.day != 31))
                {
                    menuDate.date.day++; 
                }
                else if ((menuDate.date.month == 4) || (menuDate.date.month == 6) || (menuDate.date.month == 9) || (menuDate.date.month == 11)
                    || (menuDate.date.day != 30))
                {
                    menuDate.date.day++;
                }
                else if ((menuDate.date.month == 2) || (menuDate.date.day != 28))
                {
                    menuDate.date.day++;
                }                    
            }
            else if (MONTH == menuDate.state)
            {
                if (menuDate.date.month != 12)
                {
                    menuDate.date.month++;
                }
            }
            else if (YEAR == menuDate.state)
            {
                if (menuDate.date.year != 99)
                {
                    menuDate.date.year++;
                }
            }           
        }
        else if (BUTTON_LEFT == buttonNum)// left
        {
            if (DAY == menuDate.state)
            {
               ;
            }
            else if (MONTH == menuDate.state)
            {
                menuDate.state = DAY;
            }
            else if (YEAR == menuDate.state)
            {
                menuDate.state = MONTH;
            }
        }
        else if (BUTTON_RIGHT == buttonNum)// right
        {
            if (DAY == menuDate.state)
            {
                menuDate.state = MONTH;
            }
            else if (MONTH == menuDate.state)
            {
                menuDate.state = YEAR;
            }
            else if (YEAR == menuDate.state)
            {
                ;
            }    
        }
        else if (BUTTON_SELECT == buttonNum)// select
        {
            SetDate(menuDate.date.day, menuDate.date.month, menuDate.date.year);
            menuLCD = MAIN_MENU;
        }
        printCurrentMenuOnLCD(menuLCD);
    }
/*------------------------------ TIME menu ----------------------------*/
    else if (TIME_MENU == menuLCD)
    {
      
    }
/*--------------------------- ALARM menu ------------------------*/
    else if (ALARM_MENU == menuLCD)
    {
      
    }
}//end of LCDShow()



//**************************************************************************************************
// @Function      makeStringsForLCD()
//--------------------------------------------------------------------------------------------------
// @Description   Make strings for LCD's rows
//--------------------------------------------------------------------------------------------------
// @Notes         None.  
//--------------------------------------------------------------------------------------------------
// @ReturnValue   None.
//--------------------------------------------------------------------------------------------------
// @Parameters   date,time,alarm -> parameters, which need show
//**************************************************************************************************
void makeStringsForLCD(DATE *date, TIME *time, ALARM *alarm)
{
    char str_temp[10];
	dtostrf(t1, 3, 1, str_temp);
	snprintf(screenValue[0],LCD_NUM_SYMBOL_IN_ROW,"t1     %s C",str_temp);
	dtostrf(t2, 3, 1, str_temp);
	snprintf(screenValue[1],LCD_NUM_SYMBOL_IN_ROW,"t2     %s C",str_temp);
	dtostrf(t3, 3, 1, str_temp);
	snprintf(screenValue[2],LCD_NUM_SYMBOL_IN_ROW,"t3     %s C",str_temp);
	dtostrf(humidity, 3, 1, str_temp);
	snprintf(screenValue[3],LCD_NUM_SYMBOL_IN_ROW,"Hum   %s %%",str_temp);
	dtostrf(pressurePascals, 6, 1, str_temp);
	snprintf(screenValue[4],LCD_NUM_SYMBOL_IN_ROW,"P  %s Pa",str_temp);
    dtostrf(Vbat, 4, 2, str_temp);
    snprintf(screenValue[5],LCD_NUM_SYMBOL_IN_ROW,"Vbat   %s V",str_temp);
	snprintf(screenValue[6],LCD_NUM_SYMBOL_IN_ROW,"Date  %d.%d.%d",date->day,date->month,date->year);
	snprintf(screenValue[7],LCD_NUM_SYMBOL_IN_ROW,"Time  %d:%d:%d",time->hour,time->minute,time->second);
	snprintf(screenValue[8],LCD_NUM_SYMBOL_IN_ROW,"AL %d:%d:%d:%d",alarm->ADay,alarm->AHour,alarm->AMinute,alarm->ASecond);
}// end of makeStringsForLCD()



//**************************************************************************************************
// @Function      ReadSensors()
//--------------------------------------------------------------------------------------------------
// @Description   read all sensors
//--------------------------------------------------------------------------------------------------
// @Notes         None.  
//--------------------------------------------------------------------------------------------------
// @ReturnValue   None.
//--------------------------------------------------------------------------------------------------
// @Parameters    None.
//**************************************************************************************************
void ReadSensors(void)
{
    timeCurrent = RTClib::now();  // чтение текущего времени
	pressurePascals = barometer.getPressure_hPa();
	humidity = humudity_sensor.readHumidity();

	sensors.requestTemperatures();   // Send command to all the sensors for temperature conversion

	// если датчик t1 подсоединен считываем температуру
	if (true == sensors.requestTemperaturesByAddress(t1_deviceAddress))
	{
	  t1 = sensors.getTempC(t1_deviceAddress);
	}
	else
	{
	  t1 = NAN;  
	}
    // если датчик t2 подсоединен считываем температуру
	if (true == sensors.requestTemperaturesByAddress(t2_deviceAddress))
	{
	  t2 = sensors.getTempC(t1_deviceAddress);
	}
	else
	{
	  t2 = NAN;  
	}
    // если датчик t3 подсоединен считываем температуру
	if (true == sensors.requestTemperaturesByAddress(t3_deviceAddress))
	{
	  t3 = sensors.getTempC(t3_deviceAddress);
	}
	else
	{
	  t3 = NAN;  
	} 
	
	/**************************print debug*******************************/

    #ifdef DEBUG
    Serial.print(timeCurrent.day());
    Serial.print(".");
	Serial.print(timeCurrent.month());
    Serial.print(".");
    Serial.print(timeCurrent.year());
    Serial.print("  ");
    Serial.print(timeCurrent.hour());
    Serial.print(":");
    Serial.print(timeCurrent.minute());
    Serial.print(":");
    Serial.print(timeCurrent.second()); 
	
	  Serial.print(" ");
    Serial.print("Sensor t1:");
    Serial.print(t1);
    Serial.print((char)176);//shows degrees character
    Serial.print("C  |  ");
		
    Serial.print("Sensor t2:");
    Serial.print(t2);
    Serial.print((char)176);//shows degrees character
    Serial.print("C  |  ");
    
    Serial.print("Sensor t3:");
    Serial.print(t3);
    Serial.print((char)176);//shows degrees character
    Serial.print("C  |  ");

    Serial.print("Pressure_P: ");
    Serial.print(pressurePascals);
    Serial.println("Pa  ");     
	
    //Serial.print("Pressure_mmhg: ");
    //Serial.print(pressureMillimetersHg);
    //Serial.println("mmhg  ");     
	
    Serial.print("Altitude: ");
    Serial.print(altitude);
    Serial.println("m  ");  
	
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%  ");  
	
    #endif

}// end of ReadSensors()



//**************************************************************************************************
// @Function      printCurrentMenuOnLCD()
//--------------------------------------------------------------------------------------------------
// @Description   print cuurent menu on LCD
//--------------------------------------------------------------------------------------------------
// @Notes         None.  
//--------------------------------------------------------------------------------------------------
// @ReturnValue   None.
//--------------------------------------------------------------------------------------------------
// @Parameters    menuLCD - cuurent menu
//**************************************************************************************************
void printCurrentMenuOnLCD(MENU_SCREEN menuLCD)
{
    DATE date;
    TIME time;
    ALARM alarm;
    
    if ( MAIN_MENU == menuLCD )
    { 
        date.day = timeCurrent.day();
        date.month = timeCurrent.month();
        date.year = timeCurrent.year();
        time.hour = timeCurrent.hour();
        time.minute = timeCurrent.minute();
        time.second = timeCurrent.second();
        rtc.getA1Time(alarm.ADay, alarm.AHour, alarm.AMinute, alarm.ASecond,\
                  alarm.AlarmBits, alarm.ADy, alarm.Ah12, alarm.APM); 
        makeStringsForLCD(&date,&time,&alarm);
        
        lcd.clear();
        for (byte i=0;i< NUMBER_ROWS_SCREEN;i++)
        {
           lcd.setCursor(0,i);
           if (i == (cursorPos-firstRowPos)) 
           {
                lcd.setInverted(true);	
           }
           else
           {
               lcd.setInverted(false);
           }				   
           lcd.print(screenValue[firstRowPos + i]);  
        }
    }
    else if ( DATE_MENU == menuLCD )
    {
        date.day = menuDate.date.day;
        date.month = menuDate.date.month;
        date.year = menuDate.date.year;
        time.hour = timeCurrent.hour();
        time.minute = timeCurrent.minute();
        time.second = timeCurrent.second();
        rtc.getA1Time(alarm.ADay, alarm.AHour, alarm.AMinute, alarm.ASecond,\
                  alarm.AlarmBits, alarm.ADy, alarm.Ah12, alarm.APM); 
        makeStringsForLCD(&date,&time,&alarm);
        
        lcd.clear();
        for (byte i=0;i< NUMBER_ROWS_SCREEN;i++)
        {
            lcd.setCursor(0,i);
            lcd.print(screenValue[firstRowPos + i]);  
        }
        uint8_t temp = 0;
        if ( DAY == menuDate.state )
        {
            lcd.setCursor(SCREEN_DATE_DAY_POS,(cursorPos-firstRowPos));
            temp = date.day;
        }
        else if ( MONTH == menuDate.state )
        {
            lcd.setCursor(SCREEN_DATE_MONTH_POS,(cursorPos-firstRowPos));
            temp = date.month;            
        }
        else if ( YEAR == menuDate.state )
        {
            lcd.setCursor(SCREEN_DATE_YEAR_POS,(cursorPos-firstRowPos));
            temp = date.year;        
        }        
        lcd.print("  ");
        lcd.setInverted(true);
        lcd.print(temp);
        lcd.setInverted(false);        
    }
    else if ( TIME_MENU == menuLCD )
    {

    }
    else if ( ALARM_MENU == menuLCD )
    {
        
    }
        
}// end of printCurrentMenuOnLCD()



//**************************************************************************************************
// @Function      SetDate()
//--------------------------------------------------------------------------------------------------
// @Description   Set date in RTC
//--------------------------------------------------------------------------------------------------
// @Notes         None.  
//--------------------------------------------------------------------------------------------------
// @ReturnValue   None.
//--------------------------------------------------------------------------------------------------
// @Parameters    d -> day,
//                m -> month
//                yOff -> year, only last two digits              
//**************************************************************************************************
void SetDate(uint8_t  d , uint8_t  m, uint8_t yOff )
{
    rtc.setDate(d);     // устанавливаем число
    rtc.setMonth(m);   // Устанавливаем месяц
    rtc.setYear(2000+yOff);    // Устанавливаем год    
}// end of SetDate()
