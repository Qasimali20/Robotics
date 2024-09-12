int trigPin = 10;      // trig pin of HC-SR04
int echoPin = 9;       // Echo pin of HC-SR04
int FB_speed = 150, LR_speed = 150;

int revleft4 = 4;      // REVerse motion of Left motor
int fwdleft5 = 5;      // ForWarD motion of Left motor
int revright6 = 7;     // REVerse motion of Right motor
int fwdright7 = 6;     // ForWarD motion of Right motor

long duration, distance;

void setup() {
  Serial.begin(9600);
  pinMode(revleft4, OUTPUT);      
  pinMode(fwdleft5, OUTPUT);
  pinMode(revright6, OUTPUT);
  pinMode(fwdright7, OUTPUT);
  
  pinMode(trigPin, OUTPUT);        
  pinMode(echoPin, INPUT);         
}

void loop() {
  // Measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);   
  digitalWrite(trigPin, HIGH);     
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH); 
  distance = (duration * 0.034) / 2;  

  Serial.print("Measured Distance: ");
  Serial.println(distance);

  if (distance > 30) {          
    Serial.println("Distance is greater than 30. Moving forward...");
    
    // Move forward
    analogWrite(fwdright7, FB_speed);   
    analogWrite(revright6, LOW);
    analogWrite(fwdleft5, FB_speed);                                
    analogWrite(revleft4, LOW);                                                       

//    Serial.println("Motors set to move forward");
  }
  else {
    Serial.println("Distance is 30 or less. Turning right...");

    // Move right
    analogWrite(fwdright7, LOW);     
    analogWrite(revright6, LR_speed); 
    analogWrite(fwdleft5, LR_speed);                                
    analogWrite(revleft4, LOW);

    delay(400); // Pause to complete the right turn (tune this value)
    
    // Stop motors and recheck distance
    analogWrite(revright6, LOW);
    analogWrite(fwdleft5, LOW);
    analogWrite(revleft4, LOW);

    // Debugging: Indicate stopping after turning right
    Serial.println("Motors stopped after right turn");
  }
  
  delay(100); // Small delay to prevent rapid oscillation

}
