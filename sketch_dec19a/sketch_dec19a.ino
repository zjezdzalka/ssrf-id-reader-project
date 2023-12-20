int calibrationTime = 15;        

long unsigned int lowIn;         

long unsigned int pause = 1500;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 13;    //the digital pin connected to the PIR sensor's output
int ledPin = 3;

void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  Serial.println("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.println();
      Serial.print(i);
      Serial.print("/");
      Serial.print(calibrationTime);
      delay(1000);
      }
    Serial.println();
    Serial.print(calibrationTime);
    Serial.print("/");
    Serial.print(calibrationTime);
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  }

void loop(){

     if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(((millis() - pause)/1000) - 15);
         Serial.print(".");
         Serial.print(((millis() - pause)%1000));
         Serial.print(" sec");
         Serial.println(); 
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
         //the led visualizes the sensors output pin state
       if(!lockLow){
        digitalWrite(ledPin, HIGH);
        Serial.print("motion still active at: ");
        Serial.print(((millis() - pause)/1000) - 15);
        Serial.print(".");
        Serial.print(((millis() - pause))%1000);
        Serial.print(" sec");
        Serial.println();
       }
       else{
        digitalWrite(ledPin, LOW);
       }
       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");   
          Serial.print(((millis() - pause)/1000) - 15);
          Serial.print(".");
          Serial.print(((millis() - pause))%1000);
          Serial.print(" sec");
           delay(50);
           }
       }

  }