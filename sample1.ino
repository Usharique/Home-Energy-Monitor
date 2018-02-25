#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <ArduinoJson.h>
double volts;
double amps;
double watts;
double prew=0; 
double count;
double precount=0;
 
void setup() {
 
  Serial.begin(115200);                                  //Serial connection
  WiFi.begin("nokia", "vivek123");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
 
}
 
void loop() {
 
 if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
 //Serial.println("connected");
   HTTPClient http;    //Declare object of class HTTPClient
   //int sensor = digitalRead(sensorPin1);
   //int sensor1 = digitalRead(sensorPin2);
   //int sensor2 = digitalRead(sensorPin3);
   while(Serial.available()){
    Serial.println("got");
    
    
     // '*' marks the beginning of a transmission
      bool start = Serial.find('*');

      //parse out the floats
      if(start){
        volts = Serial.parseFloat();
        amps = Serial.parseFloat();
        watts = Serial.parseFloat(); 
        count = Serial.parseFloat(); 
        
      }
      
   String voltage=(String)volts;
   String ampere=(String)amps;
   String power=(String)watts;
   
   if(prew!= watts)
   {
     if(abs(prew-watts)>15)
  {
    Serial.println("send");
   http.begin("http://192.168.43.239/setting.php?tot_load="+power+"&volt="+voltage+"&amp="+ampere);      //Specify request destination
   http.addHeader("Content-Type", "text/plain");  //Specify content-type header
   Serial.flush();
  // delay(2000);
  prew=watts; 
   Serial.println("send");
   
   int httpCode = http.GET();   //Send the request
   String payload = http.getString();                  //Get the response payload

   Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payloa
   http.end();  //Close connection
   }

   }
 if(precount!=count)  
{  
 
  String unit="99";
 http.begin("http://192.168.43.239/blink.php?unit="+unit);      //Specify request destination
   http.addHeader("Content-Type", "text/plain");  //Specify content-type header
   Serial.flush();
  // delay(2000);
  precount=count; 
   Serial.println("unit send");
   
   int httpCode = http.GET();   //Send the request
   String payload = http.getString();                  //Get the response payload

   Serial.println(httpCode);   //Print HTTP return code
   Serial.println(payload);    //Print request response payloa
   http.end();  //Close connection

  
  
  
}
   
  }
 }else{

    Serial.println("Error in WiFi connection");   

 }

 // delay(30000);  //Send a request every 30 seconds

 
}
