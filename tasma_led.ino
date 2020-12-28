#include <IRremote.h>
#include "FastLED.h"
#define NUM_LEDS 45
#define DATA_PIN 7
#define serialRate 115200


const int RECV_PIN = 4;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;
uint8_t gHue = 0;

String type = "color";

uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;


CRGB leds[NUM_LEDS];

//show solid color
void setSolidColor(int red, int green, int blue){
  LEDS.showColor(CRGB(red, green, blue));
}

//show random colors
void randomColors(){
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(random(1,255), random(1,255), random(1,255));
    }  
    FastLED.show();
}

//show rainbow
void showRainbow(){
  while(type == "rainbow"){
    irrecv.resume(); 
    for (int j = 0; j < 255; j++) {
      if (irrecv.decode(&results)){
        type = "color";
         break;
        }
      irrecv.resume(); 
      for (int i = 0; i < NUM_LEDS; i++) {
        if (irrecv.decode(&results)){
          type = "color";
          break;
         } 
        leds[i] = CHSV(i - (j * 2), 255, 255); /* The higher the value 4 the less fade there is and vice versa */ 
      }
    FastLED.show();
    delay(25); 
  }
  }
}


void showAmbilight(){
  irrecv.resume(); 
   while(type == "ambilight"){
      if (irrecv.decode(&results)) type = "color";
      Serial.println("Wykomal osie");
    

      for(i = 0; i < sizeof prefix; ++i) {
      waitLoop: while (!Serial.available()) ;;
      // Check next byte in Magic Word
      if(prefix[i] == Serial.read()) continue;
      // otherwise, start over
      i = 0;
      goto waitLoop;
  }
  
  // Hi, Lo, Checksum  
  while (!Serial.available()) ;;
  hi=Serial.read();
  while (!Serial.available()) ;;
  lo=Serial.read();
  while (!Serial.available()) ;;
  chk=Serial.read();
  
  // If checksum does not match go back to wait
  if (chk != (hi ^ lo ^ 0x55)) {
    i=0;
    goto waitLoop;
  }
  
  memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
  // Read the transmission data and set LED values
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  
  // Shows new values
  FastLED.show();
  }
  
  
}



void setup() {
  irrecv.enableIRIn();
  irrecv.blink13(true);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  LEDS.showColor(CRGB(255, 0, 0));
  delay(500);
  LEDS.showColor(CRGB(0, 255, 0));
  delay(500);
  LEDS.showColor(CRGB(0, 0, 255));
  delay(500);
  LEDS.showColor(CRGB(0, 0, 0));
  FastLED.setBrightness(255);
  Serial.begin(serialRate);
  Serial.print("Ada\n");
}

void loop() {

   
        
if (irrecv.decode(&results)){
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;
        switch(results.value){
          case 0xFFA25D://red
          setSolidColor(255,0,0);
          break;  
          case 0xFF629D://green
          setSolidColor(0,255,0);
          break;
          case 0xFFE21D://blue
          setSolidColor(0,0,255);
          break;
          case 0xFF22DD://cyan  
          setSolidColor(0,255,255);
          break;
          case 0xFF02FD://pink  
          setSolidColor(255,0,255);
          break ;  
          case 0xFFC23D://yellow
          setSolidColor(255,255,0);     
          break ;               
          case 0xFFE01F://power off
          FastLED.clear(true);
          break ;  
          case 0xFFA857://white
          setSolidColor(255,255,255);  
          break ;  
          case 0xFF906F://random colors 
          randomColors();
          break ;  
          case 0xFF6897://rainbow
          type = "rainbow";
          showRainbow();
          break ;  
          case 0xFF9867:
          Serial.println("100+");
          break ;
          case 0xFFB04F:
          Serial.println("200+");
          break ;
          case 0xFF30CF:
          Serial.println("1");
          break ;
          case 0xFF18E7:
          Serial.println("2");
          break ;
          case 0xFF7A85:
          Serial.println("3");
          break ;
          case 0xFF10EF:
          Serial.println("4");
          break ;
          case 0xFF38C7:
          Serial.println("5");
          break ;
          case 0xFF5AA5:
          Serial.println("6");
          break ;
          case 0xFF42BD:
          Serial.println("7");
          break ;
          case 0xFF4AB5:
          Serial.println("8");
          break ;
          case 0xFF52AD:
          Serial.println("9");
          type = "ambilight";
          showAmbilight();    
          break ;      
        }
        key_value = results.value;
        irrecv.resume(); 
  }

  
  /*if(type == "ambi"){
    // Wait for first byte of Magic Word
  for(i = 0; i < sizeof prefix; ++i) {
    waitLoop: while (!Serial.available()) ;;
    // Check next byte in Magic Word
    if(prefix[i] == Serial.read()) continue;
    // otherwise, start over
    i = 0;
    goto waitLoop;
  }
  
  // Hi, Lo, Checksum  
  while (!Serial.available()) ;;
  hi=Serial.read();
  while (!Serial.available()) ;;
  lo=Serial.read();
  while (!Serial.available()) ;;
  chk=Serial.read();
  
  // If checksum does not match go back to wait
  if (chk != (hi ^ lo ^ 0x55)) {
    i=0;
    goto waitLoop;
  }
  
  memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
  // Read the transmission data and set LED values
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }
  
  // Shows new values
  FastLED.show();
    
  }*/
  
}





  
