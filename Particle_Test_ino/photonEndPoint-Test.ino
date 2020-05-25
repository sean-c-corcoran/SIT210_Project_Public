/*
 * Project photonEndPoint
 * Description: The following takes readings from two analog sensors one a moisture sensor another a light sensor, 
 *              builds avgs over the course of 1 min then updates variables that sit on the particle cloud, 
 *              it also has a notification system, that publishes public events if the moisture level gets to low, it does have a set variable that allows 
 *              for the event to published at set time intervals.  
 * Author: Sean Corcoran
 * Date: 16/05/05
 * Version: 2.1
 */

String deviceID; // <- used to hold the data of the particle's device name.
String fakeData = "Hello_I_Am_Fake_Data";
int moistureSensor = A1; // <- Moisture sensor input pin
int lightSensor = A2;    // <- Light sensor input pin
int moistureRealTime;    // <- Real time moisture reading
int lightRealtime;       // <- Real time light reading
int moistureAvg;         // <- The Avg of the soil mosture after the set sampling time
int lightAvg;            // <- The Avg of then light sensor after the set sampling time
int numOfSamples = 59;   // <- Amount for samples to take of sensor readings (samples are taken 1 second apart) default should be set to 59 (1 min of sampling)

//long notificationInterval = 13200000; // 43200000 every 12 hours a new low water notification can fire
long notificationInterval = 1800000; // every 30 mins (60000 = 1 min) 1800000 = 30mins  #issue event notifcations at these time intervals
long triggerTime;                    // used to hold the value of the time of the last notification
bool firstTrigger = true;            // used to allow the first trigger and start the last trigger time

// Moisture Sensor (minimum values)
int soilWaterMin = 700; // <-- if the soil reading is below this amount a low water notification will be triggered as
int sensorMin = 100;    // <-- used to make sure there's at least some water before sending a notification (won't send notification if sensor is disconnected)

void setup()
{

  // pin IO declarations
  pinMode(moistureSensor, INPUT); // <- input mode moisture
  pinMode(lightSensor, INPUT);    // <- input mode light

  // Particle Cloud Accessable Variables
  Particle.variable("moistureValue", &moistureAvg, INT);               // <-var for the moisture Avg
  Particle.variable("lightValue", &lightAvg, INT);                     // <-var for the light Avg
  Particle.variable("moistureValue_realtime", &moistureRealTime, INT); // <-var for the moisture real time (used for testing)
  Particle.variable("lightValue_realtime", &lightRealtime, INT);       // <-var for the light real time (used for testing)
}

void loop()
{

  // reset all values before resampling
  float moistureSamples[numOfSamples], sumOfMoisture = 0.0, mosAvg = 0; // sample moisture sensor for the value of the numOfSamples
  float lightSamples[numOfSamples], sumOfLight = 0.0, lightA = 0;       // sample light sensor for the value of numOfSamples

  /* The below builds an array of readings over a period of required sampling time at one reading a second */
  for (int i = 0; i < numOfSamples; ++i)
  {
    // set each item in the array to a reading from the sensors
    moistureRealTime = analogRead(moistureSensor); // <-- Get Value from the sensor and set it as the realtime
    lightRealtime = analogRead(lightSensor);       // <-- Get Value from the sensor and set it as the realtime
    // build the array from the readings
    moistureSamples[i] = moistureRealTime;
    lightSamples[i] = lightRealtime;
    // add up all the items in the arrays
    sumOfMoisture += moistureSamples[i];
    sumOfLight += lightSamples[i];
    // hardcoded to wait 1 second before getting another sample
    delay(1000);
  }
  // cal the avgs and update the public vars
  mosAvg = sumOfMoisture / numOfSamples; // cal the avg from the samples taken
  lightA = sumOfLight / numOfSamples;    // cal the avg from the samples taken
  moistureAvg = 400;                     //set to 400 (for testing)                 // update the particle cloud vars
  lightAvg = lightA;

  notificationSys(); // check notification interval and send notification to listening parties
}

// ## If the avg of the moisture of the soil is getting low then tigger a publish event however wait a set time before sending another notifcation ##
void notificationSys()
{
  // if lower then the soil water min and above the dummy amount plus haven't issued a notification before
  // then send the event, plus start the last tigger sent time
  if (moistureAvg < soilWaterMin && moistureAvg > sensorMin && firstTrigger)
  {
    triggerTime = millis(); // set the tigger time from first fire
    //Particle.publish("WaterLow", deviceID, PRIVATE); //publish the low water event and send the device ID so the database can look it up if known
    Particle.publish("WaterLow", String(moistureAvg, DEC), PUBLIC); //publish the low water event and the current moisture avg for sending the notifcation
    firstTrigger = false;                                           //tigger has fired at least once
  }
  else if (moistureAvg < soilWaterMin && moistureAvg > sensorMin && (millis() - triggerTime) > notificationInterval) //send another notification as long as the time of the notification interval from the last update has passed.
  {
    triggerTime = millis(); //new time since fire
    //Particle.publish("WaterLow", deviceID, PUBLIC); //publish the low water event and send the device ID so the database can look it up if known
    Particle.publish("WaterLow", String(moistureAvg, DEC), PUBLIC); //publish the low water event and the current moisture avg for sending the notifcation
  }
  else if (moistureAvg < soilWaterMin && moistureAvg > sensorMin) // water level is still to low, however a notification was sent so don't send another one
  {
    // to soon don't refire
  }
  else
  {
    //do nothing plant has water
  }
}
