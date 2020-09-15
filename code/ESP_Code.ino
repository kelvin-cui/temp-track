#include <ESP8266WiFi.h>            //-------------------------Define use of ESP8266 Wifi Library for Wifi Setup               
#include <ESP8266WebServer.h>
#include <stdlib.h>                 //-------------------------Define use of stdlib to convert char arrays to float type


const byte numChars = 32;           //-------------------------Allocate Char array for recieved data from arduino
char receivedChars[numChars];

boolean newData = false;            //-------------------------Boolean to mark when new data is recieved

float temp;                         //-------------------------Float to store temp from Arduino

int bled = 2;                       //-------------------------Initalize blue LED on Pin 2(GPIO2) and onboard LED on pin 1(onboard)
int led = 1;


//-----------------------BASIC WIFI SETTINGS-------------------------//
const char* ssid = "SSID"; //Change to reflect local network
const char* password = "YOURPASS";   //Ditto

const char* host = "api.thingspeak.com"; //Domain to Server
const char* path = "/update?api_key=PO0IANZGAXMFCJMV&field1="; //Path of Server
int refreshtime = 15; //Make new HTTPS request after x seconds

void setup() {                    //------------------------------Setup, run once when powered on
  Serial.begin(115200);                  //--------------------------For Commmunication with Arduino 
  pinMode(bled, OUTPUT);                 //--------------------------Set LED pins as outputs
  pinMode(led, OUTPUT);

  WiFi.mode(WIFI_STA);                   //--------------------------Set ESP to Station mode, instead of Access Point
  WiFi.begin(ssid, password);            //--------------------------Connect to the Wifi network as defined in BASIC WIFI SETTINGS
  
  
  while (WiFi.status() != WL_CONNECTED) {//--------------------------Waiting for Wifi Connection
    digitalWrite(bled, HIGH);            //--------------------------Flash the Blue LED to indicate connecting...
    delay(500);
    Serial.print("Connecting..");
    digitalWrite(bled, LOW);
    delay(500);
  }
}

void loop() {                     //------------------------------Loop, keep running while on
  if (WiFi.status() == WL_CONNECTED) { //----------------------------While Wifi is connected
    recvTemp();                        //----------------------------Function to recieve temp from arduino
    sendTemp();                        //----------------------------Function to send temp to the server
  }
}

void recvTemp() {                 //------------------------------Function for recieving temperature float over serial
    static byte ndx = 0;               //------------------------------Set char index to zero 
    char endMarker = '\n';             //------------------------------Set "end marker" to newline symbol 
    char rc;                           //------------------------------Allocate rc char for recieved char
   
    if (Serial.available() > 0) {      //------------------------------If Serial is connected
        rc = Serial.read();            //------------------------------Read the character sent, and save to rc

        if (rc != endMarker) {         //------------------------------If the recieved character isn't a newline 
            receivedChars[ndx] = rc;   //------------------------------Add the recieved character to our character array
            ndx++;                     //------------------------------Increase char index by one
            if (ndx >= numChars) {     //------------------------------Code to ensure that recieved data isn't longer than the character array we defined - not an issue in our use case can ignore
                ndx = numChars - 1;    
            }
        }
        else {                          //------------------------------If the recieved character is a newline, marking the end of a transmission
            receivedChars[ndx] = '\0';  //------------------------------End the char array with \0 that tells arduino that the char array ends
            ndx = 0;                    //------------------------------Set char index back to zero
            newData = true;             //------------------------------Set the newData boolean to true, letting the sendTemp function know that this data hasn't been sent before
        }
    }
}

void sendTemp() {                 //---------------------------------Function for sending temps to server
    if (newData == true) {           //----------------------------------If this data hasn't already been sent

        temp = 0.0;                  //----------------------------------Reset the temp float 
        temp = atof(receivedChars);  //----------------------------------Convert the recieved data to a float type from char array, store in "temp"
        updatetemp(temp);            //----------------------------------Run updatetemp helper function
        newData = false;             //----------------------------------Mark Data as sent, so sendTemp doesn't send it again
        
    }
}


void updatetemp(float temp){       //---------------------------------Helper function for sending data to server
  WiFiClient httpsClient;             //---------------------------------Start Wifi Client
  httpsClient.setTimeout(15000);      //---------------------------------Set Timeout to 15seconds
  delay(1000);
  int retry = 0;                      //---------------------------------Set retry counter to zero
  while ((!httpsClient.connect(host, 80)) && (retry < 15)) { //----------While the client hasn't connected to the host, and there has been less than 15 retries
    delay(1000);                                             //----------Wait one second then try again
    retry++;                                                 //----------Increase the retry counter
  }
  if (retry == 15) {                                         //----------If 15 retries are attempted and still no connection
    Serial.println("Connection failed");                     
    digitalWrite(led, HIGH);                                 //----------Flash onboard LED signifying no connection made
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
  }
  else {                                                      //---------If Conenction Successful
    Serial.println("Connected to Server");
    digitalWrite(bled, HIGH);                                 //---------Turn on Blue LED, signaling start of data transmission
  }
  httpsClient.print(String("GET ") + path + String(temp) +    //---------Send a GET request, with temp in the url
                    " HTTP/1.1\r\n" +
                    "Host: " + host +
                    "\r\n" + "Connection: close\r\n\r\n");
  while (httpsClient.connected()) {                           //----------Recieve Data sent from Server (not used in our case, server doesn't send any data)
    
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
  
  delay(1000);                                                //-----------Wait one second, before closing the client
  httpsClient.stop();
  digitalWrite(bled, LOW);                                    //-----------Turn off the Blue LED, signifying end of data transmission
}
