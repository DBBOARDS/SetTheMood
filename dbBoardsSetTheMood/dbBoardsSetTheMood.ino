/*-----------------------------------------------------------------------------------------------------------
dbBoardsSetTheMood.ino

  Summary:
    Using the slider, turn knob, and photocell on the DB Analog Board allows you to control the color and
    brightness of the lights attached. You can also set the hardware to turn on at night or replicate a fire.
  
  Utilizing:
    Adafruit NeoPixel Library: https://github.com/adafruit/Adafruit_NeoPixel
   
  Programmer:
    Duncan Brandt @ DB Boards, LLC
    Created: Feb 9, 2018
  
  Development Environment Specifics:
    Arduino 1.6.11
  
  Hardware Specifications:
    Arduino Drawing Board (UNO) DB1000
    WS2812 Strip 25 DB7002
    DB Analog Board Kit DB5004

  Beerware License:
    This program is free, open source, and public domain. The program is distributed as is and is not
    guaranteed. However, if you like the code and find it useful I would happily take a beer should you 
    ever catch me at the local.
*///---------------------------------------------------------------------------------------------------------
#include <Adafruit_NeoPixel.h>        // https://github.com/adafruit/Adafruit_NeoPixel
#define PIN 1                         // Pin 1 for communication with LED strip
// 25  Number of pixels in strip, NEO_GRB  Pixels are wired for GRB bitstream, NEO_KHZ800  800 KHz bitstream
Adafruit_NeoPixel strip = Adafruit_NeoPixel(25, PIN, NEO_GRB + NEO_KHZ800);
//-----------------------------------------------------------------------------------------------------------
int slide = 2, turn = 1, photo = 0;   // Analog pins for LED controls

//-----------------------------------------------------------------------------------------------------------
void setup() {                        // The setup program runs only once after power-up or reset
  strip.begin();                      // Activate the LED strand
  strip.show();                       // Initialize all pixels to 'off'
  randomSeed(analogRead(5));          // Use floating Analog pin to generate random sequence
  
}
//-----------------------------------------------------------------------------------------------------------
void loop() {                         // Loop checks the analog board to create the users desired display
  int maxB = analogRead(turn);        // Get the brightness from the turn knob
  maxB = map(maxB, 0, 1023, 255, 20); // Map the value between 255 and 20 for the LEDs so it is like volume
  // If the lights in the room are off and the brightness is set to 0, turn the stip to full brightness
  if(maxB == 20 && analogRead(photo) < 600) maxB = 255;
  int slideVal = analogRead(slide);   // Get the color selection from the slider and map it to 3x brightness
  slideVal = map(slideVal, 0, 1023, 0, maxB * 3);
  if(slideVal < 3){                   // If the slider is pulled close to all the way down
    int fire = random(0, 20);         // Create a random flare value on the strip
    firePlace(maxB, fire);            // Run the fireplace visual with the new flare position
  }
  else{
    showColor(maxB,slideVal);      // Otherwise, use the value to select and display a color
    delay(300);
  }
}
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
void showColor(int bright, int val){  // Use the slider value to create the selected color on the LEDs
  if(val < bright){                   // If the slider is in the first third
    for(int a = 0; a < 25; a++){      // Turn all the LEDs to the selected red/green color
      strip.setPixelColor(a, strip.Color(bright-val,val,0));
    }
  }
  else if(val < 2*bright){            // If the slider is in the second third
    for(int b = 0; b < 25; b++){      // Turn all the LEDs to the selected green/blue color
      strip.setPixelColor(b, strip.Color(0,bright-(val-bright),val-bright));
    }
  }
  else{                               // If the slider is in the last third
    for(int c = 0; c < 25; c++){      // Turn all the LEDs to the selected blue/red color
      strip.setPixelColor(c, strip.Color(val-(2*bright),0,bright-(val-(2*bright))));
    }
  }
  strip.show();                       // Show the current color on the LED strip
}
//-----------------------------------------------------------------------------------------------------------
void firePlace(int bri, int flare){   // Create a lighting effect of burning embers
  int flareDelay = random(1,100);     // Create a random delay for the flare to give the fire life
  for(int d = 0; d < flareDelay; d++){// During that delay
    for(int e = 0; e < 25; e++){      // Set every LED to a random redish yellow color
      strip.setPixelColor(e,strip.Color(
        random((bri/2)+(bri/3),(bri/2)+(bri/3)+(bri/20)),random((bri/2)-(bri/3)-(bri/20),(bri/2)-(bri/3)),0));
    }
    strip.show();                     // Show the new random strip
    delay(30);                        // Wait for 30 milliseconds to run the for loop again
  }
  for(int f = 0; f < 7; f++){         // Wait through the 5 pixel brush cycle
    for(int g = 0; g < 25; g++){      // Turn all the LEDs to a random redish yellow color
      strip.setPixelColor(g,strip.Color(
        random((bri/2)+(bri/3),(bri/2)+(bri/3)+(bri/20)),random((bri/2)-(bri/3)-(bri/20),(bri/2)-(bri/3)),0));
    }
    // Brush 3 LEDs over a 5 LED space to create a flare with brighter color
    if(f<3) strip.setPixelColor(flare, strip.Color(bri,bri/5,bri/20));
    if(f>0 && f<4) strip.setPixelColor(flare+1, strip.Color(bri,bri/5,bri/20));
    if(f>1 && f<5) strip.setPixelColor(flare+2, strip.Color(bri,bri/5,bri/20));
    if(f>2 && f<6) strip.setPixelColor(flare+3, strip.Color(bri,bri/5,bri/20));
    if(f>3) strip.setPixelColor(flare+4, strip.Color(bri,bri/5,bri/20));
    strip.show();                     // Show the next step of the flare
    delay(30);                        // Wait 30 milliseconds to continue the flare
  }
}
//-----------------------------------------------------------------------------------------------------------

