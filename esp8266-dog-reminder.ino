/*
 *  Simple HTTP get webclient test
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266HTTPClient.h>


#define PUSH_INTERVAL_0 300000;
#define PUSH_INTERVAL_1 150000;

//constants
const int ledPin =  4;   
const int buttonPin = 5;
const char *host = "api.pushover.net";
const int httpsPort = 443;
//The api.pushover.net SHA1 fingerprint - this will periodically change.
const char fingerprint[] PROGMEM = "8C 6D 45 12 EC 9A 1C 50 8C 23 58 A8 0F AF 97 A1 C0 36 8B EE";
const String message = "The dog is outside!";


const char* ssid     = ""; //Your SSID
const char* password = ""; //Your wifi password
const String token = ""; //Your pushover token
const String user = ""; //Your pushover user

int switchStatus;            // status switch
int switchStatusLast = LOW;  // last status switch
int triggerTime = 0;

//forward delcarations
void startWifi();
void stopWifi();

void setup() 
{
  pinMode(ledPin, OUTPUT);      
  pinMode(buttonPin, INPUT_PULLUP);
  
  digitalWrite(ledPin, HIGH);
  
  Serial.begin(115200);
  delay(100);

  switchStatusLast = digitalRead(buttonPin);
  Serial.print("Setting switchStatus to ");
  Serial.println(switchStatus);

  triggerTime = millis() + PUSH_INTERVAL_0;
}

void loop() 
{
  switchStatus = digitalRead(buttonPin);
  if (switchStatus != switchStatusLast) 
  {
    Serial.println("Sleeping...");
    digitalWrite(ledPin, LOW);
    ESP.deepSleep(0);
  } 
  else 
  {
    if(millis() >= triggerTime) 
    {
      startWifi();
      
      Serial.println("Sending push");

      WiFiClientSecure httpsClient;

      httpsClient.setFingerprint(fingerprint);
      httpsClient.setTimeout(15000); // 15 Seconds
      
      Serial.print("HTTPS Connecting");
      int r=0; //retry counter
      while((!httpsClient.connect(host, httpsPort)) && (r < 30))
      {
          delay(100);
          Serial.print(".");
          r++;
      }
      if(r==30) 
      {
        Serial.println("Connection failed");
      }
      else 
      {
        Serial.println("Connected to web");
      }
      
      String post = String("token=")+token+"&user="+user+"&message="+message;
      
      String http = String("POST /1/messages.json HTTP/1.1\r\nHost: api.pushover.net\r\nConnection: close\r\nContent-Length: ")+post.length()+"\r\n\r\n"+post;
      httpsClient.print(http);
      
      Serial.println("request sent");
                
      while (httpsClient.connected()) 
      {
        String line = httpsClient.readStringUntil('\n');
        if (line == "\r") 
        {
          Serial.println("headers received");
          break;
        }
      }
      
      Serial.println("reply was:");
      Serial.println("==========");
      String line;
      
      while(httpsClient.available())
      {        
        line = httpsClient.readStringUntil('\n');  //Read Line by Line
        Serial.println(line); //Print response
      }
      
      Serial.println("==========");
      Serial.println("closing connection");
      
      triggerTime = millis() + PUSH_INTERVAL_1;

      //Turn off the wifi until we need it again to save battery
      stopWifi();
    }
  }
}

void stopWifi() 
{
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  Serial.println("WiFi is down");
}
  
void startWifi() 
{
    WiFi.begin(ssid, password);
  
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    //Blink the LED when the wifi connects
    for(int i = 0; i < 5; i++) {
    
      digitalWrite(ledPin, LOW);
      delay(100);
      digitalWrite(ledPin, HIGH);
      delay(100);
    }
  }
