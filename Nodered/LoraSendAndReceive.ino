#include <MKRWAN.h>
#include <TinyGPSPlus.h>
#include "avdweb_VirtualDelay.h"

#define lamp1 6
#define lamp2 7
#define lamp3 8
#define lightSensor1 A1
#define lightSensor2 A2
#define lightSensor3 A3
#define lightSensor4 A4

TinyGPSPlus gps;
LoRaModem modem;
VirtualDelay checkDelay;

#include "arduino_secrets.h"

String appEui = SECRET_APP_EUI;
String appKey = SECRET_APP_KEY;

int averageLightOnValue = 1000;
int lightSamples = 10;
int nightValue = 500;

int lampStatus = 2;
int lightValueLamp1 = 0;
int lightValueLamp2 = 0;
int lightValueLamp3 = 0;
int lightValueLamp4 = 0;
int currentSample = 0;
String lightMessage = "";

String gpsMessage = "";
String satalites = "0";
String latitude = "0";
String longitude = "0";

//Upload times
int timeValues[5][2] = {
  {06, 00},
  {12, 00},
  {18, 00},
  {00, 01},
  {} //Kan zelf gekozen worden en eventueel meerdere tijdsloten worden toegevoegd.
};
int timeDebounce = false;
int skipVirtualDelay = false;

int rebootTime[2] = {03, 00};

String codes = "";
//Codes
//#1 Lampen uit
//#2 Lampen aan

void resetFunc() {
  
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  
  while (!Serial);
  if (!modem.begin(EU868)) { //initialisatie van de LoRaWAN module
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your module version is: ");
  Serial.println(modem.version());
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());

  pinMode(lamp1, OUTPUT);
  pinMode(lamp2, OUTPUT);
  pinMode(lamp3, OUTPUT);
  pinMode(lightSensor1, INPUT);
  pinMode(lightSensor2, INPUT);
  pinMode(lightSensor3, INPUT);
  pinMode(lightSensor4, INPUT);

  //LoRaWAN configuratie
  modem.configureClass(CLASS_C);
  delay(100);
  modem.dutyCycle(true); 
  delay(50);

  //Verbinding maken met LoRaWAN
  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }

  //Timer starten voor eerste bericht
  checkDelay.start(10000);
  
  Serial.println("Enter a message to send to network");
  modem.minPollInterval(60);
}

