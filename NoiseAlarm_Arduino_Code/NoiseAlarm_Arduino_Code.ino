#include <TM1637Display.h>

const int MIC_AO        = 0;    // Microphone Analogic output
const int MIC_DO        = 11;   // Microphone Digital output 
const int LED           = 10;   // High power LED signal
const int DISPLAY_CLK   = 9;    // Display Clock
const int DISPLAY_DIO   = 8;    // Display Digital Input Output
const int BUZZER        = 3;    // Define buzzerPin
const int DISPLAY_INT   = 0x00; // Display intensity (from 0 to 7)

int adc;
int dB, PdB; //the variable that will hold the value read from the microphone each time
int counter = 0; 
int noise_counter = 0; 
int cooldown_timer = 0; 

TM1637Display display(DISPLAY_CLK, DISPLAY_DIO);  //set up the 4-Digit Display.


void beep(unsigned char delayms) { 
  tone(BUZZER, 523, 100);
}

void setup() {
  display.setBrightness(DISPLAY_INT);  //set the diplay to minimum brightness
  pinMode(LED, OUTPUT);
  pinMode(MIC_DO, INPUT);
}

void printDataScreen()
{
  if(counter <= 0)
  {
    if(digitalRead(MIC_DO) == HIGH)
    {
      if(noise_counter < 255)
      {
        noise_counter+=5; 
        
      }
      if(noise_counter == 255)
      {
        beep(100);
        noise_counter=175;
      }
    }else 
    {
      if(noise_counter > 0 && cooldown_timer <= 0)
      {
        noise_counter-=2;
        cooldown_timer = 20;
      }else
      {
        if(noise_counter < 0)
        {
          noise_counter = 0;
        } 
        cooldown_timer--; 
      }
    }
    //int roundedVal = (noise_counter/10.0) * 100; 
    //display.showNumberDecEx(roundedVal,0b00100000); //Display the Variable value;
    display.showNumberDecEx(noise_counter);
    counter = 100; 
  }else{
    counter--; 
  }
}
void loop(){

  PdB = dB; //Store the previous of dB here
  adc= analogRead(MIC_AO); //Read the ADC value from amplifer 
  dB = (adc+83.2073) / 11.003; //Convert ADC value to dB using Regression values

  
  
  printDataScreen(); 
  analogWrite(LED,noise_counter); 
}


