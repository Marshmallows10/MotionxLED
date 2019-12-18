#include <FastLED.h>
#include <SoftwareSerial.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>

#define ENA 5
#define ENB 10

#define IN1 6
#define IN2 7
#define IN3 8
#define IN4 9

#define NUM_LEDS 15
#define LED_PIN 12

CRGB led[NUM_LEDS];

int buzzer = 7;     // the pin that the LED is atteched to
int sensor = 2;  // the pin that the sensor is atteched to
int sensor1 = 3;
int sensor2 = 4;
int sensor3 = 5;
int state = LOW;             // by default, no motion detected
int LEFT,FRONT,RIGHT,BACK = 0;                 // variable to store the sensor status (value)

ros::NodeHandle node;
geometry_msgs::Twist msg;

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
void forward()
{

  analogWrite(ENA, 85);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  analogWrite(ENB, 85);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
}

void left()
{

  analogWrite(ENA, 85);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
}

void hold()
{

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
}

void right()
{

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  analogWrite(ENB, 85);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
}

void back()
{

  analogWrite(ENA, 85);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  analogWrite(ENB, 85);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
}

void roverCallBack(const geometry_msgs::Twist& cmd_vel)
{

  if(cmd_vel.linear.x > 0 && cmd_vel.angular.z == 0)
  {
    forward(); //i
    analogWrite(test, 255);
  }
  else
  {
    if(cmd_vel.linear.x == 0 && cmd_vel.angular.z > 0)
    {
      left(); //j
      analogWrite(test, 255);
    }
    else
    {
      if(cmd_vel.linear.x == 0 && cmd_vel.angular.z == 0)
      {
        hold(); //k
        analogWrite(test, 0);
      }
      else
      {
        if(cmd_vel.linear.x == 0 && cmd_vel.angular.z < 0)
        {
          right(); //l
          analogWrite(test, 255);
        }
        else
        {
          if(cmd_vel.linear.x < 0 && cmd_vel.angular.z == 0)
          {
            back(); //,
            analogWrite(test, 255);
          }
          else
          {
            hold(); //default
          }
        }
      }
    }
  }
}

ros::Subscriber <geometry_msgs::Twist> sub("cmd_vel", roverCallBack);


void setup() {
  pinMode(led, OUTPUT);      // initalize LED as an output
  pinMode(sensor, INPUT);    // initialize sensor as an input
  Serial.begin(9600);        // initialize serial

  //  Serial.begin(57600);
//  BT1.begin(57600);
  
  node.initNode();
  node.subscribe(sub);
  
  pinMode(test, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
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
    hold();
    
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
      forward();
      
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
  }
}
