# PcInt_Encoder_Motor_from_any_pin
Used this library https://github.com/SodaqMoja/Sodaq_PcInt to use PcInt to control an encoder motor from pins other than 2 and 3 on a Nano

https://www.youtube.com/shorts/ZZA0OGO0D1g proof of concept.

Found that the pos- was getting twice as often and changed the code to add another variable cutinhalf to only trigger the pos- every other hit.

Using this 


void readEncoder1(){
    c = digitalRead(ENCODERC);
    d = digitalRead(ENCODERD);
    if(d != c){
      posi1++;
    }
    else{
      if(cutinhalf1){
      posi1--;
      }
      cutinhalf1 = !cutinhalf1;
    }
