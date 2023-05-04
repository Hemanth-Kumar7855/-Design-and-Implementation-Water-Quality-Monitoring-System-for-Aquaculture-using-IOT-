#define BLYNK_TEMPLATE_ID "TMPLRUSurudx"
#define BLYNK_TEMPLATE_NAME "Temperature"
#define BLYNK_AUTH_TOKEN "Vqr7XANKT2V4V0cKwA2QxeWVsnt1HZwg"

#define BLYNK_PRINT Serial

#include <Blynk.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include<BlynkSimpleEsp32.h>
////////////////////////////////////////
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 25
OneWire oneWire(ONE_WIRE_BUS);	
// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);
//................................................
#define analogpin 2 
// //..........................................
#define analogpinph A4 ///ph sensor
int sensorval=0;
long int avgval;
float b;

int buf[10],temp ;
float calibration_value = 21.34 - 0.5;
//.....................................................................
char auth[] =BLYNK_AUTH_TOKEN;
char ssid[] = "OPPO F9 Pro";
char pass[] = "hemanth7855";

// Declaring a global variabl for sensor data
 //float tempsensorVal; 
// float turbsensorval;
// float phsensorval;
// int ultrasensorval;

// //........................................................................
const int trigPin = 16;
const int echoPin = 17;
// // defining variables
long duration;
int distance;
int tankheight=100;
int mydistance;

// This function creates the timer object. It's part of Blynk library 
BlynkTimer timer; 



void setup()
{
  sensors.begin();	// Start up the library
  Serial.begin(9600);
  //.................................................
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  //Connecting to Blynk Cloud
  Blynk.begin(auth, ssid, pass,"blynk.cloud",80); 
  // Setting interval to send data to Blynk Cloud to 1000ms. 
  // It means that data will be sent every second
  timer.setInterval(100L, mytemp); 
  timer.setInterval(100L, level);
  timer.setInterval(100L, myturb);
  timer.setInterval(100L, myph);
}
// void myTimer() 
// {
//   // This function describes what will happen with each timer tick
//   // e.g. writing sensor value to datastream V5
//   Blynk.virtualWrite(V5,tempsensorVal);
//   // Blynk.virtualWrite(V4,turbsensorval );
//   // Blynk.virtualWrite(V3, phsensorval);  
//   // Blynk.virtualWrite(V2, ultrasensorval);  
// }

void loop()
{
    // put your main code here, to run repeatedly:
    mytemp();
    delay(200);

    level();
    delay(200);

    myturb();
    delay(200);
    
     myph();
    delay(200);
    
  Serial.println("\n");
  // Reading sensor from hardware analog pin A0
  //tempsensorVal =analogRead(25); 
  // turbsensorval = analogRead(2);
  // phsensorval = analogRead(4);
  // ultrasensorval=distance;
      // Runs all Blynk stuff
  Blynk.run(); 
  
  
  // runs BlynkTimer
  timer.run(); 

}

 void mytemp(){
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.print("ºC |");
  Serial.print(temperatureF);
  Serial.println("ºF");

  
  Blynk.virtualWrite(V5, temperatureC);
  //Blynk.virtualWrite(V6, temperatureF);

  delay(1000);
  }

void level() {
// Clears the trigPin
 digitalWrite(trigPin, LOW);

// Sets the trigPin on HIGH state for 10 micro seconds
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
 duration = pulseIn(echoPin, HIGH);
// Calculating the distance
 mydistance= duration*0.034/2;
 distance = tankheight-mydistance;
// Prints the distance on the Serial Monitor
 Serial.print("Distance: ");
 Serial.print(distance);
 Serial.print("\t\t\t");

      if (distance>70&& distance<=100){
    Serial.print("\t The water level: FULL \n");
    }

      else if (distance<=70 && distance>30){
    Serial.print("\t The water level: NORMAL\n");
    }

      else if (distance<=30){
    Serial.print("\t The water level: LOW \n");
    } 
    Blynk.virtualWrite(V4, distance);
delay(1000);
}

void myturb(){
  int sensorValue = analogRead(analogpin);
  float voltage = (float(sensorValue) * (2.5 / 1024.0));
 
  Serial.print ("TURBIDITY VALUE OUTPUT (V):");
  Serial.print (voltage);
  Serial.println();
  
   if (voltage>4 ){
     Serial.print("\t Water is clear \n");
    }
   
  if (voltage<4 && voltage>3.5 ){
     Serial.print("\t Water is a little cloudy \n");
    }

   
  else if(voltage<3.5){
    Serial.print("\t Warning!!. Water is muddy/very cloudy!!! \n");
  }

  Blynk.virtualWrite(V3,voltage);  
    delay(1000);
}

float myph(){
  
  for(int i=0;i<10;i++){
    buf[i]= analogRead(analogpinph);
    delay(100);
    }
  for(int i=0;i<9;i++){
    for(int j=i+1;j<10;j++){
      if(buf[i]>buf[j]){
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;       
        
        }      
      }
       
    }  
  avgval=0;
  for(int i=2;i<8;i++){avgval+=buf[i];    }

  float phvol=(float)avgval*5.0/1024/6 ;
  float phval= -3.6585*phvol+calibration_value;   /// to calculate the ph of various substance
  Serial.print("pH Value:  ");
  Serial.println(phval);
   Serial.println("\n");

  //Serial.print("Voltage = ");
  //Serial.println(phvol);
  delay(1000);

  Blynk.virtualWrite(V2,phval); 
  
  }
