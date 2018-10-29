#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>

void alarmOff()
{
 printf("Alarm Off.\n");
 digitalWrite(5, HIGH);
 digitalWrite(0, LOW);
 digitalWrite(4, LOW);
}

void alarmSounding()
{
 int alarmDisabled = 0;
 printf("Alarm Sounding! Hold for 4 seconds to disable.\n");
 system("curl -X POST https://maker.ifttt.com/trigger/alarm_triggered/with/key/dx6eyL5uEw3ydlVK32hHSw ");
 printf("\n");
 while (alarmDisabled == 0)
 { 
  digitalWrite(5, HIGH) ; 
  digitalWrite(0, HIGH) ; 
  digitalWrite(4, HIGH) ; delay (2000);
  digitalWrite(5, LOW)  ; 
  digitalWrite(0, LOW)  ; 
  digitalWrite(4, LOW)  ; delay (2000);
  if (digitalRead(2) == 0)
   alarmDisabled = 1;
 }
 alarmOff();
 while (digitalRead(2) == 0); 
}


void alarmTriggered()
{
 
 time_t currentTime = time(NULL);
 time_t endTime = currentTime + 10;
 int alarmDisabled = 0;
 printf("Alarm is Triggered!\nYou have 10 seconds to disable alarm!\n");
 while (currentTime <= endTime && alarmDisabled == 0)
 {
  currentTime = time(NULL);
  digitalWrite(5, HIGH); 
  digitalWrite(0, HIGH) ; delay (2000);
  digitalWrite(5, LOW) ; 
  digitalWrite(0, LOW); delay (2000);
  if (digitalRead(2) == 0)
  {
    alarmDisabled = 1;
    printf("Alarm Disabled.\n");
    alarmOff();
    while (digitalRead(2) == 0);
  }
 }
 if (alarmDisabled == 0)
 {
  alarmSounding();
 }
}

void armed()
{
 printf("Alarm Armed.\n");
 digitalWrite (5, LOW) ;
 digitalWrite (0, HIGH) ;
 for (;;)
 {
  if (digitalRead(7) == 1)
  {
   alarmTriggered();
   break;
  }
  if (digitalRead(2) == 0)
  {
   printf("Alarm Disabled\n");
   alarmOff();
   while (digitalRead(2) == 0);
   break;
  }
 }
}

void arming()
{
 time_t endTime = time(NULL) + 10;
 printf("Alarm Arming.\n");
 while (time(NULL) <= endTime)
 {
   digitalWrite (5, HIGH); delay (1000); 
   digitalWrite (5, LOW) ; delay (1000);
 }  
 armed();
}

int main(int argc, char *argv[])
{
  
  wiringPiSetup () ;
  pinMode(7,INPUT); /* IR */
  pinMode(0, OUTPUT); /* LED 2 */
  pinMode(5, OUTPUT);/* LED 1 */
  pinMode(2, INPUT);/* button */
  pinMode(4, OUTPUT);/* buzzer */
  pullUpDnControl(2, PUD_UP);
  
  alarmOff();
  while(1) 
  {
    if (digitalRead(2) == 0)
	{
	 arming(); 
	}
  }
  
  return 0 ;
}