void loop() {
  codes = ""; //Het array van de codes terug naar een lege string plaatsen

  //Afhankeijk van de bepaalde statussen codes toevoegen
  if(lampStatus == 0) {
    codes += "#2, ";
  }else if(lampStatus == 1) {
    codes += "#1, ";
  }
  
  int lightSensorValue1 = analogRead(lightSensor1);
  int lightSensorValue2 = analogRead(lightSensor2);
  int lightSensorValue3 = analogRead(lightSensor3);
  int lightSensorValue4 = analogRead(lightSensor4);

  //Serial.println(lightSensorValue1);

  //Een x aantal samples pakken zodat als er een verkeerde waarde tussen zou zitten deze niet te hard zou opvallen
  if(currentSample < lightSamples) {
    currentSample += 1;
    lightValueLamp1 += lightSensorValue1;
    lightValueLamp2 += lightSensorValue2;
    lightValueLamp3 += lightSensorValue3;
    lightValueLamp4 += lightSensorValue4;
  }else {
    currentSample = 0;
    lightMessage = "";

    //Serial.print("A:"); Serial.println(lightValueLamp1);
    //Serial.print("B:"); Serial.println(lightSamples * averageLightOnValue);

    //Individuele metingen om te kijken of de lampen branden of niet. Dit wordt dubbel gechecked door de stroommetingen toe te passen op de lampen
    if(lightValueLamp1 > lightSamples * averageLightOnValue) {
      lightMessage += "1:";
    }else {
      lightMessage += "0:";
    }

    if(lightValueLamp2 > lightSamples * averageLightOnValue) {
      lightMessage += "1:";
    }else {
      lightMessage += "0:";
    }
  
    if(lightValueLamp3 > lightSamples * averageLightOnValue) {
      lightMessage += "1:";
    }else {
      lightMessage += "0:";
    }
    
    lightValueLamp1 = 0;
    lightValueLamp2 = 0;
    lightValueLamp3 = 0;

    //Kijken dat de lamp in de automatische modus staat en deze dan aan en uit doen wanneer het dag of nacht is buiten
    if(lampStatus == 2) {
      if(lightValueLamp4 > lightSamples * averageLightOnValue) {
        digitalWrite(lamp1, LOW);
        digitalWrite(lamp2, LOW);
        digitalWrite(lamp3, LOW);
      }else {
        digitalWrite(lamp1, HIGH);
        digitalWrite(lamp2, HIGH);
        digitalWrite(lamp3, HIGH);
      }
    }
    
    lightValueLamp4 = 0;
  }
  
  //if(!Serial.available() and !modem.available()) return;
  if(!modem.available()) return;

  //GPS systeem
  gpsMessage = "";
  while (Serial1.available() > 0) {
    Serial.println("GPS Serial available");
    if (gps.encode(Serial1.read())) {
      Serial.print("Sat: "); Serial.println(gps.satellites.value());
      Serial.print("Date: "); Serial.println(gps.date.value());
      //Als er meer dan 0 satalieten gevonden zijn gaan we gebruik gaan maken van de GPS locatie en tijd zo niet dan gaan we gebruik maken van een timer die ervoor zorgt
      //dat er toch nog berichten worden verstuurd op een vast tijdstip
      if(gps.satellites.value() > 0) {
        satalites = String(gps.satellites.value());
        latitude = String(gps.location.lat());
        longitude = String(gps.location.lng());
  
        gpsMessage += satalites + ":";
        gpsMessage += latitude + ":";
        gpsMessage += longitude + ":";

        int hours = gps.time.hour();
        int minutes = gps.time.minute();

        //Wanneer de tijd gelijk is aan de reboot tijd dan zal de Maker 1300 opnieuw worden opgestart zodat we geen fouten krijgen
        //Door het gebruik van delays
        if(hours == rebootTime[1] and minutes == rebootTime[2]) {
          Serial.println("Auto restart now...");
          resetFunc();
        }

        //Deze 2 functies gaan kijken of we één van de ingestelde tijden hebben gepasseerd en ervoor zorgen dat die dan maar één keer een bericht gaat versturen en
        //dit niet blijft versturen
        for(int i = 0; i < sizeof(timeValues); i++) {
          if(timeValues[i][0] == hours && timeValues[i][1] == minutes) {
            if(timeDebounce) {
              return;
            }else {
              skipVirtualDelay = true;
            }
          }
        }

        if(not skipVirtualDelay) {
          timeDebounce = false;
          return;
        }
      }
    }
  }
  
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("ERROR: The GPS module has not been detected! Please check the wiring of the module and ensure it's connected correctly.");
    Serial.println("Sending last recorded data!");

    gpsMessage += "0:";
    gpsMessage += latitude + ":";
    gpsMessage += longitude + ":";
  }

  if(not skipVirtualDelay and !checkDelay.elapsed()) return;
  skipVirtualDelay = false;

  String msg = "";
  if(Serial.available()) {
    msg = Serial.readStringUntil('\n');
  }else {
    msg += lightMessage + gpsMessage + codes + "OK";
    checkDelay.start(66000);
    //checkDelay.start(300000);
  }

  //Onderstaande code wordt gebruikt om een beriht te gaan versturen en te gaan ontvangen en deze dan ontcijferen
  if(!modem.available()) {
    Serial.println();
    Serial.print("Sending: " + msg + " - ");
    for (unsigned int i = 0; i < msg.length(); i++) {
      Serial.print(msg[i] >> 4, HEX);
      Serial.print(msg[i] & 0xF, HEX);
      Serial.print(" ");
    }
    Serial.println();
  
    int err;
    modem.beginPacket();
    modem.print(msg);
    err = modem.endPacket(true);
    if (err > 0) {
      Serial.println("Message sent correctly!");
    } else {
      Serial.println("Error sending message :(");
      Serial.println("(you may send a limited amount of messages per minute, depending on the signal strength");
      Serial.println("it may vary from 1 message every couple of seconds to 1 message every minute)");
    }
    delay(1000);
  }
  
  if (!modem.available()) {
    Serial.println("No downlink message received at this time.");
    Serial.println("");
    Serial.println("");
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

  rcv[i] = '\0';
  Serial.println(" ");
  Serial.print("Message: " ); Serial.println(rcv);
  Serial.println(" ");
  CheckMessage(rcv);
  Serial.println("");
  Serial.println("Enter a message to send to network");
}

//Serial inputs en NodeRed communicatie commando's
void CheckMessage(String message) {
  if(message == "LA0") {
    lampStatus = 0;
    Serial.println("Turning lights off...");
    digitalWrite(lamp1, LOW);
    digitalWrite(lamp2, LOW);
    digitalWrite(lamp3, LOW);
  }else if(message == "LA1") {
    lampStatus = 1;
    Serial.println("Turning lights on...");
    digitalWrite(lamp1, HIGH);
    digitalWrite(lamp2, HIGH);
    digitalWrite(lamp3, HIGH);
  }else if(message == "LAA") {
    lampStatus = 2;
    Serial.println("Turning lights auto...");
  }else if(message == "REBOOT") {
    Serial.println("Rebooting...");
    resetFunc();
  }
}
