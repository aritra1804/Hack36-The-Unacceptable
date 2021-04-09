
//libraries 
#include <MQTTClient.h>     //  MQTT messaging protocol library!
#include <ArduinoJson.h>    // sending data with Json format library!
#include <WiFi101.h>        // conecting mkr1000 to internet library!

//wifi parameters
char WIFI[]     = "^your ssid^";            // your wifi name
char PASSWORD[] = "^your password^";    // your wifi password



//cloud parameters
char CloudServer[]     = "api.artik.cloud";                             // cloud server
int  MQTTPort          = 8883;                                          // MQTT protocol port
char Did[]             = "^your device id^";                            // your device id
char Dtoken[]          = "^your device token^";                         // your device token
char CloudDataOut[]    = "/v1.1/messages/^your device id^";              
char ClientName[]      = "ARTIK-Arduino";
char CloudActionsIn[] = "/v1.1/actions/^your device id^";               // (/v1.1/actions/"DEVICE ID") 


char buf[200]; // Json Data to Artik Cloud  

//building ubjects
WiFiSSLClient ipCloudStack; 
MQTTClient mqttCloudClient; // MQTT Protocol


int relayPIN=9;
int redled=7;
int greenled=8;

 
 
void setup() 
{
  pinMode(relayPIN,OUTPUT);
  pinMode(redled,OUTPUT);
  pinMode(greenled,OUTPUT);
  //providing low 3.3V for logic level converter
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
  Serial.begin(9600);  
  WiFi.begin(WIFI, PASSWORD);                                 //connecting to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("attempting connect to : ");
    Serial.println(WIFI);
  }
  Serial.println();
  Serial.println("WiFi connected");
  
  mqttCloudClient.begin(CloudServer, MQTTPort, ipCloudStack); // Connecting to cloud 
  while (!mqttCloudClient.connect(ClientName, Did, Dtoken))   // connecting to device
  { 
    Serial.print("not connected to device"); 
    delay(1000); 
  } 
mqttCloudClient.subscribe(CloudActionsIn); 

}



void loop()
{ 

  //waiting for actions
  Serial.println("listening...");
  mqttCloudClient.loop();
  delay(10000);
}


void messageReceived(String topic, String payload, char * bytes, unsigned int length) { 
  Serial.print("data from artik cloud:"); 
  Serial.println(payload);  
  parseBuffer(payload); 
 
}


void parseBuffer(String _payload)     //translating data from Json format
{
   
StaticJsonBuffer<200> jsonBuffer; 
String json = _payload; 
JsonObject& root = jsonBuffer.parseObject(json);
boolean ACS = root["actions"][0]["parameters"]["sw"];   //ACS = order from cloud

if (ACS)                         //turning on
{
  digitalWrite(relayPIN,LOW);
  digitalWrite(greenled,HIGH);
  digitalWrite(redled,LOW);  
}
else                            //turning off
{
  digitalWrite(relayPIN,HIGH);
  digitalWrite(greenled,LOW);
  digitalWrite(redled,HIGH); 
}

}  
