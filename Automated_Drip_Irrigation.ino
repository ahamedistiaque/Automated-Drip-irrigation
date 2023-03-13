#include <OneWire.h>
#include <DallasTemperature.h>

// Temperature sensor V2
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float Celcius=0;
float Fahrenheit=0;



const int dry = 520; // value for dry sensor
const int wet = 260; // value for wet sensor


// Water flow meter
/*
Arduino Water flow meter
YF- S201 Hall Effect Water Flow Sensor
Water Flow Sensor output processed to read in litres/hour
*/
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;

bool flag = 1;

// Temperature sensor V2
void tempSen()
{
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print("Soil Temperature - C  ");
  Serial.println(Celcius);
  //Serial.print(" F  ");
  //Serial.println(Fahrenheit);
  delay(1000);
}

// Soil Moisture Sensor
void soilMoisture()
{
  int sensorVal = analogRead(A0);

  // Sensor has a range of 239 to 595
  // We want to translate this to a scale or 0% to 100%
  // More info: https://www.arduino.cc/reference/en/language/functions/math/map/
  int percentageHumididy = map(sensorVal, wet, dry, 100, 0); 
  Serial.print("Soil Moisture ");
  Serial.print(percentageHumididy);
  Serial.print("% - ");

  if(percentageHumididy < 30)
  {
    Serial.println("Watering the plant!  ");
    digitalWrite(12,LOW); // RELAY ON   
    delay(1000); 
  }

  else if(percentageHumididy > 30 && percentageHumididy < 50)
  {
    Serial.println("Wet!  ");
  }

  else if(percentageHumididy > 50)
  {
    Serial.println("Very Wet!  ");
    digitalWrite(12,HIGH); // RELAY OFF   
    delay(1000);
  }
  
  delay(1000);
}


// Water flow meter
void flow () // Interrupt function
{
flow_frequency++;
}

// 

void waterFlow()
{
  currentTime = millis();
  // Every second, calculate and print litres/hour
  if(currentTime >= (cloopTime + 1000))
  {
    cloopTime = currentTime; // Updates cloopTime

    // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
    l_hour = (flow_frequency * 60 / 7.5);
    // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour

    flow_frequency = 0; // Reset Counter
    Serial.print(l_hour, DEC); // Print litres/hour
    Serial.println(" L/hour");
  }
}





void setup() 
{
  //Solenoid Valve
  pinMode(12,OUTPUT); // RELAY PIN   
  digitalWrite(12,HIGH); // Normally ON Only For Chanies Relay Module 
  
  // Water flow meter
  pinMode(flowsensor, INPUT);
  digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
 
  // Temperature sensor V2
  Serial.begin(9600);
  sensors.begin();

  // Water flow meter
  attachInterrupt(0, flow, RISING); // Setup Interrupt
  sei(); // Enable interrupts
  currentTime = millis();
  cloopTime = currentTime;
}

void loop() {
  if(flag == 1)
  {
    Serial.println("=================================================");
    Serial.println("Developer: Istiaque Ahamed and Shartaz Khan Akash");
    Serial.println("=================================================");
    flag = 0;
  }
  // Temperature sensor V2
  tempSen();
  // Soil Moisture Sensor
  soilMoisture();
  // Water flow meter
  waterFlow();

   Serial.println("=============================");
}
