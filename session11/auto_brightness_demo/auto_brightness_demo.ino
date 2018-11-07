/*
	In this demo, we'll be using photoresistor input to control the brightness of an LED. 
  Adapted from sparkfun.com demo 
*/
const int sensorPin = P6_0;
const int ledPin = P2_5;

const int window = 10;
const int period = 1; // in milliseconds

int pastObs[window]; 
int curObs;
int LEDVoltage;

void setup()
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); //Setting up output for LED
}

/*
 * Takes the average of the past 10 light level readings
 * Implements a simple moving average filter to smooth out
 * the voltage response. 
 */
int getAvgObs() {
  float sum = 0; 
  for (int i = 0; i < window - 1; i++) {
    sum += (float) pastObs[i];
  }
  float avg_obs = sum / (float) window;
  return (int) avg_obs; 
}

/* 
 *  Adds the current light level to the array of past 
 *  light levels. 
 */
void addLastObs(int lightLevel, int lightLevels[10]) {
  int last = lightLevels[0]; 
  int cur; 
  
  for (int i = 1; i < 9; i++) {
    cur = lightLevels[i];
    lightLevels[i] = last;
    last = cur; 
  }

  lightLevels[0] = lightLevel;
}

/*
 * Converts a photoresistor level to an LED voltage between 0 to 255. *  
 */
int getLEDVoltage(int obs) {
  int min_obs = 0; // TODO: Change to actual values
  int max_obs = 0; //TODO: Change to actual values

  int min_controls = 0;
  int max_controls = 255;
  float range_controls = max_controls - min_controls;

  // Rescales the min_obs to max_obs range to be out of 255
  float diff = float(obs - min_obs + 1);
  float range = float(max_obs - min_obs);
  
  int led_voltage = (int) (diff/range * range_controls + min_controls);
  return min(max(led_voltage, min_controls), max_controls); 
}

/*
 * Implementation of our LED controller
 */
int LEDController(int obs) {  
  addLastObs(obs, pastObs); 
  int avg_obs = getAvgObs(); 
  int LEDVoltage = getLEDVoltage(avg_obs);
  return LEDVoltage;
}

void loop()
{
  /* Step 1) Build Circuit
   * Step 2) Find min and max photoresistor readings by covering and uncovering the photoresistor. 
   *         You can read the photoresistor values by opening the Serial Monitor in Tools > Serial Monitor
   *         Fill in min and max values in getLEDVoltage.
   * Step 3) Experiment with the period and window parameters (at the top of the file)
   *         to change how the LED responds to changes in photoresistor readings.
   *         Can you make the response:
   *         a) almost instaneous?
   *         b) lags behind the photoresistor just a little ?
   *         c) ignore relatively fast changes of photoresistor?
   */

  // Reading Observation
  curObs = analogRead(sensorPin); //Reading photoresistor value from analogRead
  Serial.print("Observation: ");
  Serial.println(curObs);

  // Computing Controls
  LEDVoltage = LEDController(curObs); 

  // Applying Controls
  analogWrite(ledPin, LEDVoltage); //analogWrite takes in a pin number and a value between 0 and 255   
  Serial.print("Controls: ");
  Serial.println(LEDVoltage);

  // Pausing for a brief moment
  delay(period);
}
