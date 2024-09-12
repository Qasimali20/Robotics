// Motor speed control for forward-backward and left-right.
int FB_speed = 250, LR_speed = 250;

// Pins to control the direction for motors rotation
int pin1 = 3, pin2 = 5, pin3 = 6, pin4 = 9;

// Ultrasonic sensors pins
//left=1, forward=2, right=3 
int trig1 = 2, echo1 = 4, trig2 = 7, echo2 = 8, trig3 = 10, echo3 = 11;

float duration;
float distance1, distance2, distance3;
// Threshold distances for all three sensors 
const float set_distance1 = 35, set_distance2 = 35, set_distance3 = 35;

void setup() {
    Serial.begin(9600);
    // Set motor control pins as outputs
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);
    pinMode(pin4, OUTPUT);

    // Set ultrasonic sensor pins
    pinMode(trig1, OUTPUT);
    pinMode(trig2, OUTPUT);
    pinMode(trig3, OUTPUT); 
    pinMode(echo1, INPUT);
    pinMode(echo2, INPUT);
    pinMode(echo3, INPUT);

    Serial.println("Setup Complete");
}

void loop() {
    self_driving();
}

void self_driving() {
    // Measure distances from ultrasonic sensors
    distance1 = dist_measure(trig1, echo1);
    distance2 = dist_measure(trig2, echo2);
    distance3 = dist_measure(trig3, echo3);

    // Debugging: Print sensor readings
//    Serial.print("Distance Left: ");
//    Serial.println(distance1);
//    Serial.print("Distance Forward: ");
//    Serial.println(distance2);
//    Serial.print("Distance Right: ");
//    Serial.println(distance3);

    // Make decision based on distances
    if (distance2 > set_distance2) {
        Serial.println("Moving Forward"); // Debugging
        move_forward();              // Forward
    }
    else if (distance1 > set_distance1 && distance1 > distance3) {
        Serial.println("Turning Right"); // Debugging
        turn_right();                // Right turn
    }
    else if (distance3 > set_distance3) {
        Serial.println("Turning Left"); // Debugging
        turn_left();                 // Left turn
    }
    else {
        Serial.println("No Clear Path, Moving Backward"); // Debugging
        move_backward_until_clear();  // No clear path, move backward until there is space
    }
}

void move_forward() {
    analogWrite(pin1, FB_speed);
    analogWrite(pin2, LOW);
    analogWrite(pin3, FB_speed);
    analogWrite(pin4, LOW);
    
    Serial.println("Action: Forward"); // Debugging
}

void turn_right() {
    analogWrite(pin1, LOW);
    analogWrite(pin2, LR_speed);
    analogWrite(pin3, LOW);
    analogWrite(pin4, LR_speed);

    Serial.println("Action: Right Turn"); // Debugging
}

void turn_left() {
    analogWrite(pin1, LR_speed);
    analogWrite(pin2, LOW);
    analogWrite(pin3, LR_speed);
    analogWrite(pin4, LOW);
    
    Serial.println("Action: Left Turn"); // Debugging
}

void move_backward_until_clear() {
    while (true) {
        // Measure distances again
        distance1 = dist_measure(trig1, echo1);
        distance3 = dist_measure(trig3, echo3);

        // Debugging: Print distances while moving backward
//        Serial.print("Checking Distances while moving backward - Left: ");
//        Serial.print(distance1);
//        Serial.print(", Right: ");
//        Serial.println(distance3);

        if (distance1 > set_distance1 || distance3 > set_distance3)
            break;

        // Move backward
        analogWrite(pin1, LOW);
        analogWrite(pin2, FB_speed);
        analogWrite(pin3, FB_speed);
        analogWrite(pin4, LOW);

        Serial.println("Action: Reverse"); // Debugging
    }

    // Decide which way to turn after moving backward
    if (distance1 > set_distance1 && distance1 > distance3) {
//        Serial.println("After Reverse, Turning Right"); // Debugging
        turn_right();
    } else if (distance3 > set_distance3) {
//        Serial.println("After Reverse, Turning Left"); // Debugging
        turn_left();
    }
}

float dist_measure(int trig, int echo) {
    duration = 0;
    // Send a pulse to the trigger pin
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    // Measure the duration of the pulse on the echo pin
    duration = pulseIn(echo, HIGH);
    
    // Calculate the distance based on the duration of the pulse
    float distance = ((duration * 0.034) / 2);

//    // Debugging: Print the measured distance
//    Serial.print("Measured Distance for trig ");
//    Serial.print(trig);
//    Serial.print(" and echo ");
//    Serial.print(echo);
//    Serial.print(": ");
//    Serial.println(distance);
//    
    return distance;
}
