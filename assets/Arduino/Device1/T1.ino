 
//libraries 
#include <MQTTClient.h>     //  MQTT messaging protocol library!
#include <ArduinoJson.h>    // sending data with Json format library!
#include <WiFi101.h>        // conecting mkr1000 to internet library!
#include <dht.h>            // temperature and humidity sensor library!

//wifi parameters
char WIFI[]     = "^your ssid^";            // your wifi name
char PASSWORD[] = "^your password^";    // your wifi password

//cloud parameters
char CloudServer[]     = "api.artik.cloud";                                             // cloud server
int  MQTTPort          = 8883;                                                          // MQTT protocol port
char Did[]             = "^your device id^";                                            // your device id
char Dtoken[]          = "^your device token^";                                         // your device token
char CloudDataOut[]    = "/v1.1/messages/^your device id^";              
char ClientName[]      = "ARTIK-Arduino";                                               // your client's name

char buf[128];   // data storage in buf before going to cloud!

//building objects
WiFiSSLClient ipCloudStack;
MQTTClient mqttCloudClient;
dht DHT;


void setup() 
{

  Serial.begin(9600);  
  WiFi.begin(WIFI, PASSWORD);                                              //starting wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("attempting connect to : ");
    Serial.println(WIFI);
  }
  Serial.println();
  Serial.println("WiFi connected");
  
  mqttCloudClient.begin(CloudServer, MQTTPort, ipCloudStack);             // connecting to cloud

  Serial.println("start ARTIK Cloud connect"); Serial.println();
  
  while (!mqttCloudClient.connect(ClientName, Did, Dtoken)) {             //connecting to device
    Serial.print("*");
    delay(500);    
  }

}


void loop() 
{

  Serial.println("sending data to cloud");
  sendToArtikCloud();                                     // sending data to cloud
  delay(60*1000);                                         // wait 60s!

}



void sendToArtikCloud() 
{
  
  loadBuffer();                                    // making data ready in Json format
  mqttCloudClient.publish(CloudDataOut, buf);      // sending data to cloud

}


void loadBuffer()                                        // this function make data redy for sending in Json format!
{

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& dataPair = jsonBuffer.createObject();
  DHT.read11(A0);                                       // reading temperature & humidity
  dataPair["temp"] = DHT.temperature;                   // adding temperature value to Json
  dataPair["humidity"] = DHT.humidity;                  // adding humidity value to Json
  dataPair.printTo(buf, sizeof(buf));
  
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length)
{
  
  }
