#include <MKRWAN.h>
#include <TinyGPSPlus.h>
//#include "avdweb_VirtualDelay.h"
#include "ADS1X15.h"

#define lamp1 0
#define lamp2 1
#define lamp3 2
#define lightSensor1 A0
#define lightSensor2 A1
#define lightSensor3 A2

//bool autoset = false;

TinyGPSPlus gps;
LoRaModem modem;

unsigned long wachttijd = 60000;
//unsigned long wachttijd = 120000;
unsigned long previousMillis = 0;



ADS1115 ADS(0x48);


#include "arduino_secrets.h"

String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

int lampenStatus = 0;

bool lampstatus1 = 0;
bool lampstatus2 = 0;
bool lampstatus3 = 0;

int lichtwaarde = 0;

int seconds = 0;
int minutes = 0;

String msg = "";

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);

  if (!modem.begin(EU868)) {
    //Serial.println("Failed to start module");
    while (1) {}
  };
/*   Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());
 */
  pinMode(lamp1, OUTPUT);
  pinMode(lamp2, OUTPUT);
  pinMode(lamp3, OUTPUT);

  digitalWrite(lamp1,LOW);
  digitalWrite(lamp2,LOW);
  digitalWrite(lamp3,LOW);

  modem.configureClass(CLASS_C);
  delay(100);
  modem.dutyCycle(true);
  delay(50);

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    //Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }

/*   while (!connected){
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    connected = modem.joinOTAA(appEui, appKey);
  }  */

  //ADS.begin();
  //Serial1.begin(9600);
}

void loop() {

  unsigned long tijd = millis();

   if (Serial1.available() > 0) {
    if (gps.encode(Serial1.read())) {
      seconds = gps.time.second();
      minutes = gps.time.minute();
      Serial.println(seconds);
    }
  } 

  if (lampenStatus == 1) {
    //Serial.println(lampenStatus);
    //lees waarde lichtsensor
    int lichtlamp1 = 0;
    int lichtlamp2 = 0;
    int lichtlamp3 = 0;
    int dagnacht = 0;

    for (int i=0;i<25;i++){
    lichtlamp1 += analogRead(A0);
    //Serial.println(lichtlamp1);
    lichtlamp2 += analogRead(A1);
    //Serial.println(lichtlamp2);
    lichtlamp3 += analogRead(A2);
    //Serial.println(lichtlamp3);
    dagnacht += analogRead(A3);
    //Serial.println(dagnacht);
    //lees dag nacht in
    }

    lichtlamp1 = lichtlamp1/25;
    lichtlamp2 = lichtlamp2/25;
    lichtlamp3 = lichtlamp3/25;
    lichtwaarde = dagnacht/25;
    //Serial.println(dagnacht);

/*     Serial.print("waarde 1: ");
    Serial.println(lichtlamp1);
    Serial.print("waarde 2: ");
    Serial.println(lichtlamp2);
    Serial.print("dagnacht: ");
    Serial.println(dagnacht); */


    //lees waarde stroomsensor
    //ADS.setGain(0);
    //float stroomlamp1 = 200;
    //float stroomlamp2 = 200;
    //float stroomlamp3 = 200;


    if (lichtlamp1 < 400) {
      lampstatus1 = 0;  //0 = error op lamp
    } else {
      lampstatus1 = 1;
    }

    if (lichtlamp2 < 400) {
      lampstatus2 = 0;  //0 = error op lamp
    } else {
      lampstatus2 = 1;
    }

    if (lichtlamp3 < 400) {
      lampstatus3 = 0;  //0 = error op lamp
    }

    else {
      lampstatus3 = 1;
    }
    //maak een bricht aan met de statussen van de lamp
    msg = String(lampenStatus) + ":" + lampstatus1 + ":" + lampstatus2 + ":" + lampstatus3 + ":" + String(lichtwaarde);
    lampstatus1 = 0;
    lampstatus2 = 0;
    lampstatus3 = 0;
  }

  else {
    //Serial.println(lampenStatus);
    int dagnacht = 0;
    for (int i=0;i<25;i++){
    dagnacht += analogRead(A3);
    }

    lichtwaarde = dagnacht/25;
    msg = String(lampenStatus) + ":" + lampstatus1 + ":" + lampstatus2 + ":" + lampstatus3 + ":" + String(lichtwaarde);
  }

  /*  if (autoset == true) {
    //lees of dag nacht is in
    int dagnacht = analogRead(A4);

    //actie voor nacht
    if (dagnacht < 400){
    Serial.println("Turning lights on...");
    digitalWrite(lamp1, HIGH);
    digitalWrite(lamp2, HIGH);
    digitalWrite(lamp3, HIGH);
    lampenStatus = 1;
    }

    //actie voor dag
    if (dagnacht > 500){
    Serial.println("Turning lights off...");
    digitalWrite(lamp1, LOW);
    digitalWrite(lamp2, LOW);
    digitalWrite(lamp3, LOW);
    lampenStatus = 0;
    }


  } */


  // if (minutes%2 && seconds == 0){
   if (tijd - previousMillis >= wachttijd) {
    bool send = true;
    if (send){
    int err;
    modem.beginPacket();
    modem.print(msg);
    err = modem.endPacket(true);
    if (err > 0) {
      Serial.println("Message sent correctly!");
      send = false;
    } else {
      Serial.println("Error sending message");
    }
    }
    previousMillis = tijd;
  } 

  if (!modem.available()) {

    //Serial.println("No downlink message received at this time.");

    return;

  }
  

  char rcv[64];

  int i = 0;

  while (modem.available()) {

    rcv[i++] = (char)modem.read();
  }

  Serial.print("Received: ");

  for (unsigned int j = 0; j < i; j++) {

     Serial.print(rcv[j] >> 4, HEX);

    Serial.print(rcv[j] & 0xF, HEX);

    Serial.print(" "); 
  }

  Serial.println();
  rcv[i] = '\0';

  CheckMessage(rcv);
}

void CheckMessage(String message) {
  if (message == "LA0") {
    Serial.println("Turning lights off...");
    digitalWrite(lamp1, LOW);
    digitalWrite(lamp2, LOW);
    digitalWrite(lamp3, LOW);
    lampenStatus = 0;
    //autoset = false;

  } else if (message == "LA1") {
    Serial.println("Turning lights on...");
    digitalWrite(lamp1, HIGH);
    digitalWrite(lamp2, HIGH);
    digitalWrite(lamp3, HIGH);
    lampenStatus = 1;
    //autoset = false;
  }

  /*   else if (message == "AUTO") {
    autoset = true;
  } */
}