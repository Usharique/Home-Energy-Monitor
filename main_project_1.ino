#include "EmonLib.h"         

#define SAMPLE_COUNT 5
#define VOLT_CAL 148.7    //voltage calibration
#define CURRENT_CAL1 62.6 //sensor 1 calibration
#define CURRENT_CAL2 62.8 //sensor 2 calibration
double a=0,count=0;

//create 2 instances of the energy monitor lib
EnergyMonitor emon1;             
EnergyMonitor emon2;           

//arrays to hold the sample data

const int currentPin1 = 2;
const int currentPin2 = 1;
const int voltagePin = 3;

//counter to keep track of the current sample location
int counter = 0;
double prew=0;
double precount=0;




void setup()

{  
  Serial.begin(115200);
  //Serial1.begin(115200);
  
  emon1.voltage(voltagePin, VOLT_CAL, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon1.current(currentPin1, CURRENT_CAL1);       // Current: input pin, calibration.

  emon2.voltage(voltagePin, VOLT_CAL, 1.7);  // Voltage: input pin, calibration, phase_shift
  emon2.current(currentPin2, CURRENT_CAL2);       // Current: input pin, calibration.
}

void loop()
{
  
  a=analogRead(A0); 
   if(a<1000)
  {
   digitalWrite(7,HIGH);
        count++;
       // delay(1000);
   digitalWrite(7,LOW); 
  }

  emon1.calcVI(20,40);
  a=analogRead(A0); 
   if(a<1000)
  {
     digitalWrite(7,HIGH);
        count++;
        delay(1000);
   digitalWrite(7,LOW); 
  }
   emon2.calcVI(20,40);
   a=analogRead(A0); 
   if(a<1000)
  {
    digitalWrite(7,HIGH);
        count++;
        delay(1000);
   digitalWrite(7,LOW); 
  }
 

  //calculate the most recent readings
  //emon1.calcVI(20,100);         
 // emon2.calcVI(20,100);         

  //get the final average by dividing by the # of samples
 
  double a=emon1.Irms;
  double v=emon1.Vrms*1.5;
  double w=v*emon1.Irms;
  
  //send the power info to the ESP module through Serial1
  if(abs(prew-w)>15||(count!=precount))
  {
// sendPowerInfo (v, a, w,count);
Serial.print("*");
Serial.print(v);
Serial.print(",");
Serial.print(a);
Serial.print(",");
Serial.print(w);
Serial.print(",");
Serial.println(count);
 prew=w;
 precount=count;
 a=analogRead(A0); 
   if(a<1000)
  {
    digitalWrite(7,HIGH);
        count++;
        delay(1000);
   digitalWrite(7,LOW); 
  }
  }
//Serial.println(emon1.Irms);
//Serial.println(emon1.Vrms);
//int v=emon1.Vrms*1.5;
//int w=v*emon1.Irms;
//Serial.println(w);
  
  
}


//send the power info to the ESP module through Serial1 (comma separated and starting with *)
/*void sendPowerInfo(double Volts, double Amps, double Watts,double count){
 Serial.print("*");
Serial.print(Volts);
Serial.print(",");
Serial.print(Amps);
Serial.print(",");
Serial.print(Watts);
Serial.print(",");
Serial.println(count);
}*/
