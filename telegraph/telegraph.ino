#include <Stepper.h>
#include "chars.h"

#define homepin 2  //limit switch attach pin
#define dotpin 3  //solenoid pin
#define updelay 50
#define downdelay 25

int n;
String Data="";

 Stepper stepperx(20, 10,9,11,12);   //pen stepper
 Stepper steppery(20, 6,5,7,8);      //stepper moving the paper

void setup() {
  stepperx.setSpeed(100);
  steppery.setSpeed(40);
  Serial.begin(9600);
  pinMode(homepin, INPUT);
  pinMode(dotpin, OUTPUT);
 //uncomment both the lines to test without bluetooth
 //Home();    //also take a look at Home(), you have to change steps to make it work
 //printString("Random!");
}


void loop(){
  //this loop read the data from bluetooth and when the last caharacter is '~', it sends tha data to printString() as in String format
  while(Serial.available()){
     char character = Serial.read(); 
        if (character == '~'){
          if(DataNotEmpty()) {
            Serial.print("printing ");
            Serial.println(Data);
            Home();
            printString(Data);
            steppery.step(20);
          }
          
          Data ="";
          delay(1000);
        }
        else  Data.concat(character); 
      }
   }

//checks if Data variable empty or not
bool DataNotEmpty(){
  if(Data[0] == '\0'){
    return false;
  }
  else {
    return true;
  }
}

//for understanding better this part   https://habr.com/en/post/407057/
void printLine(int b) {
  if(b != 0) {
  int k;
    for (int j = 0; b != 0; j++) {
      stepperx.step(3);
      k=j;
      delay(20);
  
      if(b & 1) ping();
      b >>= 1;
    }
    //this moves the stepper to its page start  position after printing each line
    stepperx.step(-3*(k+1));
    
    }
    //space b/w each character
  steppery.step(2);
}


// the function sends each character in char format to printChar() one by one from received string
void printString(String str) {
  int len= str.length();
  for(int i=0; i<len;i++){
   char ch = str[i];
    printChar(ch);
  }
}


//for understanding better this part   https://habr.com/en/post/407057/
void printChar(char c) {
int n = 0;
  
  for (int i = 0; i < 8; i++) {
    if(chars[c][i] != 0) {
      printLine(chars[c][i]);
      n++;
    }

    //space b/w each line
    else steppery.step(1);
  }
}

//make a dot on paper
void ping() {
  digitalWrite(dotpin, 1);
  delay(updelay);
  digitalWrite(dotpin, 0);
  delay(downdelay);
}


//home the pen and move the pen to the staring of the page
void Home(){
  while(digitalRead(homepin) == LOW){
    stepperx.step(-1);
  }
  stepperx.step(45); //change steps value to make sure your pen lands at the start of page where you want to start writing
}
