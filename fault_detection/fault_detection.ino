#include<ESP8266WiFi.h>
#include "Gsender.h"

#pragma region Globals
const char* ssid="mintu";
const char* password="12345678";
uint8_t connection_state = 0;                 
uint16_t reconnect_interval = 1000;             
#pragma endregion Globals

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
    static uint16_t attempt = 0;
    Serial.print("Connecting to ");
    if(nSSID) {
        WiFi.begin(nSSID, nPassword);  
        Serial.println(nSSID);
    } else {
        WiFi.begin(ssid, password);
        Serial.println(ssid);
    }

    uint8_t i = 0;
    while(WiFi.status()!= WL_CONNECTED && i++ < 50)
    {
        delay(200);
        Serial.print(".");
    }
    ++attempt;
    Serial.println("");
    if(i == 51) {
        Serial.print("Connection: TIMEOUT on attempt: ");
        Serial.println(attempt);
        if(attempt % 2 == 0)
            Serial.println("Check if access point available or SSID and Password\r\n");
        return false;
    }
    Serial.println("Connection: ESTABLISHED");
    Serial.print("Got IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

void Awaits()
{
    uint32_t ts = millis();
    while(!connection_state)
    {
        delay(50);
        if(millis() > (ts + reconnect_interval) && !connection_state){
            connection_state = WiFiConnect();
            ts = millis();
        }
    }
}

 int ir1=D0;
 int ir1_app= 0.434;
 int CS;
 int var=0;
 int IR1=0;
 const int sensorIn=A0;
int mVperAmp=185;

double Voltage=0;
double Vrms=0;
double Ampsrms=0;

void setup() {
  // put your setup code here, to run once:
 
  pinMode(ir1,INPUT);
  pinMode(sensorIn,INPUT);
  Serial.begin(9600);
  WiFiConnect();
  
  

}

void loop() {
  // put your main code here, to run repeatedly:
   Voltage=getVPP();
  Vrms=(Voltage/2)*0.707;
  Ampsrms=((Vrms*1000)/mVperAmp)-0.2;
  Serial.print(Ampsrms);
  Serial.println("Ampsrms");
}
float getVPP()
{
  float result;
  
    int readvalue;
    int maxvalue=0;
    int minvalue=1024;

    uint32_t start_time=millis();
    while((millis()-start_time)<1000)
    {
      readvalue=analogRead(sensorIn);
      if (readvalue>maxvalue)
      {
        maxvalue=readvalue;
      }
      if (readvalue<minvalue);
      {
        minvalue=readvalue;
      }
    }
 
  result=((maxvalue-minvalue)*5.0)/1024.0;
  
  return result;
   IR1=digitalRead(ir1);
   Serial.println(IR1);
   if(IR1==HIGH)
   var=var+ir1_app;

   
 
   
  if(result<var)
  {
    Serial.begin(115200);
    connection_state = WiFiConnect();
    if(!connection_state)  // if not connected to WIFI
        Awaits();          // constantly trying to connect

    Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
    String subject = "Subject is optional!";
    if(gsender->Subject(subject)->Send("anishaagrawalindian@gmail.com", "There is a fault in a electric appliance")) {
        Serial.println("Message send.");
    } else {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError());
    }
   }
   
   

  
  
  

}
