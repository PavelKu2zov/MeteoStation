#define ONE_WIRE_BUS      4 // data pin DS18B20
#define PIN_AM2305        5 // data pin AM2305
#define PIN_CS_SD_CARD_1  31
#define PIN_CS_SD_CARD_2  30
#define PIN_INT_ALARM     19
#define PIN_INT_BUTTON    18
#define INT_ALARM         4// arduino pin 19, real int = int2
#define INT_BUTTON        5// arduino pin 18, real int = int3

#define ALARM_1           1
#define NUMBER_ROWS_SCREEN        (6)
#define NUMBER_SHOW_PARAM         (9)
#define BUTTON_PIN                (A0)

#define BUTTON_UP                       (3)
#define BUTTON_LEFT                     (2)
#define BUTTON_RIGHT                    (4)
#define BUTTON_DOWN                     (1)
#define BUTTON_SELECT                   (5)

#define PIN_CLK_LCD              (32)
#define PIN_CLK_DIN              (33)
#define PIN_CLK_DC               (34)
#define PIN_CLK_CE               (35)
#define PIN_CLK_RST              (36)

#define LCD_NUMBER_PIXELS_WIDE_SYMBOL   (5)
#define SCREEN_DATE_POS                 (6)
#define SCREEN_TIME_POS                 (7)
#define SCREEN_ALARM_POS                (8)
#define LCD_NUM_SYMBOL_IN_ROW			(14)
#define SCREEN_DATE_DAY_POS             ((LCD_NUMBER_PIXELS_WIDE_SYMBOL * 7)-1)
#define SCREEN_DATE_MONTH_POS           ((LCD_NUMBER_PIXELS_WIDE_SYMBOL * 11)-1)
#define SCREEN_DATE_YEAR_POS            (LCD_NUMBER_PIXELS_WIDE_SYMBOL * 12)

typedef enum MENU_SCREEN_enum
{
    MAIN_MENU=0,
    DATE_MENU,
    TIME_MENU,
    ALARM_MENU
}MENU_SCREEN;

typedef enum DATE_MENU_STATE_SCREEN_enum
{
    DAY=0,
    MONTH,
    YEAR
}DATE_MENU_STATE_SCREEN;

typedef enum TIME_MENU_STATE_SCREEN_enum
{
    HOUR=0,
    MINUTE,
    SECOND
}TIME_MENU_STATE_SCREEN;

typedef struct DATE_str
{
    uint8_t day;
    uint8_t month;
    uint8_t year;
}DATE;

typedef struct TIME_str
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}TIME;

typedef struct ALARM_str
{
    byte ADay;
    byte AHour;
    byte AMinute;
    byte ASecond;
    byte AlarmBits;
    bool ADy;
    bool Ah12;
    bool APM;
}ALARM;

typedef struct DATE_MENU_SCREEN_str
{
    DATE_MENU_STATE_SCREEN state;
    DATE date;
}DATE_MENU_SCREEN;

typedef struct TIME_MENU_SCREEN_str
{
    TIME_MENU_STATE_SCREEN state;
    TIME time;
}TIME_MENU_SCREEN;

extern DATE_MENU_SCREEN menuDate;
extern TIME_MENU_SCREEN menuTime;
extern bool alarmTime;
extern byte buttonNum;
extern bool pressAnyButton;
extern char screenValue[NUMBER_SHOW_PARAM][LCD_NUM_SYMBOL_IN_ROW];
extern uint8_t firstRowPos;
extern uint8_t cursorPos;
extern int timeDelay;
extern int timeDelayOld0;
extern const DeviceAddress t1_deviceAddress;
extern const DeviceAddress t2_deviceAddress;
extern const DeviceAddress t3_deviceAddress; 
extern const int buttonDOWN;
extern const int buttonEnter;
extern const int buttonLeft;
extern const int buttonUP;
extern const int buttonRight;
extern float pressurePascals;
extern float humidity;
extern float t1;
extern float t2;
extern float t3;
extern float Vbat;
extern DateTime timeCurrent;
extern DateTime timeOld;
extern DS3231  rtc;
extern File myFile;
extern OneWire oneWire;
extern DallasTemperature sensors;
extern DHT humudity_sensor;
extern LPS25HB barometer;
extern Nokia_LCD lcd;

extern int whbuttonPressed(void);
extern void write2sd(void);
extern void LCDShow(void);
extern void makeStringsForLCD(DATE *date, TIME *time, ALARM *alarm);
extern void ReadSensors(void);
extern void printCurrentMenuOnLCD(MENU_SCREEN menuLCD);
extern void SetDate(uint8_t  d , uint8_t  m, uint8_t yOff );
extern void SetTime(uint8_t  h, uint8_t  m, uint8_t s);
