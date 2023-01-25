#include <Wire.h>
#define RTC_addr 0x6F

typedef struct Date_obj{
  uint hour;
  uint min;
  uint sec;
  uint Day;
  uint Date;
  uint Month;
  uint Year;
} Date_obj;


uint bcd2uint(byte val){return ( (val/16*10) + (val%16) );}


byte uint2bcd(uint val){return ( (val/10*16) + (val%10) );}



void setDate(Date_obj date_args){
  Wire.beginTransmission(RTC_addr);
  Wire.write(0); 
  Wire.write((uint2bcd(date_args.sec) & 0x7F) | 0x80); //second and st
  Wire.write(uint2bcd(date_args.min) & 0x7F); //minute 
  Wire.write(uint2bcd(date_args.hour) & 0x3F); //hour
  Wire.write(uint2bcd(date_args.Day) & 0x07); //day
  Wire.write(uint2bcd(date_args.Date) & 0x2F); //date
  Wire.write(uint2bcd(date_args.Month) & 0x1F); //Month
  Wire.write(uint2bcd(date_args.Year- 2000U)); //Year
  Wire.endTransmission();  
}

Date_obj getDate(){
  int index = 0 ;
  Date_obj date_time_store;
  Wire.beginTransmission(RTC_addr);
  Wire.write(0);
  Wire.endTransmission(); 
  Wire.requestFrom(RTC_addr,7 ); // read two byte
  while(Wire.available())    // slave may send less than requested
  {
    switch (index) {
      case 0: 
        date_time_store.sec = bcd2uint(Wire.read()&0x7F);
        break;
      case 1:
        date_time_store.min = bcd2uint(Wire.read()&0x7F);
        break;
      case 2: 
        date_time_store.hour = bcd2uint(Wire.read()&0x3F);
        break;
      case 3: 
        date_time_store.Day = bcd2uint(Wire.read()&0x07);
        break;
      case 4:
        date_time_store.Date = bcd2uint(Wire.read()&0x2F);
        break;
      case 5:
        date_time_store.Month = bcd2uint(Wire.read()&0x1F);
        break;
      case 6:
        date_time_store.Year = bcd2uint(Wire.read())+2000U;
        break;
    }
    index++;
  }
  return date_time_store;
}

void displayDate(Date_obj datetime){
  String week_days[7]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
  Serial.printf("%d:%d:%d\n", datetime.hour, datetime.min, datetime.sec);
  Serial.printf("%s %d/%d/%d \n",week_days[datetime.Day-1] ,datetime.Date, datetime.Month, datetime.Year);
  Serial.println("------------------------");
}

void setup() {
  Wire.begin(4,5);
  Serial.begin(115200);
  Date_obj date_time;
  date_time.hour = 12;
  date_time.min = 30;
  date_time.sec = 1;
  date_time.Day = 2;
  date_time.Date = 29;
  date_time.Month = 11;
  date_time.Year = 2023;
  setDate(date_time);
}

void loop() {
  displayDate(getDate());
  delay(1000);
}
