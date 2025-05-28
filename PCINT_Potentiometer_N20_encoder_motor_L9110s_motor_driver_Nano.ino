
// https://github.com/curiores/ArduinoTutorials

// https://github.com/SodaqMoja/Sodaq_PcInt

//  adapted to work with a two wire motor driver like a L9110s, the driver is for the motor to control steering
//  Storming Moose May 2025

// This is setup for an Arduino Nano.


#include <Sodaq_PcInt.h>
#include <util/atomic.h> // For the ATOMIC_BLOCK macro a nano/uno tool to help with interrupts

// Input Pins
#define ENCODERA  8
#define ENCODERB 9 
#define STEERINGPIN A0          

// Output Pins
#define STEERINGPWRPIN  4    
#define STEERINGDIRPIN  5 

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
long prevT = 0;
float eprev = 0;
float eintegral = 0;

int a = 0;
int b = 0;


void setup() {
  Serial.begin(9600);
  pinMode(ENCODERA,INPUT);
  pinMode(ENCODERB,INPUT);
  pinMode(STEERINGPIN,INPUT);

  PcInt::attachInterrupt(ENCODERA,readEncoder);

  pinMode(STEERINGPWRPIN,OUTPUT);
  pinMode(STEERINGDIRPIN,OUTPUT);
  
}

void loop() {

int target = 1.2*analogRead(STEERINGPIN);   // the 1 is used to increase the number of rotations to facilitate gearing differences

  // PID constants
  float kp = 1;
  float kd = 0.005;   // change this if the steering corrects too often, avoid delays, they increase the size and number of corrections
  float ki = 0.0;

  // time difference
  long currT = micros();
  float deltaT = ((float) (currT - prevT))/( 1.0e6 );                    // how long since last time
  prevT = currT;

  // Read the position
  int pos = 0; 
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    pos = posi;                                        // grabs the current position in a variable before it changes again
  }
  
  // error
  int e = pos - target;                               // are far away from target

  // derivative
  float dedt = (e-eprev)/(deltaT);                    //how far away less how far away last time divided by how much time has passed
                                                      // the rate we are moving per microsecond
  // integral
  eintegral = eintegral + e*deltaT;                   // zer0 + how far away multiplied by how much time has passed or displacement a vector quantity

  // control signal
  float u = kp*e + kd*dedt + ki*eintegral;           // calculates power required 1* how far away + adjustment factor multiplied by rate we are moving + zero as the ki = 0

  // motor power
  float pwr = fabs(u);       // absolute value of power required
  if( pwr > 255 ){
    pwr = 255;
  }

  // motor direction
  int dir = 1;
  if(u<0){
    dir = -1;
  }

  // signal the motor
  setMotor(dir,pwr,STEERINGPWRPIN,STEERINGDIRPIN);


  // store previous error
  eprev = e;

}

void setMotor(int dir, int pwr, int pwmpin, int dirpin){   // CAPITAL above letters describe this particular setup,
                                                          // using the lowercase characters here allow this programming module 
  if(dir == 1){  // reverse                               // to be used with more than one encoder motor, as in a different program or situation.
    digitalWrite(dirpin,HIGH);                            // here it is just used for the steering motor 
    analogWrite(pwmpin,255-pwr);
  }
  else if(dir == -1){   // forward
    digitalWrite(dirpin,LOW);
    analogWrite(pwmpin,pwr);
  }
  else{
    digitalWrite(dirpin,LOW);
    analogWrite(pwmpin,0);
  }  
}

void readEncoder(){
    a = digitalRead(ENCODERA);
    b = digitalRead(ENCODERB);
    if(b !=  a){
      posi++;
    }
    else{
      posi--;
    }

/*  int b = digitalRead(ENCODERB);
  if(b > 0){
    posi++;
  }
  else{
    posi--;
  }
*/
}

