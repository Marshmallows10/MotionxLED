#include <FastLED.h>

#define NUM_LEDS 10
#define LED_PIN 12

CRGB led[NUM_LEDS];

int buzzer = 7;     // the pin that the LED is atteched to
int sensor = 2;  // the pin that the sensor is atteched to
int sensor1 = 3;
int sensor2 = 4;
int sensor3 = 5;
int state = LOW;             // by default, no motion detected
int LEFT,FRONT,RIGHT,BACK = 0;                 // variable to store the sensor status (value)

void setup() {
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input
  Serial.begin(9600);        // initialize serial
}

void Red() {

FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LEDS);

  for (int i = 0; i < NUM_LEDS; i++){
    led[i] = CRGB(225, 0, 0);    
    }
    
}

void Green() {

FastLED.addLeds<NEOPIXEL, LED_PIN>(led, NUM_LEDS);

  for (int i = 0; i < NUM_LEDS; i++){
    led[i] = CRGB(0, 225, 0);    
    }
    
}
 
void loop(){
  LEFT = digitalRead(sensor);   // read sensor value
  FRONT = digitalRead(sensor1);
  RIGHT = digitalRead(sensor2);
  BACK = digitalRead(sensor3);
  if (LEFT == HIGH || FRONT == HIGH || RIGHT == HIGH || BACK == HIGH) {           // check if the sensor is HIGH
    digitalWrite(buzzer, HIGH);   // turn LED ON
    delay(100);                // delay 100 milliseconds 
    Red();
    delay(100);
    FastLED.show();
    
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       // update variable state to HIGH
    }
  } 
  else {
      digitalWrite(buzzer, LOW); // turn LED OFF
      delay(200);             // delay 200 milliseconds 
      Green();
      delay(100);
      FastLED.show();
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
  }
}
