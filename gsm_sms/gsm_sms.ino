/* This sketch is for use with Neoway M590 gsm module and based on Neoway M590 AT command set
 *  
 *  connection 
 *      M590 - Arduino
 *      I -- not used
 *      T -- 2
 *      R -- 3
 *      V -- not used
 *      K is boot pin, so connect to ground by shorting with the next G pin or connect to Arduino ground
 *      
 *      GND -- GND
 *      
 *      +5V -- 5V or external power(recommended)
 *      You may need to use external power as M590 has current draw of about 2A
 */

#include <SoftwareSerial.h>

SoftwareSerial gsm(2,3); //RX on 2, TX on 3

boolean con = true;
int x = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  gsm.begin(9600);   //change according to your modules baud rate
  while(!Serial){
    
  }
  gsm.println("AT"); //Send AT command and wait for response
  delay(500);
  if (gsm.available()){
    String resp = gsm.readString();
    if (resp == "AT\r\r\nOK\r\n"){   //check if response matches expected output
      con = true;
      Serial.println("Module connect OK");
    } else {
      Serial.println("Unable to connect");
    }
  }
  while (!con){   // sketch will not continue unless previous response matches ie. it will loop here forever
    if ( x < 1){
      Serial.println("Please check connection");
      x++;
    }
  }
    delay(2000);
    setStat(); //check function below for description
    
    gsmRssi();  //check signal strength
    delay(2500);
    //sendSms("+254707706806","hello world"); // replace with your message and number NB

}

void loop() {
  // put your main code here, to run repeatedly:
  serialHandler();
  

}


void gsmRssi(){   // the rssi function
  gsm.println("AT+CSQ");
  if (gsm.available()){
    String rssi = gsm.readString();
    Serial.println(rssi);
  }
  
}

 void sendSms(String number, String text){    //send sms function
    String adrr("AT+CMGS=\""+number+"\"");
    gsm.print(adrr); 
    delay(500);
    gsm.write(0x0d); //send carriage return  \r
    delay(500);
    gsm.println(text);
    delay(1000);
    gsm.write(0x1a);  //send Ctrl-Z
    
  }

  void setStat(){
    gsm.println("AT+CMGF=1");  // set SMS mode to text
    Serial.println("set SMS mode to txt"); 
    delay(1500);
    gsm.println("AT+CSCS=\"GSM\""); // set SMS mode to text
    delay(1500);
    Serial.println("blurt out contents of new SMS upon receipt to the GSM shield's serial out"); 
    gsm.println("AT+CNMI=2,2,0,0,0"); 
    delay(1500);
  }

  void serialHandler(){
    if (gsm.available()){
    Serial.println(gsm.readString());
  }                                             
  if (Serial.available()){
    gsm.println(Serial.readString());
  }
  }
