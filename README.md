# PcInt_Encoder_Motor_from_any_pin

https://github.com/curiores/ArduinoTutorials has a youtube video that is best of class to describe their also magnificent example for encoder motors.

Used this library https://github.com/SodaqMoja/Sodaq_PcInt to use PcInt to control an encoder motor from pins other than 2 and 3 on a Nano

https://www.youtube.com/shorts/ZZA0OGO0D1g proof of concept.

Found that the pos- was getting twice as often and changed the code to add another variable cutinhalf to only trigger the pos- every other hit.

Using this 


void readEncoder(){

    c = digitalRead(ENCODERA);
    
    d = digitalRead(ENCODERB);
    
    if(d != c){
      posi1++;
    }
    else{
      if(cutinhalf){
      posi1--;
      }
      cutinhalf = !cutinhalf;
    }
