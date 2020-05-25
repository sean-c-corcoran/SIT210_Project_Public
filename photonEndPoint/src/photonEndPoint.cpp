/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/sean/code/Particle/photonEndPoint/photonEndPoint/src/photonEndPoint.ino"
/*
 * Project photonEndPoint
 * Description:
 * Author: Sean Corcoran
 * Date: 16/05/05
 * Version: 1.3
 */

void setup();
void loop();
#line 9 "/Users/sean/code/Particle/photonEndPoint/photonEndPoint/src/photonEndPoint.ino"
String deviceID; // <- used to hold the data of the particl's device name.
String fakeData = "Hello_I_Am_Fake_Data";
int moistureSensor = A1; // <- moisture sensor
int lightSensor = A2;    // <- light sensor
int moistureAvg;
int lightAvg;
int numOfSamples = 4; // amount for samples to take of sensor readings (samples are taken 1 second apart)

void setup()
{
  // pin IO declarations
  pinMode(moistureSensor, INPUT); // <- input mode moisture
  pinMode(lightSensor, INPUT);    // <- input mode light

  WiFi.on();                  //make sure wifi is on
  deviceID = WiFi.hostname(); // get the device ID which is the same as it's hostname

  Particle.variable("DeviceID", deviceID);               // <-device ID
  Particle.variable(deviceID + "_f", fakeData);          // <-fake var using fakedate for testing (sets the device ID plus _F as the name)
  Particle.variable(deviceID + "_m", &moistureAvg, INT); // <-var for the moisture mosAvg (sets the device ID plus _m as the name)
  Particle.variable(deviceID + "_l", &lightAvg, INT);    // <-var for the light mosAvg (sets the device ID plus _l as the name)
}

void loop()
{
  // reset all values before resampling

  float moistureSamples[numOfSamples], sumOfMoisture = 0.0, mosAvg = 0; // sample moisture sensor for 30 seconds
  float lightSamples[numOfSamples], sumOfLight = 0.0, lightA = 0;       // sample light sensor for 30 seconds

  /* The below builds an array of readings over a period of 30 seconds at one reading a second */
  for (int i = 0; i < numOfSamples; ++i)
  {
    // set each item in the array to a reading from the sensors
    moistureSamples[i] = analogRead(moistureSensor);
    lightSamples[i] = analogRead(lightSensor);
    // add up all the items in the arrays
    sumOfMoisture += moistureSamples[i];
    sumOfLight += lightSamples[i];
    // hardcoded to wait 1 second before getting another sample
    delay(1000);
  }
  // cal the avgs and update the public vars
  mosAvg = sumOfMoisture / numOfSamples; // cal the avg from the samples taken
  lightA = sumOfLight / numOfSamples;    // cal the avg from the samples taken
  moistureAvg = mosAvg;
  lightAvg = lightA;
}