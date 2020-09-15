#include <Wire.h>           //-------------------------Define use of MLX90614 Temperature sensor library
#include <Adafruit_MLX90614.h>

#include <SoftwareSerial.h> //-------------------------Define use of Software Serial, setting RX to pin 4 and TX to pin 5
#define RXpin 4
#define TXpin 5
SoftwareSerial espSerial(RXpin, TXpin);


Adafruit_MLX90614 mlx = Adafruit_MLX90614(); //-------Initalize name of Tempsensor, and float to store temperature once taken
float temp = 0;


const int button = 21; //-----------------------------Initalize Button on Pin 21 (A3), as well as button state and reset variables  
int buttonState = 0;
int buttonReset = 0;


const int rled = 16;  //------------------------------Initalize Red LED on Pin 16, and Green LED on Pin 10.
const int gled = 10;

void setup() {        //------------------------------Setup, run once when powered on
  
  //Serial.begin(9600);    //-------------------------For communication with computer
  espSerial.begin(115200); //-------------------------For communication with ESP Wifi Module
  
  mlx.begin();             //--------------------------Start temperature sensor 

  pinMode(button, INPUT);  //--------------------------Set Button as an input

  pinMode(rled, OUTPUT);   //--------------------------Set LEDs as outputs
  pinMode(gled, OUTPUT);
  
}

void loop() {         //------------------------------Loop, keep running while on
  gettemp(); //--------------------------------------Loop gettemp function
}


void gettemp() {//-----------------------------------Main function, when button is pressed, gets temperature and stores into float temp, sets LED based on temp values, and sends to ESP Chip
  
  buttonState = digitalRead(button);             //------------------------------Read Button State (HIGH = Pressed Down, LOW = Not Pressed)

  if (buttonState == HIGH && buttonReset == 0){    //-----------------------------If the button is pressed, and the previous state was not pressed (reset == 0)
    temp = mlx.readObjectTempC();                   //---------------------------Get the temperature in C, and store to float "temp"
    //temp = mlx.readObjectTempF();                 //---------------------------Use this line for temperature in F
    
    setLED(temp);                                   //---------------------------Use the setLED helper function to change LED color based on temp
    
    //Serial.print("*C\tObject = ");                //---------------------------Use for Debugging the temperature read by sensor - Must uncomment Serial.begin in Setup for this to work
    //Serial.print(temp); 
    //Serial.println("*C");
    
    espSend(temp);                                  //---------------------------Use the espSend helper function to send the temp float to the ESP Chip
    
    buttonReset = 1;                                //---------------------------Set the previous state to pressed (reset = 1)
  } 
  
  else if (buttonState == LOW && buttonReset == 1){//----------------------------If the button isn't pressed, and the previous state was pressed (reset == 1)
    buttonReset = 0;                                //---------------------------Set previous state to not pressed (reset = 0)
  }
  delay(100);                                     //-----------------------------Pause for 100 Milliseconds
}

void setLED(float temp) { //-------------------------Helper Function, given a temperature, set LEDs to different colors
  if (temp > 35) {             //------------------------------------------------When the temperature is higher than 35 (35 used for demo, COVID tests for 38 and above, Temperature sensor has error of +-0.5 so setting to 38.5 and above is safe)
     digitalWrite(rled, HIGH); //------------------------------------------------Turn on the red LED
     delay(500);               //------------------------------------------------Pause for half a second (500 milliseconds)
     digitalWrite(rled, LOW);  //------------------------------------------------Turn the red LED off again
  }
  else {
    digitalWrite(gled, HIGH); //-------------------------------------------------When the temperature is lower or equal to 35
    delay(500);               //-------------------------------------------------Pause for half a second
    digitalWrite(gled, LOW);  //-------------------------------------------------Turn the green LED off again
  }
  
}

void espSend(float data) { //------------------------Helper Function, given a float, send to ESP
  espSerial.println(data, 3); //-------------------------------------------------Send float via Software Serial to ESP

}
