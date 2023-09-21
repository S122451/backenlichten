#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Wire.h>

Adafruit_BME280 bme; // I2C

//Variables
float RH;  //Stores humidity value
float T; //Stores temperature value

void setup() {
   
  bool status;

  Serial.begin(9600);
  Serial.println("BME280 sensor testing");
  
  status = bme.begin(0x76);  // The I2C address of the sensor is 0x76
  
  if (!status) {             // Loop if sensor not found
    Serial.println("Error. Check");
    Serial.println("connections");
    while (1);
  } 
  
}

void loop() {
  // read humidity of sensor
  RH = bme.readHumidity();
  // read temperature of sensor
  T = bme.readTemperature();
  // dew point lager dan 2.5 graden dan de temperatuur = mist is aanwezig
  // put your main code here, to run repeatedly:
  TD =243.04*(log(RH/100)+((17.625*T)/(243.04+T)))/(17.625-log(RH/100)-((17.625*T)/(243.04+T)));
  
  delay(2000);
  
}
